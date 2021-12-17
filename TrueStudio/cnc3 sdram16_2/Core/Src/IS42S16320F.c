#include "IS42S16320F.h"

FMC_SDRAM_CommandTypeDef command;
HAL_StatusTypeDef hal_stat;

void IS42S16320F_init(SDRAM_HandleTypeDef* hsdram) {
	__IO uint32_t tmpmrd = 0;
	/* Step 1:  Configure a clock configuration enable command */
	command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;

	/* Send the command */
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

	/* Step 2: Insert 100 us minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	HAL_Delay(1);

	/* Step 3: Configure a PALL (precharge all) command */
	command.CommandMode = FMC_SDRAM_CMD_PALL;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;

	/* Send the command */
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

	/* Step 4 : Configure a Auto-Refresh command */
	command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 8;
	command.ModeRegisterDefinition = 0;

	/* Send the command */
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

	/* Step 5: Program the external memory mode register */
	tmpmrd = (uint32_t)	SDRAM_MODEREG_BURST_LENGTH_1			|
						SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL		|
						SDRAM_MODEREG_CAS_LATENCY_2				|
						SDRAM_MODEREG_OPERATING_MODE_STANDARD	|
						SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	 command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	 command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	 command.AutoRefreshNumber = 1;
	 command.ModeRegisterDefinition = tmpmrd;

	 /* Send the command */
	 hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

	 /* Step 6: Set the refresh rate counter */
	 // 64msec / 4096 fresh = 15.62 us
	 // refresh count= 15.62usec * 84MHz - 20 = 1292
	 /* (15.62 us x Freq) – 20 */
	 // My1
	 // 64ms /8k = 7.8125us
	 // 7.8125us * 100 MHz - 20 = 761
	 // My2
	 // 64ms /8k = 7.8125us
	 // 7.8125us * 50 MHz - 20 = 370
	 // My3
	 // 64ms /8k = 7.8125us
	 // 7.8125us * 33.3 MHz - 20 = 240
	 HAL_SDRAM_ProgramRefreshRate(hsdram, 240); // refresh count
//	 hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));
}
