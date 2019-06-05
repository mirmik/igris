#ifndef IGRIS_SHELL_READLINE_H
#define IGRIS_SHELL_READLINE_H

#include <igris/datastruct/sline.h>
#include <igris/util/bug.h>
#include <igris/defs/ascii.h>

#include <igris/dtrace.h>

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
	int lastsize;

	char * history_space;
	uint8_t history_size;

	uint8_t headhist;
	uint8_t curhist;
};

__BEGIN_DECLS

static inline
void readline_init(struct readline * rl, char * buf, size_t len)
{
	sline_init(&rl->line, buf, len);

	rl->last = 0;
	rl->state = 0;
	rl->history_space = NULL;
	rl->curhist = 0;
	rl->headhist = 0;
}

static inline
void readline_history_init(struct readline * rl, char * hs, int hsize)
{
	rl->history_space = hs;
	rl->history_size = hsize;
	memset(hs, 0, rl->line.cap * hsize);
}

static inline
void readline_newline_reset(struct readline * rl)
{
	sline_reset(&rl->line);
	rl->curhist = 0;
}

static inline
char* readline_current_history_pointer(struct readline * rl)
{
	int idx;

	idx = (rl->headhist + rl->history_size - rl->curhist) % rl->history_size;
	return rl->history_space + idx * rl->line.cap;
}

static inline
char* readline_update_history_pointer(struct readline * rl)
{
	int idx;

	idx = rl->headhist;
	return rl->history_space + idx * rl->line.cap;
}

static inline
void readline_load_history_line(struct readline * rl)
{
	rl -> lastsize = rl->line.len;

	if (rl->curhist == 0)
	{
		memset(rl->line.buf, 0, rl->line.cap);
		rl->line.len = 0;
		rl->line.cursor = 0;
		return;
	}

	unsigned int sz = strlen(readline_current_history_pointer(rl));

	memcpy(rl->line.buf, readline_current_history_pointer(rl), sz);
	rl->line.len = rl->line.cursor = sz;
}

static inline
int readline_history_up(struct readline * rl)
{
	//DTRACE();
	if (rl->history_space == NULL)
		return 0;

	if (rl->curhist == rl->history_size)
		return 0;

	rl->curhist++;

	readline_load_history_line(rl);
	return 1;
}

static inline
int readline_history_down(struct readline * rl)
{
	//DTRACE();
	//DPRINT(rl->curhist);
	if (rl->history_space == NULL)
		return 0;

	if (rl->curhist == 0)
		return 0;

	rl->curhist--;

	readline_load_history_line(rl);
	return 1;
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
					{
						rl->last = 0;
						retcode = READLINE_NOTHING;
					}
					else
					{
						if (rl->history_space && rl->line.len)
						{
							char* ptr = readline_update_history_pointer(rl);
							memset(ptr, 0, rl->line.cap);
							memcpy(ptr, rl->line.buf, rl->line.len);
							*(ptr + rl->line.len) = '\0';
							rl->headhist = (rl->headhist + 1) % rl->history_size;
							rl->curhist = 0;
						}

						retcode = READLINE_NEWLINE;
					}
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
					if (readline_history_up(rl))
						retcode = READLINE_UPDATELINE;
					break;
				case 0x42: //down
					if (readline_history_down(rl))
						retcode = READLINE_UPDATELINE;
					break;
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

static inline 
int readline_linecpy(struct readline * rl, char* line, size_t maxlen) 
{
	int len = 
		(int)maxlen - 1 > (int)rl->line.len ? rl->line.len : maxlen - 1;
	
	memcpy(line, rl->line.buf, len);
	line[len] = 0;

	return len;
}

__END_DECLS

#endif