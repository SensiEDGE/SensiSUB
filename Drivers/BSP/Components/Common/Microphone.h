/**
 ******************************************************************************
 * @file    microphone.h
 * @author  
 * @version V1.0.0
 * @date    21-July-2017
 * @brief   This header file contains the functions prototypes for the
 *          microphone driver
 ******************************************************************************
 * @attention
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
#ifndef __MICROPHONE_H
#define __MICROPHONE_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/

#include "sensor.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup COMPONENTS COMPONENTS
 * @{
 */

/** @addtogroup COMMON COMMON
 * @{
 */

/** @addtogroup MICROPHONE MICROPHONE
 * @{
 */

/** @addtogroup MICROPHONE_Public_Types MICROPHONE Public types
 * @{
 */
#define MP34DT01_MIC_WHO_AM_I           0x88
    
/**
 * @brief  MICROPHONE driver structure definition
 */
typedef struct
{
  DrvStatusTypeDef ( *Init            ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *DeInit          ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Sensor_Enable   ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Sensor_Disable  ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_WhoAmI      ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Check_WhoAmI    ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Axes        ) ( DrvContextTypeDef*, SensorAxes_t* );
  DrvStatusTypeDef ( *Get_Sensitivity ) ( DrvContextTypeDef*, float* );
  DrvStatusTypeDef ( *Get_ODR         ) ( DrvContextTypeDef*, float* );
  DrvStatusTypeDef ( *Set_ODR         ) ( DrvContextTypeDef*, SensorOdr_t );
  DrvStatusTypeDef ( *Set_ODR_Value   ) ( DrvContextTypeDef*, float );
  DrvStatusTypeDef ( *Get_FS          ) ( DrvContextTypeDef*, float* );
  DrvStatusTypeDef ( *Set_FS          ) ( DrvContextTypeDef*, SensorFs_t );
  DrvStatusTypeDef ( *Set_FS_Value    ) ( DrvContextTypeDef*, float );
  DrvStatusTypeDef ( *Read_Reg        ) ( DrvContextTypeDef*, uint8_t, uint8_t* );
  DrvStatusTypeDef ( *Write_Reg       ) ( DrvContextTypeDef*, uint8_t, uint8_t );
  DrvStatusTypeDef ( *Get_DRDY_Status ) ( DrvContextTypeDef*, uint8_t* );
} MIC_Drv_t;

/**
 * @brief  MICROPHONE data structure definition
 */
typedef struct
{
  void *pComponentData; /* Component specific data. */
  void *pExtData;       /* Other data. */
} MIC_Data_t;

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

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __MICROPHONE_H */

/***************************************************************END OF FILE****/
