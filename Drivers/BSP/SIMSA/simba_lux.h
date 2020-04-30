#ifndef __SIMBA_LUX_H__
#define __SIMBA_LUX_H__

#include <stdint.h>
#include "x_nucleo_iks01a1.h"

typedef enum {
  LIGHT_SENSORS_AUTO = -1
} LIGHT_ID_t;


DrvStatusTypeDef BSP_LIGHT_Init( LIGHT_ID_t id, void **handle );
DrvStatusTypeDef BSP_LIGHT_DeInit( void **handle );
DrvStatusTypeDef BSP_LIGHT_Sensor_Enable( void *handle );
DrvStatusTypeDef BSP_LIGHT_Sensor_Disable( void *handle );
DrvStatusTypeDef BSP_LIGHT_IsInitialized( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_LIGHT_IsEnabled( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_LIGHT_Get_Light( void *handle, uint16_t *ultraviolet );
DrvStatusTypeDef BSP_LIGHT_Read_Reg( void *handle, uint8_t reg, uint8_t *data );
DrvStatusTypeDef BSP_LIGHT_Write_Reg( void *handle, uint8_t reg, uint8_t data );
DrvStatusTypeDef BSP_LIGHT_Get_DRDY_Status( void *handle, uint8_t *status );


#endif //__SIMBA_LUX_H__
