/* Additional variables and functions for cnc_task.h
 * File should used only in cnc_task.c
 * cnc_* functions should be copied to cnc_task.h
 */

#ifndef INC_CNC_TASK_EXT_H_
#define INC_CNC_TASK_EXT_H_

#include "defines.h"
#include "my_types.h"

extern BOOL enc_mode, enc_reg_valid;
extern int32_t enc_x_reg, enc_y_reg, pos_x_enc_reg, pos_y_enc_reg;

void cnc_setEncModeXY(BOOL ena);
BOOL cnc_isEncMode();

void clear_enc_reg();

#endif /* INC_CNC_TASK_EXT_H_ */
