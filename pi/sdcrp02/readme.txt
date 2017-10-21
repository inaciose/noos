sdcrp02

This is mbr and vfs implementation found on jncronin/rpi-boot.

Second cumulative sample of the sdcrpXX series dedicated to reading and writing 
files on sd card using the code found on rpi-boot.

It is not a modified version or wrapper, but the original John Cronin code
extracted and reassembled to a more or less minimal version of its sdcard access library.

New C files:
block.c
libfs.c
mbr.c
output.c
vfs.c

New H files:
config.h
console.h
log.h

