// Readline-машина состояний для встраиваемых терминалов.
// Преобразует escape-последовательности в команды sline и реализует историю
// команд.

#ifndef IGRIS_SHELL_READLINE_H
#define IGRIS_SHELL_READLINE_H

#include <algorithm>
#include <igris/container/sline.h>
#include <igris/container/unbounded_array.h>
#include <igris/util/bug.h>

namespace igris
{
    // Коды возврата readline::newdata()
    enum class ReadlineResult : int
    {
        Overflow = -1,
        Nothing = 0,
        EchoChar = 1,
        Newline = 2,
        Backspace = 3,
        Delete = 4,
        Home = 5,
        End = 6,
        UpdateLine = 7,
        Left = 8,
        Right = 9
    };

    // ASCII управляющие символы
    struct Ascii
    {
        static constexpr char BS = '\b';
        static constexpr char DEL = 0x7F; // Многие терминалы шлют DEL вместо BS
        static constexpr char ESC = 0x1B;
        static constexpr char CR = '\r';
        static constexpr char LF = '\n';
    };

    class readline
    {
        // Состояния автомата
        enum class State : int
        {
            Normal = 0,
            EscSeq = 1,      // Получен ESC, ждём '['
            EscSeqMove = 2,  // Получен ESC[, ждём команду
            EscSeqWait7E = 3 // Ждём '~'
        };

        igris::sline _line = {};
        State _state = State::Normal;

        char _last = 0;
        size_t _lastsize = 0;

        igris::unbounded_array<char> _history_space = {};
        igris::unbounded_array<char> _buffer_space = {};

        uint8_t _headhist = 0; // Индекс для записи следующей строки истории
        uint8_t _curhist = 0; // Текущий выбор истории (0 = текущий ввод)

    public:
        readline() = default;

        readline(size_t maxsize, size_t history_deep)
        {
            init(maxsize, history_deep);
        }

        void init(size_t maxsize, size_t history_deep)
        {
            _last = 0;
            _state = State::Normal;
            _curhist = 0;
            _headhist = 0;
            _buffer_space.resize(maxsize);
            _history_space.resize(history_deep * maxsize);
            std::fill(_buffer_space.begin(), _buffer_space.end(), '\0');
            std::fill(_history_space.begin(), _history_space.end(), '\0');
            _line.init(maxsize);
        }

        // Обработка входного символа
        ReadlineResult newdata(char c)
        {
            ReadlineResult result;

            switch (_state)
            {
            case State::Normal:
                result = process_normal(c);
                break;
            case State::EscSeq:
                result = process_escape_seq(c);
                break;
            case State::EscSeqMove:
                result = process_escape_move(c);
                break;
            case State::EscSeqWait7E:
                result = process_escape_wait_7e(c);
                break;
            default:
                _state = State::Normal;
                result = ReadlineResult::Nothing;
            }

            _last = c;
            return result;
        }

        // Сброс буфера для новой строки
        void newline_reset()
        {
            _line.reset();
            _curhist = 0;
        }

        sline &line()
        {
            return _line;
        }
        const sline &line() const
        {
            return _line;
        }
        size_t lastsize() const
        {
            return _lastsize;
        }

        // Копирование текущей строки во внешний буфер
        int linecpy(char *data, size_t size) const
        {
            int len = static_cast<int>(size) - 1 >
                              static_cast<int>(_line.current_size())
                          ? static_cast<int>(_line.current_size())
                          : static_cast<int>(size) - 1;
            memcpy(data, _line.data(), len);
            data[len] = 0;
            return len;
        }

    private:
        ReadlineResult process_normal(char c)
        {
            switch (c)
            {
            case Ascii::CR:
            case Ascii::LF:
                return process_newline(c);
            case Ascii::BS:
            case Ascii::DEL:
                return process_backspace();
            case Ascii::ESC:
                _state = State::EscSeq;
                return ReadlineResult::Nothing;
            default:
                _line.newdata(c);
                return ReadlineResult::EchoChar;
            }
        }

        ReadlineResult process_newline(char c)
        {
            // Игнорируем вторую часть CR+LF / LF+CR
            if ((_last == Ascii::LF || _last == Ascii::CR) && _last != c)
            {
                _last = 0;
                return ReadlineResult::Nothing;
            }

            // Сохраняем в историю если есть что сохранять и отличается от
            // последней
            if (_history_space.size() > 0 && _line.current_size() > 0 &&
                is_different_from_last_history())
            {
                push_current_line_to_history();
            }

            _curhist = 0;
            return ReadlineResult::Newline;
        }

        ReadlineResult process_backspace()
        {
            int ret = _line.backspace(1);
            return ret ? ReadlineResult::Backspace : ReadlineResult::Nothing;
        }

        ReadlineResult process_escape_seq(char c)
        {
            if (c == 0x5B)
                _state = State::EscSeqMove;
            else
                _state = State::Normal;
            return ReadlineResult::Nothing;
        }

        ReadlineResult process_escape_move(char c)
        {
            ReadlineResult result = ReadlineResult::Nothing;

            switch (c)
            {
            case 0x41: // Вверх
                if (history_up())
                    result = ReadlineResult::UpdateLine;
                break;
            case 0x42: // Вниз
                if (history_down())
                    result = ReadlineResult::UpdateLine;
                break;
            case 0x43: // Вправо
                if (_line.right())
                    result = ReadlineResult::Right;
                break;
            case 0x44: // Влево
                if (_line.left())
                    result = ReadlineResult::Left;
                break;
            case 0x33: // Delete (ESC [ 3 ~)
                if (_line.del(1))
                    result = ReadlineResult::Delete;
                _state = State::EscSeqWait7E;
                return result;
            case 0x48: // Home
                // TODO
                break;
            case 0x46: // End
                // TODO
                break;
            }

            _state = State::Normal;
            return result;
        }

        ReadlineResult process_escape_wait_7e(char c)
        {
            (void)c;
            _state = State::Normal;
            return ReadlineResult::Nothing;
        }

        // --- История ---

        size_t history_size() const
        {
            if (_buffer_space.size() == 0)
                return 0;
            return _history_space.size() / _buffer_space.size();
        }

        char *history_pointer(int num)
        {
            int idx = (_headhist + history_size() - num) % history_size();
            return _history_space.data() + idx * _buffer_space.size();
        }

        const char *history_pointer(int num) const
        {
            int idx = (_headhist + history_size() - num) % history_size();
            return _history_space.data() + idx * _buffer_space.size();
        }

        char *current_history_pointer()
        {
            return history_pointer(_curhist);
        }

        bool is_different_from_last_history() const
        {
            return !_line.equal(history_pointer(1));
        }

        void push_current_line_to_history()
        {
            char *ptr =
                _history_space.data() + _headhist * _buffer_space.size();
            memcpy(ptr, _line.data(), _line.current_size());
            ptr[_line.current_size()] = '\0';
            _headhist = (_headhist + 1) % history_size();
        }

        void load_history_line()
        {
            _lastsize = _line.current_size();

            if (_curhist == 0)
            {
                _line.clear();
                _line.reset();
                return;
            }

            const char *hist = current_history_pointer();
            if (hist[0] == '\0')
                return;

            size_t sz = strlen(hist);
            memcpy(_line.data(), hist, sz);
            _line.set_size_and_cursor(sz, sz);
        }

        bool history_up()
        {
            if (history_size() == 0)
                return false;
            if (_curhist >= history_size())
                return false;

            const char *next_hist = history_pointer(_curhist + 1);
            if (next_hist[0] == '\0')
                return false;

            _curhist++;
            load_history_line();
            return true;
        }

        bool history_down()
        {
            if (history_size() == 0)
                return false;
            if (_curhist == 0)
                return false;

            _curhist--;
            load_history_line();
            return true;
        }
    };

}

#endif