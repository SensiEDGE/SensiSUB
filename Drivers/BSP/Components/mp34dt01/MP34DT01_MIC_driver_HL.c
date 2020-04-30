/**
 ******************************************************************************
 * @file    MP34DT01_MIC_driver_HL.c
 * @author  
 * @version V1.0.0
 * @date    24-July-2017
 * @brief   This file provides a set of high-level functions needed to manage
            the MP34DT01 microphone
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

/* Includes ------------------------------------------------------------------*/

#include "MP34DT01_MIC_driver_HL.h"


//DrvStatusTypeDef BSP_MIC_Init( MIC_ID_t id, void **handle )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_DeInit( void **handle )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Sensor_Enable( void *handle )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Sensor_Disable( void *handle )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_IsInitialized( void *handle, uint8_t *status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_IsEnabled( void *handle, uint8_t *status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_IsCombo( void *handle, uint8_t *status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_Instance( void *handle, uint8_t *instance )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_WhoAmI( void *handle, uint8_t *who_am_i )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Check_WhoAmI( void *handle )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_Noise( void *handle, SensorNoise_t *absolute_noise )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_Sensitivity( void *handle, float *sensitivity )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_ODR( void *handle, float *odr )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Set_ODR( void *handle, SensorOdr_t odr )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Set_ODR_Value( void *handle, float odr )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_FS( void *handle, float *fullScale )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Set_FS( void *handle, SensorFs_t fullScale )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Set_FS_Value( void *handle, float fullScale )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Read_Reg( void *handle, uint8_t reg, uint8_t *data )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Write_Reg( void *handle, uint8_t reg, uint8_t data )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Get_DRDY_Status( void *handle, uint8_t *status )
//{
//  return 0;
//}
//
//DrvStatusTypeDef BSP_MIC_FIFO_Set_ODR_Value_Ext( void *handle, float odr )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Full_Status_Ext( void *handle, uint8_t *status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Empty_Status_Ext( void *handle, uint8_t *status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Overrun_Status_Ext( void *handle, uint8_t *status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Pattern_Ext( void *handle, uint16_t *pattern )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Data_Ext( void *handle, uint8_t *aData )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Num_Of_Samples_Ext( void *handle, uint16_t *nSamples )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Decimation_Ext( void *handle, uint8_t decimation )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Get_Axis_Ext( void *handle, int32_t *angular_velocity )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Mode_Ext( void *handle, uint8_t mode )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Set_INT1_FIFO_Full_Ext( void *handle, uint8_t status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Watermark_Level_Ext( void *handle, uint16_t watermark )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_FIFO_Set_Stop_On_Fth_Ext( void *handle, uint8_t status )
//{
//  return 0;
//}
//
//DrvStatusTypeDef BSP_MIC_Set_Interrupt_Latch_Ext( void *handle, uint8_t status )
//{
//  return 0;
//}
//DrvStatusTypeDef BSP_MIC_Set_SelfTest_Ext( void *handle, uint8_t status )
//{
//  return 0;
//}
