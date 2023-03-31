#include <igris/defs/signal.h>
#include <igris/defs/vt100.h>
#include <igris/shell/vtermxx.h>

void igris::vtermxx::init(char *buffer,
                          unsigned int buffer_size,
                          char *hbuffer,
                          unsigned int history_size)
{
    state = 0;
    echo = 1;
    prefix_string = "$ ";

    write_callback = NULL;
    execute_callback = NULL;
    signal_callback = NULL;

    readline_init(&rl, buffer, buffer_size);
    readline_history_init(&rl, hbuffer, history_size);
}

void igris::vtermxx::newline()
{
    execute_callback(sline_getline(&rl.line), sline_size(&rl.line));
    state = 1;
}

void igris::vtermxx::newdata(int16_t input_c)
{
    char c = 0;
    int ret;
    int return_flag = 0;

    while (return_flag == 0)
    {
        switch (state)
        {
        case 0:
        case 1:
            readline_newline_reset(&rl);
            if (echo)
            {
                write_callback(prefix_string,
                               (unsigned int)strlen(prefix_string));
            }
            state = 2;
            break;

        case 2:
            if (input_c < 0)
            {
                return_flag = 1;
                break;
            }
            else
            {
                c = (char)input_c;
                input_c = -1;
                state = 3;
            }
            break;

        case 3:
            // CTRL + C
            if (c == 3)
            {
                if (echo)
                    write_callback("^C\r\n", 4);
                state = 1;

                if (signal_callback)
                    signal_callback(SIGINT);
                break;
            }

            ret = readline_putchar(&rl, c);

            switch (ret)
            {
            case READLINE_ECHOCHAR:
            {
                if (echo)
                    write_callback(&c, 1);

                if (!sline_in_rightpos(&rl.line))
                {
                    char buf[16];

                    if (echo)
                    {
                        write_callback(sline_rightpart(&rl.line),
                                       sline_rightsize(&rl.line));

                        ret = vt100_left(buf, sline_rightsize(&rl.line));

                        write_callback(buf, ret);
                    }
                }
            }
            break;

            case READLINE_NEWLINE:
                state = 1;
                if (echo)
                    write_callback("\r\n", 2);
                newline();
                return_flag = 1;
                goto __recycle__;

            case READLINE_BACKSPACE:
            {
                if (echo)
                {
                    write_callback(VT100_LEFT, 3);

                    write_callback(VT100_ERASE_LINE_AFTER_CURSOR, 3);
                }
                if (!sline_in_rightpos(&rl.line))
                {
                    char buf[16];

                    if (echo)
                    {
                        write_callback(sline_rightpart(&rl.line),
                                       sline_rightsize(&rl.line));

                        ret = vt100_left(buf, sline_rightsize(&rl.line));

                        write_callback(buf, ret);
                    }
                }

                break;
            }

            case READLINE_RIGHT:
                if (echo)
                    write_callback(VT100_RIGHT, 3);
                break;

            case READLINE_LEFT:
                if (echo)
                    write_callback(VT100_LEFT, 3);
                break;

            case READLINE_NOTHING:
                break;

            case READLINE_UPDATELINE:
            {
                char buf[16];

                if (rl.lastsize)
                {
                    if (echo)
                    {
                        ret = vt100_left(buf, rl.lastsize);

                        write_callback(buf, ret);

                        write_callback(VT100_ERASE_LINE_AFTER_CURSOR, 3);
                    }
                }

                if (rl.line.len)
                    if (echo)
                        write_callback(rl.line.buf, rl.line.len);

                break;
            }

            case READLINE_DELETE:
            {
                if (echo)
                {
                    write_callback(VT100_ERASE_LINE_AFTER_CURSOR, 3);
                }
                if (!sline_in_rightpos(&rl.line))
                {
                    char buf[16];

                    if (echo)
                    {
                        write_callback(sline_rightpart(&rl.line),
                                       sline_rightsize(&rl.line));

                        ret = vt100_left(buf, sline_rightsize(&rl.line));

                        write_callback(buf, ret);
                    }
                }

                break;
            }

            default:
                // readline вернул странный код
                // делаем вид, что всё впорядке.
                ;
            }

            state = 2;
            break;

        default:
            // автомат находится в непредусмотренном состоянии
            // сбрасываем состояние и завершаем итерацию.
            state = 0;
            return;
        }

    __recycle__:;
    }
}
