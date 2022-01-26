#include "cnc_task.h"
#include "cnc_task_ext.h"

BOOL enc_mode = FALSE;
BOOL enc_reg_valid = FALSE;
int32_t enc_x_reg = 0, enc_y_reg = 0;
int32_t pos_x_enc_reg = 0, pos_y_enc_reg = 0;

void cnc_setEncModeXY(BOOL ena) {
	if ( cnc_isIdle() ) {
		enc_mode = ena;
		clear_enc_reg();
	}
}
BOOL cnc_isEncMode() { return enc_mode; }

void clear_enc_reg() {
	enc_reg_valid = FALSE;
	enc_x_reg = enc_y_reg = 0;
	pos_x_enc_reg = pos_y_enc_reg = 0;
}
