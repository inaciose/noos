printf01

Sample of printf implementation found on rpi-boot.
The linker needs (included files): __aeabi_uidiv and  __aeabi_uidivmod.

Without a file system it works over a simple uart_putc(int c) function, using a wrapper function putc(int c, void *stream)

If the device have enough memory as the Raspberry pi its a good choice to use for numeric output.
