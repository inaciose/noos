malloc03

This is the dlmalloc implementation I found on jncronin/rpi-boot.

Be aware that the start of the heap address must not be aligned on a page size
On rpi:

0x17000 dont work
0x18000 work

