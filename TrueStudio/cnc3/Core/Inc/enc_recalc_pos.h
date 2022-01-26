#ifndef INC_ENC_RECALC_POS_H_
#define INC_ENC_RECALC_POS_H_

#include "my_types.h"
#include "line.h"
#include "arc.h"

void cnc_setEncXYMode(BOOL ena);
void cnc_clearEncXYMode();
BOOL cnc_isEncMode();
void clear_enc_reg();
size_t enc_recalc_pos(point_t* const mtr_pt, const point_t* const mtr_uv_pt, const line_t* const line, const arc_t* const arc, const size_t step_id, BOOL* const is_last);

#endif /* INC_ENC_RECALC_POS_H_ */
