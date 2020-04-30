/**
 ******************************************************************************
 * @file    MP34DT01_MIC_driver_HL.h
 * @author  
 * @version V3.0.0
 * @date    21-July-2017
 * @brief   This file contains definitions for the MP34DT01_MIC_driver_HL.c 
 *          firmware driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MP34DT01_MIC_DRIVER_HL_H
#define __MP34DT01_MIC_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/

#include "microphone.h"

/* Include mic sensor component drivers. */
#include "x_nucleo_iks01a1_audio_l4.h"



/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENTS COMPONENTS
 * @{
 */

/** @addtogroup MP34DT01 MP34DT01
 * @{
 */

/** @addtogroup MP34DT01_Public_Constants Public constants
 * @{
 */

#define MP34DT01_SENSORS_MAX_NUM  1     /**< MP34DT01 max number of instances */


/** @addtogroup MP34DT01_MIC_SENSITIVITY MIC sensitivity values based on selected full scale
 * @{
 */

//#define MP34DT01_MIC_SENSITIVITY_FOR_FS_245DPS   08.75  /**< Sensitivity value for 245 dps full scale [mdps/LSB] */
//#define MP34DT01_MIC_SENSITIVITY_FOR_FS_500DPS   17.50  /**< Sensitivity value for 500 dps full scale [mdps/LSB] */
//#define MP34DT01_MIC_SENSITIVITY_FOR_FS_2000DPS  70.00  /**< Sensitivity value for 2000 dps full scale [mdps/LSB] */


/**
 * @}
 */

/** @addtogroup MP34DT01_Public_Types MP34DT01 Public Types
 * @{
 */

/**
 * @brief MP34DT01 combo specific data internal structure definition
 */

typedef struct
{
  uint8_t isMicEnabled;
  float   lastMicODR;
} MP34DT01_Combo_Data_t;

/**
 * @brief MP34DT01 mic specific data internal structure definition
 */

typedef struct
{
  float Previous_ODR;
  MP34DT01_Combo_Data_t *comboData;       /* Combo data to manage in software ODR of the combo sensors */
} MP34DT01_X_Data_t;

/**
 * @brief MP34DT01 mic specific data internal structure definition
 */

typedef struct
{
  float Previous_ODR;
  MP34DT01_Combo_Data_t *comboData;       /* Combo data to manage in software ODR of the combo sensors */
} MP34DT01_M_Data_t;

/**
 * @}
 */

/** @addtogroup MP34DT01_Public_Variables Public variables
 * @{
 */

extern MIC_Drv_t MP34DT01_X_Drv;
extern MIC_Drv_t MP34DT01_G_Drv;
extern MP34DT01_Combo_Data_t MP34DT01_Combo_Data[MP34DT01_SENSORS_MAX_NUM];

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __MP34DT01_MIC_DRIVER_HL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
