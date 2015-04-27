# coos
Extremely simple and highly portable cooperative operating system in pure C. Suitable for any microcontroller. Small footprint. Based on standard C functions setjmp and longjmp, no assembler code used.

Operating system and demo tasks occupy only 200 lines of text. Demo project compiled for PIC24FJ64GA002 took 669 bytes of program memory and 270 bytes of RAM. 

The only system service is DELAY(). It returns control from task to kernel for specified number of system ticks. For example, if system tick is 100 us, then DELAY(10) pass control to kernel for 1 ms. After 1 ms task will continue from the instruction next to DELAY().  

DELAY() must be called only from tasks; it must not be called from functions. If DELAY() is called from function then system will crash.
