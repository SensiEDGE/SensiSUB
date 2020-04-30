/**
 ******************************************************************************
 * @file    x_nucleo_iks01a1_mic.h
 * @author  
 * @version V1.0.0
 * @date    24-July-2017
 * @brief   This file contains definitions for the x_nucleo_iks01a1_mic.c
 ******************************************************************************
 * @attention
 *
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
#ifndef __X_NUCLEO_IKS01A1_MIC_H
#define __X_NUCLEO_IKS01A1_MIC_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
//#include "LSM6DS0_ACC_GYRO_driver_HL.h"
#include "x_nucleo_iks01a1.h"



/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup X_NUCLEO_IKS01A1 X_NUCLEO_IKS01A1
 * @{
 */

/** @addtogroup X_NUCLEO_IKS01A1_MIC Mic
 * @{
 */

/** @addtogroup X_NUCLEO_IKS01A1_MIC_Public_Types Public types
  * @{
  */

typedef enum
{
  MIC_SENSORS_AUTO = -1,        /* Always first element and equal to -1 */
  MP34DT01,                     /* Default on board. */
} MIC_ID_t;

/**
 * @}
 */

/** @addtogroup X_NUCLEO_IKS01A1_MIC_Public_Defines Public defines
  * @{
  */

#define MIC_SENSORS_MAX_NUM 1
/**
 * @}
 */

/** @addtogroup X_NUCLEO_IKS01A1_MIC_Public_Function_Prototypes Public function prototypes
 * @{
 */

/* Sensor Configuration Functions */
DrvStatusTypeDef BSP_MIC_Init( MIC_ID_t id );
//DrvStatusTypeDef BSP_MIC_DeInit( void **handle );
DrvStatusTypeDef BSP_MIC_Sensor_Enable( MIC_ID_t id );
DrvStatusTypeDef BSP_MIC_Sensor_Disable( void *handle );
DrvStatusTypeDef BSP_MIC_IsInitialized( void *handle, uint8_t *status );
//DrvStatusTypeDef BSP_MIC_IsEnabled( void *handle, uint8_t *status );
//DrvStatusTypeDef BSP_MIC_IsCombo( void *handle, uint8_t *status );
//DrvStatusTypeDef BSP_MIC_Get_Instance( void *handle, uint8_t *instance );
//DrvStatusTypeDef BSP_MIC_Get_WhoAmI( void *handle, uint8_t *who_am_i );
//DrvStatusTypeDef BSP_MIC_Check_WhoAmI( void *handle );
DrvStatusTypeDef BSP_MIC_Get_Noise( void *handle, uint8_t *noiseLevel);
//DrvStatusTypeDef BSP_MIC_Get_AxesRaw( void *handle, SensorAxesRaw_t *value );
//DrvStatusTypeDef BSP_MIC_Get_Sensitivity( void *handle, float *sensitivity );
//DrvStatusTypeDef BSP_MIC_Get_ODR( void *handle, float *odr );
//DrvStatusTypeDef BSP_MIC_Set_ODR( void *handle, SensorOdr_t odr );
//DrvStatusTypeDef BSP_MIC_Set_ODR_Value( void *handle, float odr );
//DrvStatusTypeDef BSP_MIC_Get_FS( void *handle, float *fullScale );
//DrvStatusTypeDef BSP_MIC_Set_FS( void *handle, SensorFs_t fullScale );
//DrvStatusTypeDef BSP_MIC_Set_FS_Value( void *handle, float fullScale );
//DrvStatusTypeDef BSP_MIC_Get_Axes_Status( void *handle, uint8_t *xyz_enabled );
//DrvStatusTypeDef BSP_MIC_Set_Axes_Status( void *handle, uint8_t *enable_xyz );
//DrvStatusTypeDef BSP_MIC_Read_Reg( void *handle, uint8_t reg, uint8_t *data );
//DrvStatusTypeDef BSP_MIC_Write_Reg( void *handle, uint8_t reg, uint8_t data );
//DrvStatusTypeDef BSP_MIC_Get_DRDY_Status( void *handle, uint8_t *status );
//
//DrvStatusTypeDef BSP_MIC_FIFO_Set_ODR_Value_Ext( void *handle, float odr );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Full_Status_Ext( void *handle, uint8_t *status );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Empty_Status_Ext( void *handle, uint8_t *status );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Overrun_Status_Ext( void *handle, uint8_t *status );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Pattern_Ext( void *handle, uint16_t *pattern );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Data_Ext( void *handle, uint8_t *aData );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Num_Of_Samples_Ext( void *handle, uint16_t *nSamples );
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Decimation_Ext( void *handle, uint8_t decimation );
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Axis_Ext( void *handle, int32_t *angular_velocity );
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Mode_Ext( void *handle, uint8_t mode );
//DrvStatusTypeDef BSP_MIC_FIFO_Set_INT1_FIFO_Full_Ext( void *handle, uint8_t status );
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Watermark_Level_Ext( void *handle, uint16_t watermark );
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Stop_On_Fth_Ext( void *handle, uint8_t status );
//
//DrvStatusTypeDef BSP_MIC_Set_Interrupt_Latch_Ext( void *handle, uint8_t status );
//DrvStatusTypeDef BSP_MIC_Set_SelfTest_Ext( void *handle, uint8_t status );

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

#endif /* __X_NUCLEO_IKS01A1_MIC_H */

/*****************************END OF FILE****/
