arm32 is a replica of https://adamransom.github.io/posts/raspberry-pi-bare-metal-part-1.html
arm64 is modified version of arm32 code, primarly changed the registers from r0,r1 to x1,x2 and updated kernel.ld to load
the kernel at the appropriate memory location.

I chose blinking an LED as my first program on Raspberry Pi as it's an easiest way to see if your program is working.
However, it turned out to be harder than what I thought, the reason being the OK LED is not part of the GPIO instead connected
on the expansion board and only accessible via VideoCore CPU. We need to use mailbox to communciate with VideoCore to turn on
the led.

If you want to understand more about how it works, refer to https://adamransom.github.io/posts/raspberry-pi-bare-metal-part-1.html

