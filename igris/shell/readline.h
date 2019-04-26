#ifndef IGRIS_SHELL_READLINE_H
#define IGRIS_SHELL_READLINE_H

#include <igris/datastruct/sline.h>
#include <igris/util/bug.h>
#include <igris/defs/ascii.h>

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

struct readline
{
	struct sline line;
	int state;

	char last;
	char * history_space;
};

__BEGIN_DECLS

static inline
void readline_init(struct readline * rl, char * buf, size_t len)
{
	sline_init(&rl->line, buf, len);

	rl->last = 0;
	rl->state = 0;
	rl->history_space = NULL;
}

static inline
void readline_history_init(struct readline * rl, char * hs)
{
	rl->history_space = hs;
}

static inline
void readline_newline_do(struct readline * rl)
{
	sline_reset(&rl->line);
}

static inline
int readline_putchar(struct readline * rl, char c)
{
	int ret;
	int retcode;

	switch (rl->state)
	{
		case READLINE_STATE_NORMAL:
			switch (c)
			{
				case '\r':
				case '\n':
					if ((rl->last == '\n' || rl->last == '\r') && rl->last != c)
						retcode = READLINE_NOTHING;
					else
						retcode = READLINE_NEWLINE;
					break;

				case ASCII_BS:
					ret = sline_backspace(&rl->line, 1);
					retcode = ret ? READLINE_BACKSPACE
					          : READLINE_NOTHING;
					break;

				case ASCII_ESC:
					rl->state = READLINE_STATE_ESCSEQ;
					retcode = READLINE_NOTHING;
					break;

				default:
					sline_putchar(&rl->line, c);
					retcode = READLINE_ECHOCHAR;
					break;
			}
			break;

		case READLINE_STATE_ESCSEQ:
			switch (c)
			{
				case 0x5B:
					rl->state = READLINE_STATE_ESCSEQ_MOVE;
					break;

				default:
					dprln("?warning in readline?");
					rl->state = READLINE_STATE_NORMAL;
					break;
			}
			retcode = READLINE_NOTHING;
			break;
		case READLINE_STATE_ESCSEQ_MOVE:
			retcode = READLINE_NOTHING;
			switch (c)
			{
				case 0x41: //up
				case 0x42: //down
					BUG();
				case 0x43: //right
					ret = sline_right(&rl->line);
					if (ret)
						retcode = READLINE_RIGHT;
					break;
				case 0x44: //left
					ret = sline_left(&rl->line);
					if (ret)
						retcode = READLINE_LEFT;
					break;
			}
			rl->state = READLINE_STATE_NORMAL;
			break;
		default:
			BUG();
	}

	rl->last = c;
	return retcode;
}

__END_DECLS

#endif