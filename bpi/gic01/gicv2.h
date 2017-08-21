void gic_v2_init(void);
void gic_irq_enable(int irqno);
void gic_eoi(int);
int gic_getack( void );