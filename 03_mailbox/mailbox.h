/*Reference: 
 * https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
 * Mailbox channels : https://github.com/raspberrypi/firmware/wiki/Mailboxes
 * Mailbox registers: https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 */
#ifndef MAILBOX_H
#define MAILBOX_H

#include <peripherals.h>
#include <os_types.h>

//Declare mailbox message of 64 elements, should be sufficient for most cases.
#define MAILBOX_MESSAGE_MAX 64
extern volatile uint32_t mailbox_messages[MAILBOX_MESSAGE_MAX];

#define MAIL_BASE 0xB880	// Base address for the mailbox registers

// This bit is set in the status register if there is no space to write into the mailbox
#define MAIL_FULL 0x80000000
// This bit is set in the status register if there is nothing to read from the mailbox
#define MAIL_EMPTY 0x40000000

#define MAILBOX_ADDRESS   PERIPHERAL_BASE + MAIL_BASE


//Mail box channels
#define MB_PROPERTY_TAG_CH  8

//TAGS
#define MBOX_TAG_GET_SERIAL 0x00010004

/**
 * Mailbox register offsets
 * Mailbox Read/Write Peek  Sender  Status    Config
 * 0       0x00       0x10  0x14    0x18      0x1c
 * 1       0x20       0x30  0x34    0x38      0x3c
 */
typedef struct mbox_type
{
	uint32_t read; //0x00
        uint32_t reserved1;
        uint32_t reserved2;
        uint32_t reserved3;
	uint32_t peek; //0x10
	uint32_t sender; //0x14
	uint32_t status; //0x18
	uint32_t config; //0x1c
	uint32_t write; //0x20
}mbox_t;

extern mbox_t *mailbox;


//Request response code
#define MAILBOX_REQUEST 0
#define MAILBOX_RESPONSE_SUCCESS 0x80000000
#define MAILBOX_RESPONSE_ERROR 0x80000001


uint32_t mailbox_send(uint32_t  message, uchar_t channel);
uint32_t mailbox_read(uchar_t channel);

#endif 
