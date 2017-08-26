#ifndef RPI2_H
#define RPI2_H

#include "types.h"
#include "memlayout.h"

#define VIC_BASE (0x3F00B200)

#define RPI_INTERRUPT_CONTROLLER_BASE   (0x3F00B200+KERNBASE)

/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
 *     See the BCM2835 ARM Peripherals manual, section 7.5 */
#define RPI_BASIC_ARM_TIMER_IRQ         (0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (7)


/** @brief The interrupt controller memory mapped register set */
typedef struct {
    volatile uint32 IRQ_basic_pending;
    volatile uint32 IRQ_pending_1;
    volatile uint32 IRQ_pending_2;
    volatile uint32 FIQ_control;
    volatile uint32 Enable_IRQs_1;
    volatile uint32 Enable_IRQs_2;
    volatile uint32 Enable_Basic_IRQs;
    volatile uint32 Disable_IRQs_1;
    volatile uint32 Disable_IRQs_2;
    volatile uint32 Disable_Basic_IRQs;

} rpi_irq_controller_t;


extern rpi_irq_controller_t* RPI_GetIrqController( void  );

#endif
