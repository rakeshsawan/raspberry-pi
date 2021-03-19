#include "mini_uart.h"
#include "mailbox.h"
#include "framebuffer.h"
#include "os_types.h"
void gpu_putc(char c);

void turn_on_led(void);
void gpu_init();

void kernel_proc0(void)
{
	uart_init();
	uart_send_string("Booting OS - Core-0!\r\n");
	get_framebuffer();
//	uart_send_string("Got framebuffer\n");
	turn_on_led();
	//uart_send_string("Turned on led\n");
	gpu_init();
	print_string("\nBooting OS\n");
	print_string("\nFramebuffer address: ");
	print_hex(mailbox_messages[28]);
	/*while (1) {
		uart_send(uart_recv());
	}*/
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
