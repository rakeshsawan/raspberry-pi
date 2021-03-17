
.section .data
.align 4 // This ensures lowest 4 bits are 0 for the following label
PropertyInfo:
  // = Message Header =
  .int PropertyInfoEnd - PropertyInfo // Calculate buffer size
  .int 0 // Request code: Process Request
  // = Tag Header =
  .int 0x00038041 // Tag ID (SET_GPIO_STATE)
  .int 8 // Value buffer size
  .int 0 // Request/response size
  // = Tag Value Buffer =
  .int 130 // ACT_LED pin number
  .int 1 // Turn it on
  .int 0 // End tag
PropertyInfoEnd:
.align 4 // This ensures lowest 4 bits are 0 for the following label
framebuffer_prop_buffer:
  // = Message Header =
  .int framebuffer_prop_end - framebuffer_prop_buffer// Calculate buffer size
  .int 0 // Request code: Process Request
  // = Tag Header =
  .int 0x48003 // Tag ID (Set physical width_
  .int 8 // Value buffer size
  .int 8 // Request/response size
  // = Tag Value Buffer =
  .int 1024 // Width
  .int 768 // height
  .int 0x48004 // Tag ID (Set virtual width)
  .int 8 // Value buffer size
  .int 8 // Request/response size
  // = Tag Value Buffer =
  .int 1024 // Width
  .int 768 // Height
  .int 0x48009 // Tag ID (Set virtual offset)
  .int 8 // Value buffer size
  .int 8 // Request/response size
  // = Tag Value Buffer =
  .int 0 // Width
  .int 0 // Height
  .int 0x48005 // Tag ID (Set depth)
  .int 4 // Value buffer size
  .int 4 // Request/response size
  // = Tag Value Buffer =
  .int 32 // Width
  .int 0x48006 // Tag ID (Set pixel order)
  .int 4 // Value buffer size
  .int 4 // Request/response size
  // = Tag Value Buffer =
  .int 1 // RGB
  .int 0x40001 // Tag ID (get frame buffer)
  .int 8 // Value buffer size
  .int 8 // Request/response size
  // = Tag Value Buffer =
  .int 4096// Width
  .int 0// Height
  .int 0x40008 // Tag ID (get pitch)
  .int 4 // Value buffer size
  .int 4 // Request/response size
  // = Tag Value Buffer =
  .int 0 // Width
  .int 0 // End tag
framebuffer_prop_end:

.section .text
.global _turn_on_led
.global _get_framebuffer

_turn_on_led:
  mov x4,x30; //save link register
  ldr x3, =PropertyInfo // Load x2 with address of our message buffer
  bl _mailbox_send
  mov x30,x4; //restore link register
  ret
/*
//  mailbox .req x1 // Alias mailbox to x1
  ldr x1, =0x3f00b880 // Load the mailbox's base address into x1

  wait1$:
    status .req x2 // Alias status to x2
    //ldr status, [mailbox, #0x38] // Load the Mailbox 1 status address
    ldr status, [x1, #0x38] // Load the Mailbox 1 status address
    tst status, #0x80000000 // Check the status against the FULL bit
    .unreq status // Unset the alias
    bne wait1$ // Keep checking the mailbox until it isn't full

  message .req x2 // Alias message to x2
  ldr message, =PropertyInfo // Load x2 with address of our message buffer
  add message,message, #8 // Put the channel in the last 4 bits
  //str message, [mailbox, #0x20] // Put the message in the mailbox
  str message, [x1, #0x20] // Put the message in the mailbox
  .unreq message // Unset the alias
  mov x30,x4; //restore link register
  ret*/

_get_framebuffer:
  mov x4,x30; //save link register
  ldr x3, =framebuffer_prop_buffer// Load x2 with address of our message buffer
  bl _mailbox_send
  mov x30,x4; //restore link register
  ret

_mailbox_send:
  mov x5,x30;
  mailbox .req x1 // Alias mailbox to x1
  ldr mailbox, =0x3f00b880 // Load the mailbox's base address into x1

  wait1$:
    status .req x2 // Alias status to x2
    ldr status, [mailbox, #0x38] // Load the Mailbox 1 status address
    tst status, #0x80000000 // Check the status against the FULL bit
    .unreq status // Unset the alias
    bne wait1$ // Keep checking the mailbox until it isn't full

  message .req x3 // Alias message to x2
  add message,message, #8 // Put the channel in the last 4 bits
  str message, [mailbox, #0x20] // Put the message in the mailbox
  .unreq message // Unset the alias
  mov x30,x5;
  ret

