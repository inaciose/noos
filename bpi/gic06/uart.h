void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc();

//-------------------------------------------------------------------------
//
// ISR code
//
void uart_gic_enable( void );

//-------------------------------------------------------------------------

void hexstrings ( unsigned int );
void hexstring ( unsigned int );