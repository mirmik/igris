#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <igris/util/printf_impl.h>

struct printchar_handler_data {
	int fd;
};

static void file_printchar(void *d, int c) {
	fdputc(c, ((struct printchar_handler_data *)d)->fd);
}

int vfdprintf(int fd, const char *format, va_list args) {
	struct printchar_handler_data data;

	//assert(file != NULL);
	//assert(format != NULL);

	data.fd = fd;

	return __printf(file_printchar, &data, format, args);
}

int fdprintf(int fd, const char *format, ...) {
	int ret;
	va_list args;

	//assert(file != NULL);
	//assert(format != NULL);

	va_start(args, format);
	ret = vfdprintf(fd, format, args);
	va_end(args);

	return ret;
}
