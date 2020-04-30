/**
 *******************************************************************************
 * @file    APDS9250_LIGHT_driver_HL.c
 * @author  Droid Technologies. Taras Chornenkyi
 * @date    5-January-2018
 * @brief   This file provides a set of high-level functions needed to manage
            the APDS9250 light sensor
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "APDS9250_LIGHT_driver.h"

extern uint8_t Sensor_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
extern uint8_t Sensor_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );


/**
 * @}
 */

APDS9250_Error_et APDS9250_ReadReg( void *handle, uint8_t RegAddr, uint16_t NumByteToRead, uint8_t *Data )
{
  if ( Sensor_IO_Read( handle, RegAddr, Data, NumByteToRead ) )
    return APDS9250_ERROR;
  else
    return APDS9250_OK;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_WriteReg( void *handle, uint8_t RegAddr, uint16_t NumByteToWrite, uint8_t *Data )
{
  if ( Sensor_IO_Write( handle, RegAddr, Data, NumByteToWrite ) )
    return APDS9250_ERROR;
  else
    return APDS9250_OK;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_SensorEnable( void *handle )
{
    return APDS9250_ERROR;
}

/**
 * @}
 */
APDS9250_Error_et APDS9250_SensorDisable( void *handle )
{
    return APDS9250_ERROR;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_GetWhoAmI( void *handle, uint8_t *who_am_i )
{
    if(APDS9250_OK == APDS9250_ReadReg(handle, APDS9250_REG_PART_ID, 1, who_am_i)) {
        return APDS9250_OK;
    }
    return APDS9250_ERROR;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_GetIr         (void *handle, uint32_t *value)
{
    APDS9250_Error_et ret;
    uint8_t adc_data[3];

    ret = APDS9250_ReadReg(handle, APDS9250_REG_DATA_IR_0, sizeof(adc_data), adc_data);
    
    if (ret == APDS9250_OK)
    {
        *value = adc_data[0] | (adc_data[1] << 8) | (adc_data[2] << 16);
    }
    
    return ret;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_GetGrn        (void *handle, uint32_t *value)
{
    APDS9250_Error_et ret;
    uint8_t adc_data[3];

    ret = APDS9250_ReadReg(handle, APDS9250_REG_DATA_GREEN_0, sizeof(adc_data), adc_data);
    
    if (ret == APDS9250_OK)
    {
        *value = adc_data[0] | (adc_data[1] << 8) | (adc_data[2] << 16);
    }
    
    return ret;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_GetBlue       (void *handle, uint32_t *value)
{
    APDS9250_Error_et ret;
    uint8_t adc_data[3];

    ret = APDS9250_ReadReg(handle, APDS9250_REG_DATA_RED_0, sizeof(adc_data), adc_data);
    
    if (ret == APDS9250_OK)
    {
        *value = adc_data[0] | (adc_data[1] << 8) | (adc_data[2] << 16);
    }
    
    return ret;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_GetRed        (void *handle, uint32_t *value)
{
    APDS9250_Error_et ret;
    uint8_t adc_data[3];

    ret = APDS9250_ReadReg(handle, APDS9250_REG_DATA_RED_0, sizeof(adc_data), adc_data);
    
    if (ret == APDS9250_OK)
    {
        *value = adc_data[0] | (adc_data[1] << 8) | (adc_data[2] << 16);
    }
    
    return ret;
}

/**
 * @}
 */

APDS9250_Error_et APDS9250_GetLight      (void *handle, uint32_t *value)
{
    APDS9250_Error_et ret;
    uint8_t adc_data[6];

    ret = APDS9250_ReadReg(handle, APDS9250_REG_DATA_IR_0, sizeof(adc_data), adc_data);
    
    if (ret == APDS9250_OK)
    {
        uint32_t ir_value    = adc_data[0] | (adc_data[1] << 8) | (adc_data[2] << 16);
        uint32_t green_value = adc_data[3] | (adc_data[4] << 8) | (adc_data[5] << 16);

        uint32_t factor = ir_value > green_value ? 35 : 46;

        uint32_t lux = ((green_value * factor) / 3) / 100;
        *value = (uint16_t)lux;
    }
    
    return ret;
}
