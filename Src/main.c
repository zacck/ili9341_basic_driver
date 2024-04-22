/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f407xx.h"
#include "reg_util.h"
#include "bsp_lcd.h"

void SystemClockSetup(void);


int main(void)
{
	SystemClockSetup();
	bsp_lcd_init();
    /* Loop forever */
	for(;;);
}

void SystemClockSetup(void){
	RCC_TypeDef *pRcc = RCC;
	FLASH_TypeDef *pFlash = FLASH;
	PWR_TypeDef *pPwr = PWR;



	// program flash overdrive and wait states
	REG_SET_VAL(pFlash->ACR, 0x5U, 0xFU, FLASH_ACR_LATENCY_Pos);

	//2. Over drive settings
	REG_SET_BIT(pRcc->APB1ENR, RCC_APB1ENR_PWREN_Pos); /*Enable clock for PWR register access*/
	REG_SET_BIT(pPwr->CR, PWR_CR_VOS_Pos); 				/*VOS = 1*/


	// set up main PLL
	REG_SET_VAL(pRcc->PLLCFGR, 0x8U, 0x3FU, RCC_PLLCFGR_PLLM_Pos); //PLL_M
	REG_SET_VAL(pRcc->PLLCFGR, 168U, 0x1FFU, RCC_PLLCFGR_PLLN_Pos); //PLL_N
	REG_SET_VAL(pRcc->PLLCFGR, 0x00U, 0x3U, RCC_PLLCFGR_PLLP_Pos); //PLL_P

	// clock prescalers
	REG_SET_VAL(pRcc->CFGR, 0U, 0xFU, RCC_CFGR_HPRE_Pos); /*AHB prescaler*/
	REG_SET_VAL(pRcc->CFGR, 0x5U, 0x7U, RCC_CFGR_PPRE1_Pos); /*APB1 prescaler*/
	REG_SET_VAL(pRcc->CFGR, 0x4U, 0x7U, RCC_CFGR_PPRE2_Pos); /*APB2 prescaler*/

	//Turn On PLL and wait for PLL READY
	REG_SET_BIT(pRcc->CR, RCC_CR_PLLON_Pos);
	while(!REG_READ_BIT(pRcc->CR, RCC_CR_PLLRDY_Pos));

	//use PLL as system clock
	REG_SET_VAL(pRcc->CFGR, 0x2U, 0x3U, RCC_CFGR_SW_Pos);
	while(!(REG_READ_VAL(pRcc->CFGR, 0x3U, RCC_CFGR_SWS_Pos) == 0x2U));



}


