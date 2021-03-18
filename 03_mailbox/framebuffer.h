#include <os_types.h>
typedef struct framebuffer_type
{
	void *buf;
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t buf_size;
	uint32_t chars_width;
	uint32_t chars_height;
	uint32_t chars_x;
	uint32_t chars_y;
}framebuffer_t;

void get_framebuffer();
