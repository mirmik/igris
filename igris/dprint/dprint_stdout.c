#include <stdio.h>
#include <unistd.h>

#define GXX_DEBUG_STDOUT STDOUT_FILENO

void debug_putchar(char c) {
    int _ = write(GXX_DEBUG_STDOUT, &c, 1);
	(void) _;
}

void debug_write(const char* c, int i) {
    int _ = write(GXX_DEBUG_STDOUT, c, i);
	(void) _;
}
	
	
	
