#ifndef INC_DEFINES_H_
#define INC_DEFINES_H_

#define RESEASE

#define NAME ("CNC")
#define MODEL ("0.3")

#if defined(RESEASE)
	#define VER_TYPE (1) // 0 - stable, 1 - alpha, 2 - beta
	#define WDT_ENA // for MCU program error
	#define SOFT_WDT // for lose USB connection
#else
	#define VER_TYPE (2) // 0 - stable, 1 - alpha, 2 - beta
	#define PRINT
//	#define SOFT_WDT
#endif

//#define STONE
#ifdef STONE
	#define IS_STONE (1)
#else
	#define IS_STONE (0)
#endif

#define FAC_VER (0)
#define FAC_REV (3) // CNC3
#define VER (1)
#define REV (1)

// 0.2.2.10 - semaphore unblocked at the start with error
// 0.2.2.11 - corrected feedback calculation error
// 0.2.2.12 - PID for V

///////////////////////////////////////
#define FPGA_CLOCK (72e6) // Hz
#define DEFAULT_PAUSE_CLOCK (FPGA_CLOCK / 7200) // 10kHz

#define MS_TO_TICK(_MS) ((1.0e-3 * FPGA_CLOCK) * _MS)

#define ROLLBACK_DEFAULT	(0.3) // mm
#define ROLLBACK_ATTEMPTS	(3)

//#define F_DEFAULT (1.0 / 1000.0 * 60) // step in 1 sec
#define F_DEFAULT_UMS	(300.0) // um/s
#define F_ROLLBACK_DEFAULT_UMS	(30.0) // um/s
//#define F_DEFAULT		(F_DEFAULT_UMS / 1000.0 * 60) // 18 mm/min
#define T_DEFAULT_TICK	( FPGA_CLOCK / (F_DEFAULT_UMS * 1e-3) ) // 240,000,000 clocks/mm
#define T_ROLLBACK_DEFAULT_TICK	( FPGA_CLOCK / (F_ROLLBACK_DEFAULT_UMS * 1e-3) ) // 2,400,000,000 clocks/mm

#define F_MIN_UMS (1) // um/sec
#define F_MAX_UMS (F_DEFAULT_UMS) // um/sec
#define F_MIN_MMM ( F_MIN_UMS * 60.0 * 1e-3 ) // mm / min
#define F_MAX_MMM ( F_MAX_UMS * 60.0 * 1e-3 ) // mm / min

#define COE_UMSEC2_TO_MMTICK2 ( 1e-3 / (FPGA_CLOCK * FPGA_CLOCK) )
#define COE_UMS_TO_MMTICK ( 1e-3 / FPGA_CLOCK )
#define UMS_TO_MMTICKS(UMS)	( UMS * COE_UMS_TO_MMTICK )

#define UMS_TO_TICKSMM(UMS)	( FPGA_CLOCK / (UMS * 1e-3) )

#define T_MIN_TICK			UMS_TO_TICKSMM(F_MAX_UMS) // 240,000,000 clocks / mm
#define T_MAX_TICK			UMS_TO_TICKSMM(F_MIN_UMS) // 72,000,000,000 clocks / mm

#define F_MIN (1.0 / T_MAX_TICK)
#define F_MAX (1.0 / T_MIN_TICK)

#define SCALE			(1000.0) // steps / mm
#define SCALE_UV		(1000.0) // steps / mm
#define SCALE_ENC		(1000.0 / 5) // steps / mm

#define DEFAULT_STEP	(0.001) // mm
#define STEP_MIN		(0.001) // mm

#define DRUM_VEL_MAX (7)

//#define SUM_IMIT_STEPS

//#define UDIV_CEIL(N, D) (N % D ? N / D + 1 : N / D)

#endif /* INC_DEFINES_H_ */
