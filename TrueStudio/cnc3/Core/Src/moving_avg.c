#include "moving_avg.h"

#include <stdio.h>
#include <float.h>

static unsigned wraddr, n;
static int valid;
static float sum = 0;
static float ar[MOVING_AVG_LEN] = {0};

void mavg_clear() {
	wraddr = 0;
	n = 0;
	valid = 0;
	sum = 0;
}

int mavg_n() { return (int)n; }

void mavg_add(float value) {
	float old = ar[wraddr];
	ar[wraddr++] = value;
	sum += valid ? value - old : value; // because we don't clear array

	if (wraddr >= MOVING_AVG_LEN)
		wraddr = 0;

	if (n < MOVING_AVG_LEN) {
		n++;

		if (n >= MOVING_AVG_LEN)
			valid = 1;
	}
}

int mavg_empty() { return n == 0; }
int mavg_used() { return n; }
int mavg_valid() { return valid; }

int mavg_ready(float lim) {
	return valid || sum > lim;
}

float mavg_get() {
//	return valid ? sum / MOVING_AVG_LEN : 0;
//	return valid ? sum / MOVING_AVG_LEN : FLT_MAX;
	return n ? sum / n : FLT_MAX;
}

void mavg_tb() {
	printf("avg_tb:\n");
	for (int i = 0; i < 0x100; i++) {
		int x = i & 0x3f;
		x += 100;
		mavg_add(x);
		printf("%d %d N:%d v:%x r:%x\n", x, (int)(mavg_empty() ? 0 : mavg_get()), mavg_used(), mavg_valid(), mavg_ready(1000));
	}

	printf("avg clear\n");
	mavg_clear();
	for (int i = 0; i < 0x100; i++) {
		int x = i & 0x3f;
		x += 900;
		mavg_add(x);
		printf("%d %d N:%d v:%x r:%x\n", x, (int)(mavg_empty() ? 0 : mavg_get()), mavg_used(), mavg_valid(), mavg_ready(1000));
	}
}
