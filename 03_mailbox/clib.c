#include <os_types.h>
#include "clib.h"
#include "framebuffer.h"
void memcpy(void * dest, const void * src, int bytes) {
    char * d = dest;
    const char * s = src;
    while (bytes--) {
        *d++ = *s++;
    }
}
void bzero(void * dest, int bytes) {
    memset(dest, 0, bytes);
}

void memset(void * dest, uint8_t c, int bytes) {
    uint8_t * d = dest;
    while (bytes--) {
        *d++ = c;
    }
}
void print_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		gpu_putc((char)str[i]);
	}
}

/**
 * Display a binary value in hexadecimal
 */
void print_hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        gpu_putc(n);
    }
}

