/**
@file
Расширение драйвера datastruct/sline, реализующее логику работы командной строки
в парадигме readline. Драйвер умеет преобразовывать управляющие входные
последовательности в команды sline и реализует механизм работы с историей
предыдущих команд.

Драйвер реализован в виде callback-less машины состояний и предполагает
наличие внешнего уровня, интерпретирующего возвращаемые машиной статусы.

TODO: Вынести драйвер работы с историей как отдельный объект.
*/

#ifndef IGRIS_SHELL_READLINE_H
#define IGRIS_SHELL_READLINE_H

#include <igris/datastruct/sline.h>
#include <igris/defs/ascii.h>
#include <igris/util/bug.h>

#define READLINE_OVERFLOW -1

#define READLINE_NOTHING 0
#define READLINE_ECHOCHAR 1

#define READLINE_NEWLINE 2
#define READLINE_BACKSPACE 3
#define READLINE_DELETE 4
#define READLINE_HOME 5
#define READLINE_END 6

#define READLINE_UPDATELINE 7

#define READLINE_LEFT 8
#define READLINE_RIGHT 9

#define READLINE_STATE_NORMAL 0
#define READLINE_STATE_ESCSEQ 1
#define READLINE_STATE_ESCSEQ_MOVE 2
#define READLINE_STATE_ESCSEQ_MOVE_WAIT_7E 3

namespace igris
{
    class readline
    {
        sline _line;
        int _state;

        char _last;
        int lastsize;

        char *_history_space; // Указатель на буффер истории.
        uint8_t _history_size; // Количество строк в буффере истории.

        uint8_t _headhist; // Индекс в массиве, куда будет перезаписываться
                           // новая строка истории.
        uint8_t _curhist; // Индекс выбора строки истории (0-пустая,
                          // 1-последняя, 2-предпоследняя и т.д.)

    public:
        sline &line()
        {
            return _line;
        }

        void init(char *buf, size_t len)
        {
            sline_init(&_line, buf, (unsigned int)len);

            _last = 0;
            _state = 0;
            _history_space = NULL;
            _curhist = 0;
            _headhist = 0;
        }

        void history_init(char *hs, int hsize)
        {
            _history_space = hs;
            _history_size = hsize;
            memset(hs, 0, _line.cap * hsize);
        }

        void newline_reset()
        {
            // Когда случается новая линия, сбрасываем буффер ввода и скидываем
            // переключатель строки истории на последнюю строку.
            sline_reset(&_line);
            _curhist = 0;
        }

        // Указатель на строку, взятую от последней пришедшей. (1 - последняя)
        char *history_pointer(int num)
        {
            int idx = (_headhist + _history_size - num) % _history_size;
            return _history_space + idx * _line.cap;
        }

        // Вернуть указатель на строку истории, на которую указывает _curhist.
        // Вызывается на кнопки вверх/вниз
        char *current_history_pointer()
        {
            return history_pointer(_curhist);
        }

        void _push_line_to_history(const char *str, size_t len)
        {
            char *ptr = _history_space + _headhist * _line.cap;
            memcpy(ptr, str, len);
            *(ptr + len) = '\0';
            _headhist = (_headhist + 1) % _history_size;
        }

        void _push_line_to_history(const char *str)
        {
            _push_line_to_history(str, strlen(str));
        }

        // Обновить историю, записав туда новую строку.
        void _push_current_line_to_history()
        {
            _push_line_to_history(_line.buf, _line.len);
        }

        void load_history_line()
        {
            lastsize = _line.len;

            if (_curhist == 0)
            {
                memset(_line.buf, 0, _line.cap);
                _line.len = 0;
                _line.cursor = 0;
                return;
            }

            unsigned int sz = (unsigned int)strlen(current_history_pointer());

            memcpy(_line.buf, current_history_pointer(), sz);
            _line.len = _line.cursor = sz;
        }

        int history_up()
        {
            // DTRACE();
            if (_history_space == NULL)
                return 0;

            if (_curhist == _history_size)
                return 0;

            _curhist++;

            load_history_line();
            return 1;
        }

        int is_not_same_as_last()
        {
            return !sline_equal(&_line, history_pointer(1));
        }

        int history_down()
        {
            if (_history_space == NULL)
                return 0;

            if (_curhist == 0)
                return 0;

            _curhist--;

            load_history_line();
            return 1;
        }

        int newdata(char c)
        {
            int ret;
            int retcode;

            switch (_state)
            {
            case READLINE_STATE_NORMAL:
                switch (c)
                {
                case '\r':
                case '\n':
                    // TODO: Возможно тут некорректно отрабатывается комбинация
                    // rnrnrnrn
                    if ((_last == '\n' || _last == '\r') && _last != c)
                    {
                        _last = 0;
                        retcode = READLINE_NOTHING;
                    }
                    else
                    {
                        if (_history_space && _line.len &&
                            is_not_same_as_last())
                        {
                            // Если есть буффер истории и введенная строка не
                            // нулевая и отличается от последней, сохраняем
                            // строку в историю.
                            _push_current_line_to_history();
                        }
                        _curhist = 0;

                        retcode = READLINE_NEWLINE;
                    }
                    break;

                case ASCII_BS:
                    ret = sline_backspace(&_line, 1);
                    retcode = ret ? READLINE_BACKSPACE : READLINE_NOTHING;
                    break;

                case ASCII_ESC:
                    _state = READLINE_STATE_ESCSEQ;
                    retcode = READLINE_NOTHING;
                    break;

                default:
                    sline_putchar(&_line, c);
                    retcode = READLINE_ECHOCHAR;
                    break;
                }
                break;

            case READLINE_STATE_ESCSEQ:
                switch (c)
                {
                case 0x5B:
                    _state = READLINE_STATE_ESCSEQ_MOVE;
                    break;

                default:
                    // dprln("?warning in readline?");
                    _state = READLINE_STATE_NORMAL;
                    break;
                }
                retcode = READLINE_NOTHING;
                break;

            case READLINE_STATE_ESCSEQ_MOVE:
                retcode = READLINE_NOTHING;
                switch (c)
                {
                case 0x41: // up
                    if (history_up())
                        retcode = READLINE_UPDATELINE;
                    break;
                case 0x42: // down
                    if (history_down())
                        retcode = READLINE_UPDATELINE;
                    break;
                case 0x43: // right
                    ret = sline_right(&_line);
                    if (ret)
                        retcode = READLINE_RIGHT;
                    break;
                case 0x44: // left
                    ret = sline_left(&_line);
                    if (ret)
                        retcode = READLINE_LEFT;
                    break;
                case 0x33:
                    ret = sline_delete(&_line, 1);
                    if (ret)
                        retcode = READLINE_DELETE;
                    _state = READLINE_STATE_ESCSEQ_MOVE_WAIT_7E;
                    _last = c;
                    return retcode;
                default:
                    // dpr("unr esc: hex:");
                    // dprhex(c);
                    // dpr(" dec:");
                    // dprln((int)c);
                    ;
                }
                _state = READLINE_STATE_NORMAL;
                break;

            case READLINE_STATE_ESCSEQ_MOVE_WAIT_7E:
                // if (c != '\x7E')
                //    dprln("?esc?");
                _state = READLINE_STATE_NORMAL;
                retcode = READLINE_NOTHING;
                break;

            default:
                // автомат находится в непредусмотренном состоянии
                // сбрасываем состояние и завершаем автомат.
                _state = READLINE_STATE_NORMAL;
                retcode = READLINE_NOTHING;
            }

            _last = c;
            return retcode;
        }

        int linecpy(char *data, size_t size)
        {
            int len =
                (int)size - 1 > (int)_line.len ? (int)_line.len : (int)size - 1;

            memcpy(data, _line.buf, len);
            data[len] = 0;

            return len;
        }
    };
}

#endif
