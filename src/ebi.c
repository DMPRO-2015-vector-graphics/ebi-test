/**************************************************************************//**
 * @file
 * @brief Empty Project
 * @author Energy Micro AS
 * @version 3.20.2
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/
/*#include "em_device.h"
#include "em_chip.h"
#include "em_ebi.h"*/

#include "bsp.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_ebi.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include <math.h>

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

/* Local prototypes */
void Delay(uint32_t dlyTicks);

/* Counts 1ms timeTicks */
volatile uint32_t msTicks;

/* Local prototypes */
void Delay(uint32_t dlyTicks);

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

int main(void)
{
  CHIP_Init();

  // Enable clocks
  CMU_ClockEnable(cmuClock_EBI, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

  /* Configure board. Select either EBI or SPI mode. */
  BSP_Init(BSP_INIT_DK_SPI);

  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();

  // EBI config
  EBI_Init_TypeDef ebiConfig = EBI_INIT_DEFAULT;



  // Set EBI address pins PE[15:8]
  GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 9, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 8, gpioModePushPull, 0);

  // Set EBI data pins PA15 + PA[6:0]
  GPIO_PinModeSet(gpioPortA, 15, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 6, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 4, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 0);

  /* EBI ARDY/WEN/REN/ALE */
  GPIO_PinModeSet( gpioPortF,  2, gpioModeInput,    0 );
  GPIO_PinModeSet( gpioPortF,  8, gpioModePushPull, 0 );
  GPIO_PinModeSet( gpioPortF,  9, gpioModePushPull, 0 );
  GPIO_PinModeSet( gpioPortC, 11, gpioModePushPull, 0 );

  /* --------------------------------------------------------- */
  /* Second bank needs a name, Bank 0, Base Address 0x80000000 */
  /* --------------------------------------------------------- */
      ebiConfig.banks       = EBI_BANK0;
      ebiConfig.csLines     = EBI_CS1;
      ebiConfig.mode        = ebiModeD8A8;
      ebiConfig.alePolarity = ebiActiveHigh;
      ebiConfig.location    = ebiLocation1;
      /* keep blEnable */
      ebiConfig.blEnable     = false;
      ebiConfig.addrHalfALE  = false;
      ebiConfig.readPrefetch = false;
      ebiConfig.noIdle       = true;

      /* keep alow/ahigh configuration */
      ebiConfig.aLow = ebiALowA0;
      ebiConfig.aHigh = ebiAHighA18;

      /* Address Setup and hold time */
      ebiConfig.addrHoldCycles  = 3;
      ebiConfig.addrSetupCycles = 3;

      /* Read cycle times */
      ebiConfig.readStrobeCycles = 7;
      ebiConfig.readHoldCycles   = 3;
      ebiConfig.readSetupCycles  = 3;

      /* Write cycle times */
      ebiConfig.writeStrobeCycles = 7;
      ebiConfig.writeHoldCycles   = 3;
      ebiConfig.writeSetupCycles  = 3;

      /* Configure EBI bank 1 */
      EBI_Init(&ebiConfig);


  uint16_t *BANK0_BASE_ADDR = 0x80000000;
  uint16_t DATA = 0xF000;
  uint32_t cnt = 0;
		  *BANK0_BASE_ADDR = 0x1;
		  BSP_LedsSet(0xff00);
  while (1) {
	  //if(cnt > 500000){
		  *BANK0_BASE_ADDR = DATA;
		  BSP_LedsSet(DATA);
		  Delay(500);
	  //}else{
		  *BANK0_BASE_ADDR = 0x0000;
		  BSP_LedsSet(0x0000);
	  //}
		  Delay(500);
	  //if(cnt>1000000)
	//	  cnt=0;
	  //cnt++;

  }
}

