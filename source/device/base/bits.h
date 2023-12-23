#ifndef __BITS_H__
#define __BITS_H__

#define BIT_SET(reg, position) ((reg) |= (1U << position))
#define BIT_CLR(reg, position) ((reg) &= ~(1U << position))
#define BIT_GET(reg, position) ((reg) & (1U << position))

#define BITS_SET(reg, mask) ((reg) |= (mask))
#define BITS_CLR(reg, mask) ((reg) &= ~(mask))
#define BITS_GET(reg, mask) ((reg) & (mask))

#endif
