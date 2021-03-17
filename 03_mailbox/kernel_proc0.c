#include "mini_uart.h"
#include "mailbox.h"
#include "os_types.h"
void turn_on_led(void);
void get_framebuffer();
void kernel_proc0(void)
{
	uart_init();
	uart_send_string("Hello, world - Core-0!\r\n");
	get_framebuffer();
	uart_send_string("Got framebuffer\n");
	turn_on_led();
	uart_send_string("Turned on led\n");
	/*while (1) {
		uart_send(uart_recv());
	}*/
}

void get_framebuffer()
{
	mailbox_messages[0] = 35 * 4;
	mailbox_messages[1] = MAILBOX_REQUEST;

	mailbox_messages[2] = 0x48003; //set phy wh
	mailbox_messages[3] = 8;
	mailbox_messages[4] = 8;
	mailbox_messages[5] = 1024; //FrameBufferInfo.width
	mailbox_messages[6] = 768;	//FrameBufferInfo.height

	mailbox_messages[7] = 0x48004; //set virt wh
	mailbox_messages[8] = 8;
	mailbox_messages[9] = 8;
	mailbox_messages[10] = 1024; //FrameBufferInfo.virtual_width
	mailbox_messages[11] = 768;	 //FrameBufferInfo.virtual_height

	mailbox_messages[12] = 0x48009; //set virt offset
	mailbox_messages[13] = 8;
	mailbox_messages[14] = 8;
	mailbox_messages[15] = 0; //FrameBufferInfo.x_offset
	mailbox_messages[16] = 0; //FrameBufferInfo.y.offset

	mailbox_messages[17] = 0x48005; //set depth
	mailbox_messages[18] = 4;
	mailbox_messages[19] = 4;
	mailbox_messages[20] = 32;		//FrameBufferInfo.depth
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
void turn_on_led()
{
	mailbox_messages[0] = 32;
	mailbox_messages[1] = MAILBOX_REQUEST;
	mailbox_messages[2] = 0x00038041;
	mailbox_messages[3] = 8;
	mailbox_messages[4] = 0;
	mailbox_messages[5] = 130;
	mailbox_messages[6] = 1;
	mailbox_messages[7] = 0;
	uint32_t message = (((uint32_t)((unsigned long)&mailbox_messages) & ~0xF));
	mailbox_send(message, MB_PROPERTY_TAG_CH);
}
