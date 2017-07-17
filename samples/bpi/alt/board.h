#ifndef u32
  typedef unsigned int u32;
#endif

void soc_detection_init(void);
int soc_detection_show(void);
int get_boot_device(void);

void gpio_init(void);

void uart0_init(void);
void uart0_putc(char c);
void uart0_puts(const char *s);
char uart0_getc();
//void uart_fifo_flush ( void )

enum { BOOT_DEVICE_UNK, BOOT_DEVICE_FEL, BOOT_DEVICE_MMC0, BOOT_DEVICE_SPI };

void ledconf (void);
void ledon (void);
void ledoff (void);