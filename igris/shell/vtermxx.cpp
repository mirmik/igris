#include <igris/defs/signal.h>
#include <igris/defs/vt100.h>
#include <igris/shell/vtermxx.h>

namespace igris
{

    void vtermxx::init(unsigned int buffer_size, unsigned int history_size)
    {
        _state = State::ShowPrompt;
        _echo = true;
        _prompt = "$ ";
        _write_callback = {};
        _execute_callback = {};
        _signal_callback = {};
        _rl.init(buffer_size, history_size);
    }

    void vtermxx::write(const char *data, unsigned int len)
    {
        if (_write_callback)
            _write_callback(data, len);
    }

    void vtermxx::write_str(const char *str)
    {
        write(str, static_cast<unsigned int>(strlen(str)));
    }

    void vtermxx::execute_line()
    {
        if (_execute_callback)
            _execute_callback(_rl.line().getline(), _rl.line().current_size());
        _state = State::AfterNewline;
    }

    void vtermxx::newdata(int16_t input_c)
    {
        char c = 0;

        while (true)
        {
            switch (_state)
            {
            case State::ShowPrompt:
            case State::AfterNewline:
                _rl.newline_reset();
                if (_echo)
                    write_str(_prompt);
                _state = State::WaitInput;
                break;

            case State::WaitInput:
                if (input_c < 0)
                    return;
                c = static_cast<char>(input_c);
                input_c = -1;
                _state = State::ProcessChar;
                break;

            case State::ProcessChar:
            {
                // Ctrl+C
                if (c == 0x03)
                {
                    if (_echo)
                        write("^C\r\n", 4);
                    _state = State::AfterNewline;
                    if (_signal_callback)
                        _signal_callback(SIGINT);
                    break;
                }

                ReadlineResult result = _rl.newdata(c);
                handle_readline_result(result, c);

                if (result == ReadlineResult::Newline)
                    return;

                _state = State::WaitInput;
                break;
            }

            default:
                _state = State::ShowPrompt;
                return;
            }
        }
    }

    void vtermxx::handle_readline_result(ReadlineResult result, char c)
    {
        switch (result)
        {
        case ReadlineResult::EchoChar:
            if (_echo)
                write(&c, 1);

            // Перерисовка правой части если курсор не в конце
            if (!_rl.line().in_rightpos() && _echo)
            {
                write(_rl.line().rightpart(), _rl.line().rightsize());
                char buf[16];
                int len = vt100_left(buf, _rl.line().rightsize());
                write(buf, len);
            }
            break;

        case ReadlineResult::Newline:
            if (_echo)
                write("\r\n", 2);
            execute_line();
            break;

        case ReadlineResult::Backspace:
            if (_echo)
            {
                write(VT100_LEFT, 3);
                write(VT100_ERASE_LINE_AFTER_CURSOR, 3);
            }

            if (!_rl.line().in_rightpos() && _echo)
            {
                write(_rl.line().rightpart(), _rl.line().rightsize());
                char buf[16];
                int len = vt100_left(buf, _rl.line().rightsize());
                write(buf, len);
            }
            break;

        case ReadlineResult::Delete:
            if (_echo)
                write(VT100_ERASE_LINE_AFTER_CURSOR, 3);

            if (!_rl.line().in_rightpos() && _echo)
            {
                write(_rl.line().rightpart(), _rl.line().rightsize());
                char buf[16];
                int len = vt100_left(buf, _rl.line().rightsize());
                write(buf, len);
            }
            break;

        case ReadlineResult::Right:
            if (_echo)
                write(VT100_RIGHT, 3);
            break;

        case ReadlineResult::Left:
            if (_echo)
                write(VT100_LEFT, 3);
            break;

        case ReadlineResult::UpdateLine:
            // Навигация по истории — перерисовка всей строки
            if (_rl.lastsize() > 0 && _echo)
            {
                char buf[16];
                int len = vt100_left(buf, _rl.lastsize());
                write(buf, len);
                write(VT100_ERASE_LINE_AFTER_CURSOR, 3);
            }

            if (_rl.line().data() && _rl.line().current_size() > 0 && _echo)
                write(_rl.line().data(), _rl.line().current_size());
            break;

        default:
            break;
        }
    }

}