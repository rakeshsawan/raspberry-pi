#include "mini_uart.h"

void log_error(char *message)
{
	uart_send_string("ERROR:");
	uart_send_string(message);
}
