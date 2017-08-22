#include "macro.h"
#include "timer_io.h"

static inline void delay(unsigned int count)
{
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
     : "=r"(count): [count]"0"(count) : "cc");
}

// TMR_IRQ_EN register description
// bit 0    TMR_IRQ0_EN           rw 0/1 = nop/enable interrupt
// bit 1    TMR_IRQ1_EN           rw 0/1 = nop/enable interrupt
// bit 2    TMR_IRQ2_EN           rw 0/1 = nop/enable interrupt
// (...)
// bit 5    TMR_IRQ5_EN           rw 0/1 = nop/enable interrupt

// TMR_IRQ_STA register description
// bit 0    TMR_IRQ0_PEN           rw 0/1 = nop/(interrupt pending/clear)
// bit 1    TMR_IRQ1_PEN           rw 0/1 = nop/(interrupt pending/clear)
// bit 2    TMR_IRQ2_PEN           rw 0/1 = nop/(interrupt pending/clear)
// (...)
// bit 5    TMR_IRQ5_PEN           rw 0/1 = nop/(interrupt pending/clear)


// TMR_0_CTRL register description
// bit 0    TMR_0_RUN           rw 0/1 = stop/start
// bit 1    TMR_0_RELOAD        rw 0/1 = nop /reload
// bit 2:3  TMR_0_CLK_SRC       rw 0/1/2/3 = 32kHz/24MHz/PLL6 divided by 6/nop
// bit 4:6  TMR_0_CLK_PRESCALE  rw 0/1/(...)/6/7 = div(1/2/4/8/16/32/64/128)
// bit 7    TMR_0_MODE          rw 0/1 = continuous mode / single run

void timer0_init(unsigned int counter)
{
  // stops timer to allow change on his registers
  mmio_write32(TMR_0_CTRL, 0x4); // timer 0 control : stop
  
  delay(150); // need to wait to take effect

  // start value decrementing to zero.. 
  // then set its bit to 1 on IRQ_STA timer register (check and set it to 0)
  // and generate an interrupt if its bit is enabled TMR_IRQ_EN register 
  mmio_write32(TMR_0_INTR_VAL, counter);    // timer 0 interval (start value)
  
  // start timer countdown
  mmio_write32(TMR_0_CTRL, 0x7);  // timer 0 control : start

  // enable interrupt generation
  mmio_write32or(TMR_IRQ_EN, 0x1);  // timer 0
}

int timer0_check(void) {
  // check his status register bit
  if(CHECK_BIT(mmio_read32(TMR_IRQ_STA), 0)) {
    return 1;
  }
  return 0;
}

void timer0_clear_irq(void) {
  // clean his status register bit
  mmio_write32or(TMR_IRQ_STA, 1);
}

//-------------------------------------------------------------------------

void timer1_init(unsigned int counter)
{

  // stops timer to allow change on his registers
  mmio_write32(TMR_1_CTRL, 0x4); // timer 1 control : stop
  
  delay(150); // need to wait to take effect

  // start value decrementing to zero.. 
  // then generate interrupt if its bit is enabled TMR_IRQ_EN register 
  mmio_write32(TMR_1_INTR_VAL, counter);   // timer 1 interval (start value)
  
  // start timer countdown
  mmio_write32(TMR_1_CTRL, 0x7);  // timer 1 control : start

  // enable interrupt generation
  mmio_write32or(TMR_IRQ_EN, 0x2);  // timer 1
  
}

int timer1_check(void) {
  // check his status register bit
  if(CHECK_BIT(mmio_read32(TMR_IRQ_STA), 1)) {
    return 1;
  }
  return 0;
}

void timer1_clear_irq(void) {
  // clean his status register bit
  mmio_write32or(TMR_IRQ_STA, 2);
}

//-------------------------------------------------------------------------

void timer2_init(unsigned int counter)
{

  // stops timer to allow change on his registers
  mmio_write32(TMR_2_CTRL, 0x4); // timer 2 control : stop
  
  delay(150); // need to wait to take effect

  // start value decrementing to zero.. 
  // then generate interrupt if its bit is enabled TMR_IRQ_EN register 
  mmio_write32(TMR_2_INTR_VAL, counter);  // timer 2 interval (start value)
  
  // start timer countdown
  mmio_write32(TMR_2_CTRL, 0x7);  // timer 2 control : start

  // enable interrupt generation
  mmio_write32or(TMR_IRQ_EN, 0x4);  // timer 2
  
}

int timer2_check(void) {
  // check his status register bit
  if(CHECK_BIT(mmio_read32(TMR_IRQ_STA), 2)) {
    return 1;
  }
  return 0;
}

void timer2_clear_irq(void) {
  // clean his status register bit
  mmio_write32or(TMR_IRQ_STA, 4);
}

//-------------------------------------------------------------------------
//
// ISR code
//

#include "gicv2.h"

#include "uart.h"
#include "gpio.h"

// global variable
int ledstate = 0;

void isr_timer0 ( int irq_idx )
{
  (void) irq_idx;

  // change led status
  if(ledstate) {
    // led off
    ledstate = 0;
    greenled_off();
  } else {
    // led on
    ledstate = 1;
    greenled_on();
  }

  timer0_clear_irq();
}

void isr_timer1 ( int irq_idx )
{
  (void) irq_idx;

  uart_puts("timer 1\n");
  timer1_clear_irq();
  
}

void isr_timer2 ( int irq_idx )
{
 (void) irq_idx;
 
  uart_puts("--- timer 2\n");
  timer2_clear_irq();
}

void timer0_gic_enable( void ) {
  gic_enable_irq (TMR_0_IRQNO, isr_timer0);
}

void timer1_gic_enable( void ) {
  gic_enable_irq (TMR_1_IRQNO, isr_timer1);
}

void timer2_gic_enable( void ) {
  gic_enable_irq (TMR_2_IRQNO, isr_timer2);
}

