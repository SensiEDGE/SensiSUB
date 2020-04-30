/**
******************************************************************************
  * @file    main.c
  * @author  Central LAB
  * @version V1.0.0
  * @date    20-January-2016
  * @brief   Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "main.h"
#include "cube_hal.h"
#include "radio_shield_config.h"
#include "spirit1.h"
#include "process.h"
#include "my_vl6180x.h"
#include "x-nucleo-6180xa1.h"
#include "vl6180x_platform.h"
#include "vl6180x_api.h"
#include "stm32f4xx_it.h"

/** @defgroup LWM2M_example
  * @{
  */

/** @addtogroup LWM2M_example
  * @{
  */


volatile uint8_t vl6180x_int = 0;

/* Private function prototypes -----------------------------------------------*/

void USARTConfig(void);
void Stack_6LoWPAN_Init(void);
void XNUCLEO6180XA1_WaitMilliSec(int n) {
    HAL_Delay(n); /* it's milli sec  cos we do set systick to 1KHz */
}

void DISP_ExecLoopBody(void) {}

/**
 * @brief  Main program
 * 		initialises HAL structures and calls the contiki main
 * @param  None
 * @retval None
 */
int main()
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    HAL_EnableDBGStopMode();
    
    MX_GPIO_Init();
    
    /* Initialize LEDs */
    BSP_LED_Init(LED2);

    RadioShieldLedInit(RADIO_SHIELD_LED);

    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

    USARTConfig();

    /* Initialize RTC */
    RTC_Config();
    RTC_TimeStampConfig();

    /*Compiler, HAL and firmware info:*/
  printf("\t(HAL %ld.%ld.%ld_%ld)\r\n"
      "\tCompiled %s %s"
#if defined (__IAR_SYSTEMS_ICC__)
      " (IAR)\r\n\n",
#elif defined (__CC_ARM)
      " (KEIL)\r\n\n",
#elif defined (__GNUC__)
      " (openstm32)\r\n\n",
#endif
         HAL_GetHalVersion() >>24,
        (HAL_GetHalVersion() >>16)&0xFF,
        (HAL_GetHalVersion() >> 8)&0xFF,
         HAL_GetHalVersion()      &0xFF,
       __DATE__,__TIME__);


    Stack_6LoWPAN_Init();

    while(1) {
      int r = 0;
      do {
        r = process_run();
      } while(r > 0);
#if !PRESENCE_PERIODIC
      if(vl6180x_int == 1)
      {
    	  vl6180x_int = 0;
    	  VL6180X_Changed();
      }
#endif

    }
}

/**
  * @}
  */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/