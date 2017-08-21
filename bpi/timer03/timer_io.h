#ifndef _TIMER_IO_H_
#define _TIMER_IO_H_

#define SUNXI_TIMER_BASE    0x01c20c00

#define TMR_IRQ_EN        ( SUNXI_TIMER_BASE + 0x00 ) // All timer IRQ's Enable/Disable
#define TMR_IRQ_STA       ( SUNXI_TIMER_BASE + 0x04 ) // All timer status (pending interrups)

#define TMR_0_CTRL        ( SUNXI_TIMER_BASE + 0x10 ) // Timer 0 control
#define TMR_0_INTR_VAL    ( SUNXI_TIMER_BASE + 0x14 ) // Timer 0 interval value
#define TMR_0_CUR_VAL     ( SUNXI_TIMER_BASE + 0x18 ) // Timer 0 current value

#define TMR_1_CTRL        ( SUNXI_TIMER_BASE + 0x20 ) // Timer 1 control
#define TMR_1_INTR_VAL    ( SUNXI_TIMER_BASE + 0x24 ) // Timer 1 interval value
#define TMR_1_CUR_VAL     ( SUNXI_TIMER_BASE + 0x28 ) // Timer 1 current value

#define TMR_2_CTRL        ( SUNXI_TIMER_BASE + 0x30 ) // Timer 2 control
#define TMR_2_INTR_VAL    ( SUNXI_TIMER_BASE + 0x34 ) // Timer 2 interval value
#define TMR_2_CUR_VAL     ( SUNXI_TIMER_BASE + 0x38 ) // Timer 2 current value

//#define CLK_HZ          1000000     // the clock is 1MHZ

#define TMR_0_IRQNO 54 // 0x36
#define TMR_1_IRQNO 55 // 0x37
#define TMR_2_IRQNO 56 // 0x38

#endif
