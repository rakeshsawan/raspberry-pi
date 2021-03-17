.section  ".text.boot"

.global	_start

_start:
	mrs x0,mpidr_el1 //Load the current processor id
	and x0,x0,#3 //Mask the bit belonging to core id
	cbz x0,master
	sub x1,x0,#1 //Check for core id 1 and jump to label 1
	cbz x1,1f 
	sub x1,x0,#2 //Check for core id 2 and jump to label 2
	cbz x1,2f 
	b hang

master:
	bl bss_init
	bl  memzero
	mov sp, #0x8000
	bl kernel_proc0
	b hang

1:
	bl bss_init
	bl  memzero
	mov sp, #0x8000
	bl kernel_proc0
	b hang

2: 
	
	b hang
	add x1, x1, #0x9000
	mov sp, x1
	bl kernel_proc0
	b hang

hang:
	b hang


bss_init:
	adr x0,bss_begin
	adr x1,bss_end
	sub x1,x1,x0
	ret

