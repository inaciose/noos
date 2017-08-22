void gic_v2_init(void);

//-------------------------------------------------------------------------
//
// ISR code
//

// type definition to a void pointer to a function
// used to register functions to handle interrupts
typedef void (*ISR) (int intno);

void gic_enable_irq (int n, ISR isr);
int gic_dispatch ( void );

