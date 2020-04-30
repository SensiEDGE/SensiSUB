#include "simba_lux.h"
#include "x_nucleo_iks01a1.h"
#include "component.h"

// I2C address
#define APDS9250_I2C_ADDRESS           (0x52 << 1)

// Register definitions
#define APDS9250_REG_MAIN_CTRL          0x00
#define APDS9250_REG_LS_MEAS_RATE       0x04
#define APDS9250_REG_LS_GAIN            0x05
#define APDS9250_REG_PART_ID            0x06
#define APDS9250_REG_MAIN_STATUS        0x07
#define APDS9250_REG_DATA_IR_0          0x0A
#define APDS9250_REG_DATA_IR_1          0x0B
#define APDS9250_REG_DATA_IR_2          0x0C
#define APDS9250_REG_DATA_GREEN_0       0x0D
#define APDS9250_REG_DATA_GREEN_1       0x0E
#define APDS9250_REG_DATA_GREEN_2       0x0F
#define APDS9250_REG_DATA_BLUE_0        0x10
#define APDS9250_REG_DATA_BLUE_1        0x11
#define APDS9250_REG_DATA_BLUE_2        0x12
#define APDS9250_REG_DATA_RED_0         0x13
#define APDS9250_REG_DATA_RED_1         0x14
#define APDS9250_REG_DATA_RED_2         0x15
#define APDS9250_REG_INT_CFG            0x19
#define APDS9250_REG_INT_PERSISTENCE    0x1A
#define APDS9250_REG_THRES_UP_0         0x21
#define APDS9250_REG_THRES_UP_1         0x22
#define APDS9250_REG_THRES_UP_2         0x23
#define APDS9250_REG_THRES_LOW_0        0x24
#define APDS9250_REG_THRES_LOW_1        0x25
#define APDS9250_REG_THRES_LOW_2        0x26
#define APDS9250_REG_THRES_VAR          0x27

// Register values
#define APDS9250_PART_ID                0xB2


static DrvContextTypeDef LuxHandle;
static uint8_t LuxInitialized = 0;

static uint8_t APDS9250_IO_Init(void);
static uint8_t APDS9250_IO_Read( void* handle, uint8_t reg, uint8_t *buf, uint16_t count);
static uint8_t APDS9250_IO_Write(void* handle, uint8_t reg, uint8_t *buf, uint16_t count);
static uint8_t BSP_LUX_Init(void);
static uint8_t BSP_LUX_IsInitalized(void);
static uint8_t BSP_LUX_PowerON(void);
static uint8_t BSP_LUX_PowerOFF(void);
static uint8_t BSP_LUX_IsDataReady(void);
static uint8_t BSP_LUX_GetValue(uint16_t *pData);
static uint8_t APDS9250_IO_Init(void);

extern uint8_t Sensor_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
extern uint8_t Sensor_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );

DrvStatusTypeDef BSP_LIGHT_Init( LIGHT_ID_t id, void **handle )
{
  LuxHandle.address = APDS9250_I2C_ADDRESS;
  handle = (void**)&LuxHandle;
  if(0 == BSP_LUX_Init()) {
    return COMPONENT_OK;
  }
  
  return COMPONENT_ERROR;
}

DrvStatusTypeDef BSP_LIGHT_DeInit( void **handle )
{
  BSP_LUX_PowerOFF();
  return COMPONENT_OK;
}

DrvStatusTypeDef BSP_LIGHT_Sensor_Enable( void *handle )
{
  BSP_LUX_PowerON();
  return COMPONENT_OK;
}
DrvStatusTypeDef BSP_LIGHT_Sensor_Disable( void *handle )
{
  BSP_LUX_PowerOFF();
  return COMPONENT_OK;
}
DrvStatusTypeDef BSP_LIGHT_IsInitialized( void *handle, uint8_t *status )
{
  *status = 0 != BSP_LUX_IsInitalized();
  return COMPONENT_OK;
}
DrvStatusTypeDef BSP_LIGHT_IsEnabled( void *handle, uint8_t *status )
{
  // Perhaps, this is an issue since initialized doesn't mean enabled
  *status = 0 != BSP_LUX_IsInitalized();
  return COMPONENT_OK;
}
DrvStatusTypeDef BSP_LIGHT_Get_Light( void *handle, uint16_t *light )
{
  if(0 == BSP_LUX_GetValue(light)) {
    return COMPONENT_OK;
  }
  
  return COMPONENT_ERROR;
}
DrvStatusTypeDef BSP_LIGHT_Get_DRDY_Status( void *handle, uint8_t *status )
{
  *status = BSP_LUX_IsDataReady();
  return COMPONENT_OK;
}

uint8_t BSP_LUX_Init(void)
{
    if (!LuxInitialized)
    {
        uint8_t gain = 0x01; // gain = 3;
        uint8_t meas_rate = 0x22; // 18bit, 100 mS

        if (APDS9250_IO_Init() != 0)
        {
            return 1;
        }

        // Set gain
        if (APDS9250_IO_Write((void*)&LuxHandle, APDS9250_REG_LS_GAIN, &gain, 1) != 0)
        {
            return 1;
        }
        // Set measurement rate
        if (APDS9250_IO_Write((void*)&LuxHandle, APDS9250_REG_LS_MEAS_RATE, &meas_rate, 1) != 0)
        {
            return 1;
        }
        LuxInitialized = 1;
    }

    return 0;
}


uint8_t BSP_LUX_IsInitalized(void)
{
    return LuxInitialized;
}


uint8_t BSP_LUX_PowerON(void)
{
    uint8_t main_ctrl = 0x02; // ALS, active
    //uint8_t main_ctrl = 0x06; // RGB, active
    return APDS9250_IO_Write((void*)&LuxHandle, APDS9250_REG_MAIN_CTRL, &main_ctrl, 1);
}


uint8_t BSP_LUX_PowerOFF(void)
{
    uint8_t main_ctrl = 0;
    return APDS9250_IO_Write((void*)&LuxHandle, APDS9250_REG_MAIN_CTRL, &main_ctrl, 1);
}


uint8_t BSP_LUX_IsDataReady(void)
{
    uint8_t ret;
    uint8_t main_status;

    ret = APDS9250_IO_Read((void*)&LuxHandle, APDS9250_REG_MAIN_STATUS, &main_status, 1);
    if (ret != 0)
    {
        return 0;
    }

    return (main_status & 0x08) ? 1 : 0;
}


uint8_t BSP_LUX_GetValue(uint16_t *pData)
{
    uint8_t ret;
    uint8_t adc_data[6];

    ret = APDS9250_IO_Read((void*)&LuxHandle, APDS9250_REG_DATA_IR_0, adc_data, 6);
    if (ret == 0)
    {
        uint32_t ir_value    = adc_data[0] | (adc_data[1] << 8) | (adc_data[2] << 16);
        uint32_t green_value = adc_data[3] | (adc_data[4] << 8) | (adc_data[5] << 16);

        uint32_t factor = ir_value > green_value ? 35 : 46;

        uint32_t lux = ((green_value * factor) / 3) / 100;
        *pData = (uint16_t)lux;
    }

    return ret;
}



uint8_t APDS9250_IO_Init(void)
{
    // // Init I2C
    // if(I2C_Global_Init() != HAL_OK)
    // {
    //     return 1;
    // }
    return 0;
}


static uint8_t APDS9250_IO_Read(void* handle, uint8_t reg, uint8_t *buf, uint16_t count)
{
    uint8_t ret_val = 0;

    
    if (COMPONENT_OK != Sensor_IO_Read(handle, reg, buf, count))
    {
        ret_val = 1;
    }

    return ret_val;
}


static uint8_t APDS9250_IO_Write(void* handle, uint8_t reg, uint8_t *buf, uint16_t count)
{
    uint8_t ret_val = 0;

    if (COMPONENT_OK != Sensor_IO_Write(handle, reg, buf, count))
    {
        ret_val = 1;
    }

    return ret_val;
}
