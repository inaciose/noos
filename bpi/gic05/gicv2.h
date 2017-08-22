void gic_v2_init(void);
void gic_irq_enable(int);
void gic_eoi(int);
int gic_getack( void );

//-------------------------------------------------------------------------
//
// ISR code
//

// type definition to a void pointer to a function
// used to register functions to handle interrupts
typedef void (*ISR) (int intno);

void isrs_init( void );
void pic_enable (int n, ISR isr);
int pic_dispatch ( void );

