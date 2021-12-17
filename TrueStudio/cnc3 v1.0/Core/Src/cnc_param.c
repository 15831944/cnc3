#include "cnc_param.h"

#include <math.h>

#include "my_types.h"
#include "fpga.h"

static double step = DEFAULT_STEP; // motor step, mm
static scale_t scale_xy = {SCALE, SCALE}, scale_uv = {SCALE_UV, SCALE_UV}, scale_enc = {SCALE_ENC, SCALE_ENC}; // steps / mm

static float acc = ACC, dec = DEC; // mm/tick^2/V

/* result - mean velocity, mm/clock
 * v0 - initial velocity, mm/clock
 * acc - acceleration, mm/clock^2
 * l - lenght, mm
 */
float velocity(float v0, float acc, float d) {
	static float v1_sq, v1;

	v1_sq = v0 * v0 + 2 * acc * d;
	v1 = v1_sq > 0 ? sqrt(v1_sq) : 0;
	return 0.5 * (v0 + v1);
}

void cnc_resetParam() {
	step = DEFAULT_STEP;
	scale_xy.x = SCALE;
	scale_xy.y = SCALE;
	scale_uv.x = SCALE_UV;
	scale_uv.y = SCALE_UV;
	scale_enc.x = SCALE_ENC;
	scale_enc.y = SCALE_ENC;
	acc = ACC;
	dec = DEC;
}

BOOL cnc_isIdle();

// mm
void cnc_setStep(float value) {
	if (cnc_isIdle())
		step = value < STEP_MIN ? STEP_MIN : value;
}
// mm
float cnc_step() { return step; }

// um/s^2
void cnc_setAcc(float value) { acc = value * COE_UMSEC2_TO_MMTICK2; }
void cnc_setDec(float value) { dec = value * COE_UMSEC2_TO_MMTICK2; }

// um/s^2
float cnc_getAcc() { return acc * (1.0 / COE_UMSEC2_TO_MMTICK2); }
float cnc_getDec() { return dec * (1.0 / COE_UMSEC2_TO_MMTICK2); }

// mm/clock^2
float cnc_acc() { return acc; }
float cnc_dec() { return dec; }

// mm/clock^2 / Vcode
float cnc_pidAcc() { return acc * COE_DCODE_TO_VOLT; }
float cnc_pidDec() { return dec * COE_DCODE_TO_VOLT; }

// steps / mm
const scale_t* cnc_scale_xy() { return &scale_xy; }
double cnc_scale_x() { return scale_xy.x; }
double cnc_scale_y() { return scale_xy.y; }
double cnc_getScale(AXIS_T axis) { return axis == AXIS_X ? scale_xy.x : (AXIS_Y ? scale_xy.y : 0); }
// steps / mm
const scale_t* cnc_scale_uv() { return &scale_uv; }
double cnc_scale_u() { return scale_uv.x; }
double cnc_scale_v() { return scale_uv.y; }
// steps / mm
const scale_t* cnc_scale_enc() { return &scale_enc; }
double cnc_scale_enc_x() { return scale_enc.x; }
double cnc_scale_enc_y() { return scale_enc.y; }
