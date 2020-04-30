/**
******************************************************************************
  * @file    main.c
  * @author  Central LAB
  * @version V1.1.0
  * @date    08-February-2017
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
/**
 * @mainpage FP-SNS-6LPNODE1 IPSO Smart Objects/OMA LWM2M over 6LoWPAN Software
 *
 * @image html st_logo.png
 *
 * <b>Introduction</b>
 *
 * This firmware package includes Components Device Drivers, Board Support Package
 * and example application for the following STMicroelectronics elements:
 * - X-NUCLEO-IDS01A4/X-NUCLEO-IDS01A5 SPIRIT1 Sub-1GHz radio expansion boards
 * - X-NUCLEO-IKS01A1/X-NUCLEO-IKS01A2 mems sensors expansion boards
 * - X-NUCLEO-6180XA1 Flightsense expansion board
 * - NUCLEO-F401RE/NUCLEO-L152RE Nucleo board
 *
 * <b>Components of the FP-SNS-6LPNODE1 according to STM32Cube architecture</b><br>
 * \image html FP-SNS-6LPNODE1_Components.png "Figure 1: FP-SNS-6LPNODE1 block diagram"
 *
 * <b>Example Application: ipso-nosensors</b>
 *
 * This application implements a IPSO Smart Object Profile (OMA LWM2M client) for a 6LoWPAN network, using the OMA LWM2M/CoRE CoAP protocol suite to connect to a OMA LWM2M Server.
 * Implemented Objects are:
 *
 * - Server
 * - Device
 * - IPSO Digital Input (NUCLEO board User Button)
 * - IPSO Light Control (NUCLEO board green LED and X-NUCLEO-IDS01Ax board red LED)
 *
 * <b>Example Application: ipso-flightsense</b>
 *
 * This application (available for NUCLEO-F401RE only) implements a IPSO Smart Object Profile (OMA LWM2M client) for a 6LoWPAN network, using the OMA LWM2M/CoRE CoAP protocol suite to connect to a OMA LWM2M Server.
 * Implemented Objects are:
 *
 * - Server
 * - Device
 * - IPSO Digital Input (NUCLEO board User Button)
 * - IPSO Digital Output (a pin of the NUCLEO)
 * - IPSO Presence Sensor
 * - IPSO Light Control (NUCLEO board green LED and X-NUCLEO-IDS01Ax board red LED)
 *
 * <b>Example Application: ipso-mems</b>
 *
 * This application implements a IPSO Smart Object Profile (OMA LWM2M client) for a 6LoWPAN network, using the OMA LWM2M/CoRE CoAP protocol suite to connect to a OMA LWM2M Server.
 * Implemented Objects are:
 *
 * - Server
 * - Device
 * - IPSO Digital Input (NUCLEO board User Button)
 * - IPSO Temperature
 * - IPSO Humidity
 * - IPSO Light Control (NUCLEO board green LED)
 * - IPSO Accelerometer
 * - IPSO Magnetometer
 * - IPSO Barometer
 *
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

/** @defgroup LWM2M_example
  * @{
  */

/** @addtogroup LWM2M_example
  * @{
  */

/* Function prototypes -------------------------------------------------------*/
void USARTConfig(void);
void Stack_6LoWPAN_Init(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void RTC_Config(void);
void RTC_TimeStampConfig(void);
/*----------------------------------------------------------------------------*/
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
    }

}

/**
  * @}
  */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
