.global uart_init
uart_init1:

   stp	x29, x30, [sp, #-32]!
   mov	x29, sp
   ldr w0,=0x3f200015

   mov	x0, #0x4                   	// #4
   movk	x0, #0x3f20, lsl #16
   ldr  w0,[x0] //get32
   and	w0, w0, #0xffff8fff
   orr	w0, w0, #0x2000
   and	w0, w0, #0xfffc7fff
   orr	w0, w0, #0x10000
   mov	x0, #0x4                   	// #4
   movk	x0, #0x3f20, lsl #16
   
   ldr w0,[x0] // get 32
   mov	w1, #0x0                   	// #0
   mov	x0, #0x94                  	// #148
   movk	x0, #0x3f20, lsl #16
   bl	put32
   mov	x0, #0x96                  	// #150
   bl	delay
   mov	w1, #0xc000                	// #49152
   mov	x0, #0x98                  	// #152
   movk	x0, #0x3f20, lsl #16
   bl	put32
   mov	x0, #0x96                  	// #150
   bl	delay
   mov	w1, #0x0                   	// #0
   mov	x0, #0x98                  	// #152
   movk	x0, #0x3f20, lsl #16
   bl	put32
   mov	w1, #0x1                   	// #1
   mov	x0, #0x5004                	// #20484
   movk	x0, #0x3f21, lsl #16
   bl	put32
   mov	w1, #0x0                   	// #0
   mov	x0, #0x5060                	// #20576
   movk	x0, #0x3f21, lsl #16
   bl	put32
   mov	w1, #0x0                   	// #0
   mov	x0, #0x5044                	// #20548
   movk	x0, #0x3f21, lsl #16
   bl	put32
   mov	w1, #0x3                   	// #3
   mov	x0, #0x504c                	// #20556
   movk	x0, #0x3f21, lsl #16
   bl	put32
   mov	w1, #0x0                   	// #0
   mov	x0, #0x5050                	// #20560
   movk	x0, #0x3f21, lsl #16
   bl	put32
   mov	w1, #0x10e                 	// #270
   mov	x0, #0x5068                	// #20584
   movk	x0, #0x3f21, lsl #16
   bl	put32
   mov	w1, #0x3                   	// #3
   mov	x0, #0x5060                	// #20576
   movk	x0, #0x3f21, lsl #16
   bl	put32
   nop
   ldp	x29, x30, [sp], #32
   ret

.global uart_send
uart_send1:
   mov x2, x30 //Save link register
   mov	w1, w0

wait_for_read_1:
   ldr w0,#AUX_MU_LSR_REG
   ldr w0,[x0]
   and	w0, w0, #0x20
   cmp	w0, #0x0
   b.eq	wait_for_read_1

   ldr w0,#AUX_MU_IO_REG
   str w1,[x0]
   mov x30, x2 //Restore link register
   ret

.global uart_recv1
uart_recv1:
   mov x2, x30 //Save link register
   mov	w1, w0

wait_for_read_2:
   ldr w0,#AUX_MU_LSR_REG
   ldr w0,[x0]
   and	w0, w0, #0x01
   cmp	w0, #0x0
   b.eq	wait_for_read_2

   ldr w0,#AUX_MU_IO_REG
   ldr w0,[x0]
   and	w0, w0, #0xff
   mov x30, x2 //Restore link register
   ret

.data
AUX_MU_LSR_REG:
  .word 0x3f215054

AUX_MU_IO_REG:
  .word 0x3f215040  
