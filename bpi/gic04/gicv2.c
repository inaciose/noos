#include <stdint.h>
#include "macro.h"
#include "gicv2_io.h"

// global variables
static void *gicc_base, *gicd_base;

//-------------------------------------------------------------------------

static void* get_gic_baseaddr(void)
{
  unsigned val;
  asm volatile("mrc p15, 4, %0, c15, c0, 0;"
      : "=r" (val)
      );
  /* FIXME Here we handle only 32 bit addresses. But there are 40 bit addresses with lpae */
  val >>= 15;
  val <<= 15;
  return (void*)val;
}

void gic_v2_init(void)
{
  void *gicbase = get_gic_baseaddr();
  gicd_base = gicbase + GICD_OFFSET; // global variable
  gicc_base = gicbase + GICC_OFFSET; // global variable
  
  // Global enable forwarding interrupts from distributor to cpu interface
  mmio_write32(gicd_base + GICD_CTLR, GICD_CTLR_GRPEN1);
  
  // Global enable signalling of interrupt from the cpu interface
  mmio_write32(gicc_base + GICC_CTLR, GICC_CTLR_GRPEN1);
  mmio_write32(gicc_base + GICC_PMR, GICC_PMR_DEFAULT);
}

void *gic_v2_gicd_get_address(void)
{
  return gicd_base;
}

void *gic_v2_gicc_get_address(void)
{
  return gicc_base;
}

void gic_v2_irq_enable(unsigned int irqn)
{
  unsigned m ,n, val;
  void *address;
  m = irqn;
  n = m / 32;
  address = gicd_base + GICD_ISENABLER + 4*n;
  val = mmio_read32(address);
  val |= 1 << (m % 32);
  mmio_write32(address, val);
}

void gic_v2_irq_set_prio(int irqno, int prio)
{
  /* See doc: ARM Generic Interrupt Controller: Architecture Specification version 2.0
   * section 4.3.11
   */
  
  int n, m, offset;
  volatile uint8_t *gicd = gic_v2_gicd_get_address();
  m = irqno;
  n = m / 4;
  offset = GICD_IPRIORITYR + 4*n;
  offset += m % 4; /* Byte offset */
  
  gicd[offset] = 0xff;
  
  gicd[offset] = prio << 4;  
}

void gic_irq_enable(int irqno)
{
  volatile uint8_t *gicd = gic_v2_gicd_get_address() + GICD_ITARGETSR;
  int n, m, offset;
  m = irqno;
  n = m / 4;
  offset = 4*n;
  offset += m % 4;
  
  gicd[offset] |= gicd[0];
  
  gic_v2_irq_set_prio(irqno, LOWEST_INTERRUPT_PRIORITY);  
  gic_v2_irq_enable(irqno);  
}

void gic_eoi(int intn)
{
  //GICC_REG(GICC_EOIR) = intn;
  mmio_write32(0x01C82010, intn); 
}

int gic_getack( void )
{
  //return GICC_REG(GICC_IAR);
  return mmio_read32(0x01C8200C);
}
