/**
******************************************************************************
  * @file    res-digital-output.c
  * @author  Central LAB
  * @version V1.0.0
  * @date    20-June-2016
  * @brief   digital output resource
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
 * \file
 *         digital output actuator
 * \author
 *         Nicola Stefani
 */
/*---------------------------------------------------------------------------*/
#include "ipso-objects.h"
#include "dev/sensor-common.h"
#include "sensors.h"
#include "st-lib.h"

/** @addtogroup LWM2M_example
  * @{
  */

/*---------------------------------------------------------------------------*/
#define CONTROLLED_PORT GPIOA
#define CONTROLLED_PIN	GPIO_PIN_5
/*---------------------------------------------------------------------------*/
void write_digital_pin(int level)
{
	if (level == 1){
		st_lib_hal_gpio_write_pin(CONTROLLED_PORT, CONTROLLED_PIN, GPIO_PIN_SET);
	} else {
		st_lib_hal_gpio_write_pin(CONTROLLED_PORT, CONTROLLED_PIN, GPIO_PIN_RESET);
	}
}
/*---------------------------------------------------------------------------*/
int read_digital_pin(void)
{
	GPIO_PinState state;

	state = st_lib_hal_gpio_read_pin(CONTROLLED_PORT, CONTROLLED_PIN);

	if (state == GPIO_PIN_SET) {
		return 1;
	} else {
		return 0;
	}

}
/*---------------------------------------------------------------------------*/
void init_digital_pin(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  GPIO_InitStruct.Pin       = CONTROLLED_PIN;
	  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull      = GPIO_PULLUP;
	  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

	  HAL_GPIO_Init(CONTROLLED_PORT, &GPIO_InitStruct);

}
/*---------------------------------------------------------------------------*/

/**
  * @}
  */


