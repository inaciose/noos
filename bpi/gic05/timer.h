void timer0_init(unsigned int);
int timer0_check(void);
void timer0_clear_irq(void);

void timer1_init(unsigned int);
int timer1_check(void);
void timer1_clear_irq(void);

void timer2_init(unsigned int);
int timer2_check(void);
void timer2_clear_irq(void);

void timer0_pic_enable( void );
void timer1_pic_enable( void );
void timer2_pic_enable( void );
