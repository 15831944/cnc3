#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "my_types.h"

void enc_clear();
uint16_t enc_getStatus();
BOOL enc_getError();

void enc_enable();
void enc_disable();
uint16_t enc_isEnabled();

void enc_setDir(uint16_t value);
uint16_t enc_getDir();

void enc_snap();

int32_t enc_get(size_t i);
void enc_getXY(int32_t* const x, int32_t* const y);
void enc_set(size_t i, int32_t value);
void enc_setXY(int32_t x, int32_t y);

void enc_print();

#endif /* INC_ENCODER_H_ */
