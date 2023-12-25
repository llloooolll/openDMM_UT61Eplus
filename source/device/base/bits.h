#ifndef __BITS_H__
#define __BITS_H__

#define SET_BIT(reg, position) ((reg) |= (1U << position))
#define CLR_BIT(reg, position) ((reg) &= ~(1U << position))
#define GET_BIT(reg, position) ((reg) & (1U << position))

#define SET_BITS(reg, mask) ((reg) |= (mask))
#define CLR_BITS(reg, mask) ((reg) &= ~(mask))
#define GET_BITS(reg, mask) ((reg) & (mask))

#endif
