void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc();
void uart_irq_rxtx_config( unsigned int mode);