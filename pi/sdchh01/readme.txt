sdchh01

Its not working... (its derivate on folder sdchb01 is working)

This is the sd card implementation found in a zip file on the following Rasperry Pi bare metal forum post.
https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=94133

It is not a modified version or wrapper, but the original code reassembled merged with the RPiHaribote/haribote malloc and file implementations.

None of the errors talked in the post is corrected in this version. Almost all of the orginal code is preserved.


Code files:
__aeabi_*.*
entry.S
file.c
mailbox.c
malloc.c
mylib.c
sdcard.c
uart.c

Header files:
file.h
mailbox.h
malloc.h
mmio.h
mylib.h
sdcard.h

Highlight files:
file.c
sdcard.c

file.c functions
void init_filesystem(struct MEMMAN *memman);
void file_readfat(struct MEMMAN *memman, uint32_t *fat, long long img);
void file_loadfile(int clustno, int size, char *buf, uint32_t *fat, long long img);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);

sdcard.c functions
int mbxGetClockRate(int id);
int sdInit();
static int sdDebugResponse( int resp );
static int sdWaitForInterrupt( unsigned int mask );
static int sdWaitForCommand();
static int sdWaitForData();
static int sdSendCommandP( EMMCCommand* cmd, int arg );
static int sdSendAppCommand();
static int sdSendCommand( int index );
static int sdSendCommandA( int index, int arg );
static int sdReadSCR();
int fls_long (unsigned long x);
unsigned long roundup_pow_of_two (unsigned long x);
static uint32_t sdGetClockDivider ( uint32_t freq );
static int sdSetClock( int freq );
static int sdResetCard( int resetType );
static int sdAppSendOpCond( int arg );
static int sdSwitchVoltage();
int sdTransferBlocks( long long address, int numBlocks, unsigned char* buffer, int write );
int sdClearBlocks( long long address, int numBlocks );
static void sdInitGPIO();
int sdGetBaseClock();
int sdInitCard();
static void sdParseCID();
static void sdParseCSD();
