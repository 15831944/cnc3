#include "cnc_param.h"

#include <math.h>

#include "my_types.h"
#include "fpga.h"

static double step = STEP; // motor step, mm
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
	step = STEP;
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
void cnc_setStep(double value) {
	if (cnc_isIdle())
		step = value < STEP_MIN ? STEP_MIN : value > STEP_MAX ? STEP_MAX : value;
}
// mm
double cnc_step() { return step; }

/* Acceleration and Deceleration in a feedback mode
 * um / s^2
 */
void cnc_setAcc(float value) { acc = value * COE_UMSEC2_TO_MMTICK2; }
void cnc_setDec(float value) { dec = value * COE_UMSEC2_TO_MMTICK2; }

// um / s^2
float cnc_getAcc() { return acc * (1.0 / COE_UMSEC2_TO_MMTICK2); }
float cnc_getDec() { return dec * (1.0 / COE_UMSEC2_TO_MMTICK2); }

// mm / clock^2
float cnc_acc() { return acc; }
float cnc_dec() { return dec; }

// mm / clock^2 / Vcode
float cnc_pidAcc() { return acc * COE_DCODE_TO_VOLT; }
float cnc_pidDec() { return dec * COE_DCODE_TO_VOLT; }

/* Scale of motors and linear encoders
 * steps / mm
 */
static double limitScale(double value) { return value < SCALE_MIN ? SCALE_MIN : value > SCALE_MAX ? SCALE_MAX : value; }

void cnc_setScaleX(double value) {
	if (cnc_isIdle())
		scale_xy.x = limitScale(value);
}
void cnc_setScaleY(double value) {
	if (cnc_isIdle())
		scale_xy.y = limitScale(value);
}
void cnc_setScaleU(double value) {
	if (cnc_isIdle())
		scale_uv.x = limitScale(value);
}
void cnc_setScaleV(double value) {
	if (cnc_isIdle())
		scale_uv.y = limitScale(value);
}
void cnc_setScaleEncX(double value) {
	if (cnc_isIdle())
		scale_enc.x = limitScale(value);
}
void cnc_setScaleEncY(double value) {
	if (cnc_isIdle())
		scale_enc.y = limitScale(value);
}

// steps / mm
const scale_t* cnc_scaleXY() { return &scale_xy; }
double cnc_scaleX() { return scale_xy.x; }
double cnc_scaleY() { return scale_xy.y; }
double cnc_getScale(AXIS_T axis) { return axis == AXIS_X ? scale_xy.x : (AXIS_Y ? scale_xy.y : 0); }

// steps / mm
const scale_t* cnc_scaleUV() { return &scale_uv; }
double cnc_scaleU() { return scale_uv.x; }
double cnc_scaleV() { return scale_uv.y; }

// steps / mm
const scale_t* cnc_scaleEnc() { return &scale_enc; }
double cnc_scaleEncX() { return scale_enc.x; }
double cnc_scaleEncY() { return scale_enc.y; }
