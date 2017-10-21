#include "config.h"
#include "vfs.h"

// uart.c
void _puts(const char *);
void uart_init(void);

// output.c
void output_enable_uart();
void output_init();
int split_putc(int c);

// stream.c
int def_stream_putc(int, FILE *);

// malloc.c
void init_heap(unsigned int, unsigned int);
void print_heap_range( void );

// libfs.c
void vfs_init( void );

// vfs.c
void libfs_init( void );

// needed in mmio (dont know here it is init)
const unsigned int base_adjust = 0;

// mallooc heap start
extern unsigned int _end;
unsigned int *end = &_end;

// stdio generic
extern int (*stdout_putc)(int);
extern int (*stderr_putc)(int);
extern int (*stream_putc)(int, FILE*);

int cmain(void) {

  // First use the serial console
  stdout_putc = split_putc;
  stderr_putc = split_putc;
  stream_putc = def_stream_putc;

  output_init();
  output_enable_uart();  
  
  uart_init();
  _puts("... hello\n");
  _puts("start sdcrp04\n");

  init_heap((unsigned int)end, 0x8000000);
  print_heap_range();  

  vfs_init(); // not needed if -02 flag is removed from gcc
  libfs_init();
  
  _puts("libfs_init done.\n");

  // list devices
  _puts("MAIN: device list: ");
  vfs_list_devices();
  _puts("end\n");

  // open file
  FILE *f = (void*)0;

  // fname on default device
  char *fname = "/config.txt";
  
  _puts("Try open the file: ");
  _puts(fname);
  _puts("\n");
  
  f = fopen(fname, "r");
  if(f) {
     _puts(fname);
     _puts(" open.\n");
  } else {
     _puts(fname);
     _puts(" not open.\n");
  }
    
  _puts("fopen done.\n");

  // display file contents
  char buf[512];
  int n;
  
  while((n = fread(buf, 1, sizeof(buf), f)) > 0) {
    fwrite(buf, 1, sizeof(buf), stdout);
  }
  if(n < 0){
    printf("\ncat: read error\n");
  }
  _puts("\nfile contents display done.\n");
  
  fclose(f);
  _puts("fclose done.\n");

  _puts("end\n");
  
  while(1) {}
  return -1;
}
