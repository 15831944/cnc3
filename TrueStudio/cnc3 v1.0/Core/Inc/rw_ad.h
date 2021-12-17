#ifndef INC_RW_AD_H_
#define INC_RW_AD_H_

#include "my_types.h"

#define MCU_BAR		(0x00000000)
#define FPGA_BAR	(0x04000000)
#define PA_BAR		(0x08000000)
#define ADDR_MASK	(0xFC000000)

void reset();

void ad_writeRegs(size_t addr, size_t len, const uint8_t buf[], size_t N);
void ad_readRegs(uint32_t addr, size_t len);
void ad_readFifo(uint32_t addr, size_t len);

#endif /* INC_RW_AD_H_ */
