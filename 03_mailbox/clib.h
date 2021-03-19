#ifndef CLIB_H
#define CLIB_H

void memcpy(void * dest, const void * src, int bytes);
void bzero(void * dest, int bytes); 
void memset(void * dest, uint8_t c, int bytes);
void print_string(char* str);

#endif
