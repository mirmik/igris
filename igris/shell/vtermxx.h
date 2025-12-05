// VT100-совместимый обработчик терминала для встраиваемых систем.
// Реализует обработку ввода как машину состояний поверх igris::readline.

#ifndef IGRIS_SHELL_VTERM_H
#define IGRIS_SHELL_VTERM_H

#include <igris/compiler.h>
#include <igris/event/delegate.h>
#include <igris/shell/readlinexx.h>

namespace igris
{
    class vtermxx
    {
    public:
        using write_callback_t =
            igris::delegate<void, const char *, unsigned int>;
        using execute_callback_t =
            igris::delegate<void, const char *, unsigned int>;
        using signal_callback_t = igris::delegate<void, int>;

        static constexpr int16_t INIT_STEP = -1;

    private:
        enum class State : int
        {
            ShowPrompt = 0,
            AfterNewline = 1,
            WaitInput = 2,
            ProcessChar = 3
        };

        execute_callback_t _execute_callback = {};
        write_callback_t _write_callback = {};
        signal_callback_t _signal_callback = {};

        State _state = State::ShowPrompt;
        bool _echo = true;
        const char *_prompt = "$ ";
        readline _rl = {};

    public:
        vtermxx() = default;

        vtermxx(unsigned int buffer_size, unsigned int history_size)
        {
            init(buffer_size, history_size);
        }

        void init(unsigned int buffer_size, unsigned int history_size);
        void newdata(int16_t c);

        void start()
        {
            newdata(INIT_STEP);
        }

        void set_prompt(const char *str)
        {
            _prompt = str;
        }
        const char *prompt() const
        {
            return _prompt;
        }

        void set_echo(bool enabled)
        {
            _echo = enabled;
        }
        bool echo() const
        {
            return _echo;
        }

        void set_write_callback(write_callback_t cb)
        {
            _write_callback = cb;
        }
        void set_execute_callback(execute_callback_t cb)
        {
            _execute_callback = cb;
        }
        void set_signal_callback(signal_callback_t cb)
        {
            _signal_callback = cb;
        }

        [[deprecated("Использовать start()")]] void init_step()
        {
            start();
        }

    private:
        void execute_line();
        void write(const char *data, unsigned int len);
        void write_str(const char *str);
        void handle_readline_result(ReadlineResult result, char c);
    };

}

#endif