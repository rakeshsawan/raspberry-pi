#include "mailbox.h"
#include "log.h"
#include "os_types.h"

//Ensure the array is 16 bit aligned.
//volatile uint32_t __attribute__((aligned(16))) mailbox_messages[MAILBOX_MESSAGE_MAX];
volatile uint32_t  mailbox_messages[MAILBOX_MESSAGE_MAX];
mbox_t *mailbox= ((mbox_t *) (MAILBOX_ADDRESS));

//The mailbox message should have last 4 bit contain the channel number
//and the remaining 28 bits should have the buffer address.
uint32_t create_mailbox_message(uint32_t message, uchar_t channel)
{
	//check to ensure last 4 bits are zero,
	uint32_t channel_byte = message & 0xF;
	if(channel_byte != 0)
	{
		return -1;		
	} 
	//Set channel number in last 4 bits
	message = message | (channel & 0xF);

	return message;
}
short mailbox_send(uint32_t  message, uchar_t channel)
{
	message = create_mailbox_message(message, channel);

	while(mailbox->status == MAIL_FULL);
	mailbox->write = message;
	return 0;
}
/**
 * This method reads the message from mailbox
 */
uint32_t mailbox_read(uchar_t channel)
{
	uint32_t message;
	uchar_t  channel_read;
	do
	{
		//Wait untill mailbox is not empty.
		while(mailbox->status == MAIL_EMPTY);

		message = mailbox->read;

		//Extract channel number
		channel_read = message & 0XF;
	}while(channel_read!= channel);

	return message;
}

