#include "mini_uart.h"

void kernel_proc0(void)
{
//	uart_init();
	uart_send_string("Hello, world - Core-0!\r\n");

	while (1) {
		uart_send(uart_recv());
	}
}
