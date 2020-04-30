/**
 *******************************************************************************
 * @file    light.h
 * @author  Droid Technologies. Taras Chornenkyi
 * @date    5-January-2018
 * @brief   This file provides a set of high-level functions needed to manage
            the APDS9250 light sensor
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIGHT_H
#define __LIGHT_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include "sensor.h"

/**
 * @brief  LIGHT driver structure definition
 */
typedef struct
{
  DrvStatusTypeDef ( *Init            ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *DeInit          ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Sensor_Enable   ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Sensor_Disable  ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_WhoAmI      ) ( DrvContextTypeDef*, uint8_t* );
  DrvStatusTypeDef ( *Check_WhoAmI    ) ( DrvContextTypeDef* );
  DrvStatusTypeDef ( *Get_Ir          ) ( DrvContextTypeDef*, uint32_t* );
  DrvStatusTypeDef ( *Get_Grn         ) ( DrvContextTypeDef*, uint32_t* );
  DrvStatusTypeDef ( *Get_Blue        ) ( DrvContextTypeDef*, uint32_t* );
  DrvStatusTypeDef ( *Get_Red         ) ( DrvContextTypeDef*, uint32_t* );
  DrvStatusTypeDef ( *Get_Light       ) ( DrvContextTypeDef*, uint32_t* );
  DrvStatusTypeDef ( *Get_Fs          ) ( DrvContextTypeDef*, float *);
  DrvStatusTypeDef ( *Set_Fs          ) ( DrvContextTypeDef*, SensorFs_t );
} LIGHT_Drv_t;

/**
 * @brief  ULTRAVIOLET data structure definition
 */
typedef struct
{
  void *pComponentData; /* Component specific data. */
  void *pExtData;       /* Other data. */
} LIGHT_Data_t;

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

#endif /* __ULTRAVIOLET_H */

/***************************************************************END OF FILE****/
