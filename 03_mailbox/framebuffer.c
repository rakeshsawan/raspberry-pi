#include "mailbox.h"
#include "framebuffer.h"
#include "os_types.h"
#include "bitmap.h"
#include "clib.h"
#define COLORDEPTH 24
#define BYTES_PER_PIXEL COLORDEPTH/8
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8 
framebuffer_t init_framebuffer();
void framebuffer_mailbox_send();
framebuffer_t framebuffer;
typedef struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

void write_pixel(uint32_t x, uint32_t y, const pixel_t * pix) {
    uint8_t * location = framebuffer.buf + y*framebuffer.pitch + x*BYTES_PER_PIXEL;
    memcpy(location, pix, BYTES_PER_PIXEL);
}

void gpu_putc(char c) {
    static const pixel_t WHITE = {0xff, 0xff, 0xff};
    static const pixel_t BLACK = {0x00, 0x00, 0x00};
    uint8_t w,h;
    uint8_t mask;
    const uint8_t * bmp = font(c);
    uint32_t i, num_rows = framebuffer.height/CHAR_HEIGHT;

    // shift everything up one row
    if (framebuffer.chars_y >= num_rows) {
        // Copy a whole character row into the one above it
        for (i = 0; i < num_rows-1; i++)
            memcpy(framebuffer.buf + framebuffer.pitch*i*CHAR_HEIGHT, framebuffer.buf + framebuffer.pitch*(i+1)*CHAR_HEIGHT, framebuffer.pitch * CHAR_HEIGHT);
        // zero out the last row
        bzero(framebuffer.buf + framebuffer.pitch*i*CHAR_HEIGHT,framebuffer.pitch * CHAR_HEIGHT);
        framebuffer.chars_y--;
    }

    if (c == '\n') {
        framebuffer.chars_x = 0;
        framebuffer.chars_y++;
        return;
    }

    for(w = 0; w < CHAR_WIDTH; w++) {
        for(h = 0; h < CHAR_HEIGHT; h++) {
            mask = 1 << (w);
            if (bmp[h] & mask)
                write_pixel(framebuffer.chars_x*CHAR_WIDTH + w, framebuffer.chars_y*CHAR_HEIGHT + h, &WHITE);
            else
                write_pixel(framebuffer.chars_x*CHAR_WIDTH + w, framebuffer.chars_y*CHAR_HEIGHT + h, &BLACK);
        }
    }

    framebuffer.chars_x++;
    if (framebuffer.chars_x > framebuffer.chars_width) {
        framebuffer.chars_x = 0;
        framebuffer.chars_y++;
    }
}
void get_framebuffer()
{
	framebuffer_mailbox_send();
	framebuffer = init_framebuffer();
}


framebuffer_t init_framebuffer()
{
	framebuffer_t framebuffer;
	uint32_t address = mailbox_messages[28]&0x3FFFFFFF;
	//framebuffer.buf = (void*)(unsigned long)mailbox_messages[28];		
	framebuffer.buf = (void*)(unsigned long)address;
	framebuffer.width =  mailbox_messages[5];
	framebuffer.height = mailbox_messages[6];
	//framebuffer.pitch = mailbox_messages[33];
	framebuffer.pitch = framebuffer.width * BYTES_PER_PIXEL;
	framebuffer.buf_size=mailbox_messages[29];


	framebuffer.chars_width =  framebuffer.width/CHAR_WIDTH;
	framebuffer.chars_height = framebuffer.height/CHAR_WIDTH;
	framebuffer.chars_x = 0;
	framebuffer.chars_y = 0;
	return framebuffer;
}
void framebuffer_mailbox_send()
{
	mailbox_messages[0] = 35 * 4;
	mailbox_messages[1] = MAILBOX_REQUEST;

	mailbox_messages[2] = 0x48003; //set phy wh
	mailbox_messages[3] = 8;
	mailbox_messages[4] = 8;
	mailbox_messages[5] = 640;//1024; //FrameBufferInfo.width
	mailbox_messages[6] = 480;//768;	//FrameBufferInfo.height

	mailbox_messages[7] = 0x48004; //set virt wh
	mailbox_messages[8] = 8;
	mailbox_messages[9] = 8;
	mailbox_messages[10] = 640;//1024; //FrameBufferInfo.virtual_width
	mailbox_messages[11] = 480;//768;	 //FrameBufferInfo.virtual_height

	mailbox_messages[12] = 0x48009; //set virt offset
	mailbox_messages[13] = 8;
	mailbox_messages[14] = 8;
	mailbox_messages[15] = 0; //FrameBufferInfo.x_offset
	mailbox_messages[16] = 0; //FrameBufferInfo.y.offset

	mailbox_messages[17] = 0x48005; //set depth
	mailbox_messages[18] = 4;
	mailbox_messages[19] = 4;
	mailbox_messages[20] = 24;//32;		//FrameBufferInfo.depth
	mailbox_messages[21] = 0x48006; //set pixel order
	mailbox_messages[22] = 4;
	mailbox_messages[23] = 4;
	mailbox_messages[24] = 1; //RGB, not BGR preferably

	mailbox_messages[25] = 0x40001; //get framebuffer, gets alignment on request
	mailbox_messages[26] = 8;
	mailbox_messages[27] = 8;
	mailbox_messages[28] = 4096; //FrameBufferInfo.pointer
	mailbox_messages[29] = 0;	 //FrameBufferInfo.size

	mailbox_messages[30] = 0x40008; //get pitch
	mailbox_messages[31] = 4;
	mailbox_messages[32] = 4;
	mailbox_messages[33] = 0; //FrameBufferInfo.pitch

	mailbox_messages[34] = 0;

	uint32_t message = (((uint32_t)((unsigned long)&mailbox_messages) & ~0xF));
	mailbox_send(message, MB_PROPERTY_TAG_CH);
}
void gpu_init(void) {
    static const pixel_t WHITE = {0xff, 0xff, 0xff};
    static const pixel_t BLACK = {0x00, 0x00, 0x00};
    // Aparantly, this sometimes does not work, so try in a loop
    //while(framebuffer_init());

    // clear screen
    for (uint32_t j = 0; j < framebuffer.height; j++) {
        for (uint32_t i = 0; i < framebuffer.width; i++) {
            //write_pixel(i,j,&BLACK);
            write_pixel(i,j,&WHITE);
        }
    }
}
