#ifndef TX_BUF_H_
#define TX_BUF_H_

#include "my_types.h"
#include "rx_buf.h"

#define TX_BUF_SIZE (512)

extern uint8_t tx_buf[TX_BUF_SIZE];

void tx_wrack(uint32_t addr, uint8_t len);
void tx_rdack(COMMAND_T cmd, uint32_t addr, uint8_t len);
void tx_readRegsAck(uint32_t addr, uint8_t len);
void tx_readFifoAck(uint32_t addr, uint8_t len);
void tx_error(COMMAND_T cmd, uint32_t addr);

#endif /* TX_BUF_H_ */
