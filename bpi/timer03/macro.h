// Memory-Mapped I/O macros
//-------------------------------------------------------------------------

// Memory-Mapped I/O input
#define mmio_read32(addr)         (*((volatile unsigned long  *)(addr)))
// Memory-Mapped I/O output
#define mmio_write32(addr, v)     (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define mmio_write32or(addr, v)   (*((volatile unsigned long  *)(addr)) |= (unsigned long)(v))

//-------------------------------------------------------------------------
// General word, byte and bit macros
//-------------------------------------------------------------------------

// check if one bit is set in a int
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
