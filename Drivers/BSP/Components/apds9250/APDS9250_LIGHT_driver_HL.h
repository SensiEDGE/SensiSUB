/**
 *******************************************************************************
 * @file    APDS9250_LIGHT_driver_HL.h
 * @author  Droid Technologies. Taras Chornenkyi
 * @date    5-January-2018
 * @brief   This file provides a set of high-level functions needed to manage
            the APDS9250 light sensor
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APDS_9250_LIGHT_DRIVER_HL_H
#define __APDS_9250_LIGHT_DRIVER_HL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "light.h"

/* Include magnetic sensor component drivers. */
#include "APDS9250_LIGHT_driver.h"


/**
 * @brief APDS9250 data structure definition
 */
typedef struct
{
    uint32_t Ir;
    uint32_t Grn;
    uint32_t Blue;
    uint32_t Red;
    uint32_t Light;
} APDS9250_Data_t;

/**
 * @}
 */

/** @addtogroup APDS9250_Public_Variables Public variables
 * @{
 */

extern LIGHT_Drv_t APDS9250Drv;

#ifdef __cplusplus
}
#endif

#endif /* __APDS_9250_LIGHT_DRIVER_HL_H */
