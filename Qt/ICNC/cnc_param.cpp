#include "cnc_param.h"
#include <cmath>

using namespace  std;

uint16_t CncParam::inputLevel = DEFAULT_INPUT_LEVEL;
bool CncParam::sdEnable = DEFAULT_SD_ENA;
bool CncParam::reverseX = false;
bool CncParam::reverseY = false;
bool CncParam::reverseU = false;
bool CncParam::reverseV = false;
bool CncParam::swapXY = false;
bool CncParam::swapUV = false;
bool CncParam::reverseEncX = false;
bool CncParam::reverseEncY = false;
double CncParam::step = DEFAULT_STEP; // mm
double CncParam::scaleX = DEFAULT_SCALE_XY, CncParam::scaleY = DEFAULT_SCALE_XY, CncParam::scaleU = DEFAULT_SCALE_UV, CncParam::scaleV = DEFAULT_SCALE_UV;
double CncParam::scaleEncX = DEFAULT_SCALE_ENC_XY, CncParam::scaleEncY = DEFAULT_SCALE_ENC_XY;
bool CncParam::encXY = false;

bool CncParam::fb_ena = false;
unsigned CncParam::rb_attempts = DEFAULT_RB_ATTEMPTS;
double CncParam::low_thld = 0, CncParam::high_thld = 200, CncParam::rb_to = DEFAULT_RB_TO, CncParam::rb_len = DEFAULT_RB_LEN, CncParam::rb_speed = DEFAULT_RB_SPEED;
double CncParam::acc = DEFAULT_ACC, CncParam::dec = DEFAULT_DEC;

// scale - steps/mm
int32_t CncParam::mm_to_steps(double mm, double scale) {
    double res = mm * scale;

    if (res > INT32_MAX)
        res = INT32_MAX;
    else if (res < INT32_MIN)
        res = INT32_MIN;

    return static_cast<int32_t>(round(res));
}

// scale - steps/mm
double CncParam::steps_to_mm(int32_t steps, double scale) {
    return steps / scale;
}

double CncParam::ums_to_mmm(double value) {
    return value * (60.0 / 1000.0);
}