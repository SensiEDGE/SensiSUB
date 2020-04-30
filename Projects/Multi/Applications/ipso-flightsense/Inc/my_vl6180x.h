/**
******************************************************************************
  * @file    my_vl6180x.h
  * @author  Central LAB
  * @version V1.0.0
  * @date    20-January-2016
  * @brief   Header for vl6180x sensor
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

#include "vl6180x_platform.h"
#include "vl6180x_api.h"

#define PRESENCE_LOW_TH 20
#define PRESENCE_HIGH_TH 25


/**
 * @brief This function initializes one VL6180X  device for reading Lux or Distance
 * @param enum measure_t measure type of measure that we want: RANGE_ONLY/ALS_ONLY/RANGE_ALS
 * @param uint32_t Sn device number
 * @retval None
 */
void Kickoff_VL6180X_ProximitySen(measure_t measure,uint32_t Sn);


/**
 * @brief This function Initializes the X-NUCLEO-618XA1 expansion understanding
 * if it's a Version 1 or 2, and the number of attached satellites
 * @param None
 * @retval None
 */
void Init_VL6180X_ProximitySens(void);


/**
 * @brief This function reads the proximity value of one VL6180X  device
 * @param uint32_t Sn device number
 * @retval uint16_t distance Value read
 */
uint16_t RdFligthSense_Distance (uint32_t Sn);

void Flight_Clear_Int(void);
