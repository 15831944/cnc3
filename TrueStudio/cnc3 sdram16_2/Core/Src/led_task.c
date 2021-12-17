#include "stm32h7xx_hal.h"
#include "led_task.h"
#include "my_types.h"
//#include "sem_led.h"
//#include "fpga.h"
//#include "encoder.h"

#include <stdio.h>

static uint8_t cnt;
static uint32_t tic, toc;

void ledEnable0(BOOL ena) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, ena);
}

void ledEnable1(BOOL ena) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, ena);
}

void led_task() {
	toc = HAL_GetTick();

	if (toc - tic >= LED_HALF_PERIOD) {
		ledEnable0(cnt & 1);
		ledEnable1(cnt & 2);
		cnt++;
		tic += LED_HALF_PERIOD;
	}
}
