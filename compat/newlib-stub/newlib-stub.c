#include <igris/dprint.h>

int _write()
{
	dprln("newlib:_write");
	return -1;
}

int _read()
{
	dprln("newlib:_read");
	return -1;
}

int _close()
{
	dprln("newlib:_close");
	return -1;
}

int _fstat()
{
	dprln("newlib:_fstat");
	return -1;
}

int _isatty()
{
	dprln("newlib:_isatty");
	return -1;
}

int _lseek()
{
	dprln("newlib:_lseek");
	return -1;
}

int _getpid()
{
	dprln("newlib:_getpid");
	return -1;
}

int _kill()
{
	dprln("newlib:_kill");
	return -1;
}

int _exit()
{
	dprln("newlib:_exit");
	return -1;
}

void *_sbrk(int incr) {
	dprln("newlib:_sbrk");
	return NULL;
}

