/**
******************************************************************************
  * @file    my_vl6180x.c
  * @author  Central LAB
  * @version V1.0.0
  * @date    20-January-2016
  * @brief   Main functions for vl6180x sensor
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

#include "cube_hal.h"
#include "x-nucleo-6180xa1.h"
#include "vl6180x_platform.h"
#include "my_vl6180x.h"

/** @addtogroup LWM2M_example
  * @{
  */
/*---------------------------------------------------------------------------*/
int V2_ExpanderRd(int, uint8_t *, int);
int V2_ExpanderWR(int, uint8_t *, int);
/*---------------------------------------------------------------------------*/
#define I2C_BUS      (&I2CHandle)
#define I2C_TIME_OUT 100
/*---------------------------------------------------------------------------*/
#define SYS_CTRL 0x03 //This is the SYS_CTRL register of the STMPE1600 expander
/*In order to shut down I2C interface of the display we need to set bit 6 of the SYS_CTRL  register
to 1 (I2C_SHDN), so we need a mask of 01000000 and then perform a bitwise OR*/

I2C_HandleTypeDef I2CHandle;

//struct MyVL6180Dev_t BoardDevs[4] = { [0]= { .DevID = 0 }, [1]= { .DevID = 1 }, [2]= { .DevID = 2 }, [3]= { .DevID = 3 }, };
struct MyVL6180Dev_t BoardDevs[1] = { [0]= { .DevID = 0 }};

int nPresentDevs=0; /* Total Number of discovered VL6180X satellites */

//int PresentDevIds[4];
//enum measure_t MeasureRunning[4] = {NONE,NONE,NONE,NONE};
int PresentDevIds[1];
measure_t MeasureRunning[1] = {NONE};

static void Disable_I2C_Expander_Interface(void);

#if !PRESENCE_PERIODIC
/**
 * @brief This function initializes one VL6180X  device for reading Lux or Distance
 * @param enum measure_t measure type of measure that we want: RANGE_ONLY/ALS_ONLY/RANGE_ALS
 * @param uint32_t Sn device number
 * @retval None
 */
void Kickoff_VL6180X_ProximitySen(measure_t measure,uint32_t Sn)
{
  int DevIds=PresentDevIds[Sn];
  VL6180xDev_t dev =  BoardDevs +DevIds;
  int FinalI2cAddr;
  int status;

  /* put all under reset */
  XNUCLEO6180XA1_ResetId(0, DevIds);
  HAL_Delay(1);

  /* unreset device that wake up at default i2c addres 0x52 */
  XNUCLEO6180XA1_ResetId(1, DevIds);
  HAL_Delay(2);    /* at least 400usec before to acces device */
  dev->I2cAddr = 0x52;
  status = VL6180x_InitData(dev);
  if( status ){
      printf("VL6180x_InitData fails\r\n");
  }
  FinalI2cAddr = 0x52 + ((DevIds+1) * 2);
  status = VL6180x_SetI2CAddress(dev, FinalI2cAddr);
  dev->I2cAddr = FinalI2cAddr;
  if( status ){
      printf("VL6180x_SetI2CAddress fails\r\n");
  }
  HAL_Delay(1);
  status= VL6180x_Prepare(dev);
  if( status<0 ){
      printf("VL6180x_Prepare fails\r\n");
  }

  /* kick off measure on all device */
  switch (measure) {
    case RANGE_ONLY:
    	VL6180x_RangeSetMaxConvergenceTime(dev, 50);
		VL6180x_RangeSetInterMeasPeriod(dev, 100);

		VL6180x_UpscaleSetScaling(dev, 3);


		VL6180x_SetupGPIO1(dev, GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT, INTR_POL_HIGH);

		/* make sure from now on all register in group are not fetched by device */
		VL6180x_SetGroupParamHold(dev, 1);

		/* get interrupt whenever we go out of the window */
		VL6180x_RangeSetThresholds(dev, PRESENCE_LOW_TH, PRESENCE_HIGH_TH, 0 );
		/* set range interrupt reporting low threshold*/
		VL6180x_RangeConfigInterrupt(dev, CONFIG_GPIO_INTERRUPT_OUT_OF_WINDOW);

		/* leave device peak up all new register in group */
		VL6180x_SetGroupParamHold(dev, 0);

		/* clear any interrupt that should ensure a new edge get generated even if we missed it */
		VL6180x_RangeClearInterrupt(dev);

		XNUCLEO6180XA1_EnableInterrupt();
		/*Clear any pending device interrupt even if already active it should force a new edge so we can pick up*/
		VL6180x_ClearAllInterrupt(dev);

		VL6180x_RangeSetSystemMode(dev, MODE_START_STOP|MODE_CONTINUOUS);


      break;
    case ALS_ONLY:
      //status = VL6180x_AlsSetSystemMode(dev, MODE_START_STOP | MODE_CONTINUOUS);
      break;
    case RANGE_ALS:
      //status = VL6180x_RangeALSStartContinuousInterleavedMode(dev);
      break;
    default:
      printf("Error VL6180x_RangeStartContinuousMode fail S%ld", Sn);
      status = -1;
  }

  if( status<0 ){
    printf("Error VL6180x_RangeStartContinuousMode fail S%ld\t\n", Sn);
    dev->Ready=0;
    printf("Device S%ld NotReady\r\n",Sn);
  } else {
    dev->Ready=1;
    MeasureRunning[Sn] = RANGE_ONLY;
    printf("Device S%ld Ready\r\n",Sn);
  }
}
#else
/**
 * @brief This function initializes one VL6180X  device for reading Lux or Distance
 * @param enum measure_t measure type of measure that we want: RANGE_ONLY/ALS_ONLY/RANGE_ALS
 * @param uint32_t Sn device number
 * @retval None
 */
void Kickoff_VL6180X_ProximitySen(measure_t measure,uint32_t Sn)
{
  int DevIds=PresentDevIds[Sn];
  VL6180xDev_t dev =  BoardDevs +DevIds;
  int FinalI2cAddr;
  int status;

  /* put all under reset */
  XNUCLEO6180XA1_ResetId(0, DevIds);
  HAL_Delay(1);

  /* unreset device that wake up at default i2c addres 0x52 */
  XNUCLEO6180XA1_ResetId(1, DevIds);
  HAL_Delay(2);    /* at least 400usec before to acces device */
  dev->I2cAddr = 0x52;
  status = VL6180x_InitData(dev);
  if( status ){
      printf("VL6180x_InitData fails\r\n");
  }
  FinalI2cAddr = 0x52 + ((DevIds+1) * 2);
  status = VL6180x_SetI2CAddress(dev, FinalI2cAddr);
  dev->I2cAddr = FinalI2cAddr;
  if( status ){
      printf("VL6180x_SetI2CAddress fails\r\n");
  }
  HAL_Delay(1);
  status= VL6180x_Prepare(dev);
  if( status<0 ){
      printf("VL6180x_Prepare fails\r\n");
  }

  /* kick off measure on all device */
  switch (measure) {
    case RANGE_ONLY:
    	VL6180x_RangeSetMaxConvergenceTime(dev, 50);
		VL6180x_RangeSetInterMeasPeriod(dev, 100);

		status = VL6180x_RangeStartContinuousMode(dev);
      break;
    case ALS_ONLY:
      //status = VL6180x_AlsSetSystemMode(dev, MODE_START_STOP | MODE_CONTINUOUS);
      break;
    case RANGE_ALS:
      //status = VL6180x_RangeALSStartContinuousInterleavedMode(dev);
      break;
    default:
      printf("Error VL6180x_RangeStartContinuousMode fail S%ld", Sn);
      status = -1;
  }

  if( status<0 ){
    printf("Error VL6180x_RangeStartContinuousMode fail S%ld\t\n", Sn);
    dev->Ready=0;
    printf("Device S%ld NotReady\r\n",Sn);
  } else {
    dev->Ready=1;
    MeasureRunning[Sn] = RANGE_ONLY;
    printf("Device S%ld Ready\r\n",Sn);
  }
}

#endif
/*---------------------------------------------------------------------------*/
/**
 * @brief This function Initializes the X-NUCLEO-618XA1 expansion understanding
 * if it's a Version 1 or 2, and the number of attached satellites
 * @param None
 * @retval None
 */
void Init_VL6180X_ProximitySens(void)
{
  int status;

  /* Initialize VL6180x proximity sensor and discovery the satellites */
  int n_dev=0;
  int i;
  XNUCLEO6180XA1_I2C1_Init(&I2CHandle);

  if (!XNUCLEO6180XA1_IsV2()) {
     printf ("V2 shield is required\n");
     n_dev=1;
     /* put device under reset */
     XNUCLEO6180XA1_ResetId(0, 0);
  } else {
     n_dev = 1;
     /* put all devices under reset */
     for (i = 0; i < n_dev; i++) {
       /* put all under reset */
       XNUCLEO6180XA1_ResetId(0, i);
    }
  }
  /* detect presence and initialise devices i2c address  */
  /*set device i2c address for dev[i] = 0x52+(i+1)*2 */
  for (i = n_dev - 1; i >= 0; i--) {
    int FinalI2cAddr;
    uint8_t id;
    /* unreset device that wake up at default i2c addres 0x52 */
    HAL_Delay(1);
    XNUCLEO6180XA1_ResetId(1, i);
    HAL_Delay(2);
    BoardDevs[i].I2cAddr = 0x52;
    /* to detect device presence try to read it's dev id */

    status = VL6180x_RdByte(&BoardDevs[i], IDENTIFICATION_MODEL_ID, &id);
    if (status) {
        /* these device is not presnt skip init and clear it's letter on string */
        continue;
    }
    /* device present */
    PresentDevIds[nPresentDevs]=i;
    nPresentDevs++;
    status = VL6180x_InitData(&BoardDevs[i]);
    if( status ){
        printf("VL6180x_InitData fails\r\n");
    }
    FinalI2cAddr = 0x52 + ((i+1) * 2);
    status = VL6180x_SetI2CAddress(&BoardDevs[i], FinalI2cAddr);
    if( status ){
        printf("VL6180x_SetI2CAddress fails\r\n");
    }
    BoardDevs[i].I2cAddr = FinalI2cAddr;

    status= VL6180x_Prepare(&BoardDevs[i]);
    if( status<0 ){
        printf("VL6180x_Prepare fails device=%d\r\n",i);
    } else {
      printf("VL6180x_Prepare ok device=%d\r\n",i);
    }

    //Disable_I2C_Expander_Interface();

  }
}
/*---------------------------------------------------------------------------*/

/**
 * @brief This function reads the proximity value of one VL6180X  device
 * @param uint32_t Sn device number
 * @retval uint16_t distance Value read
 */
uint16_t RdFligthSense_Distance (uint32_t Sn)
{
  int status;
  VL6180x_RangeData_t Range;
  VL6180xDev_t dev;
  //static uint16_t prevRangeMeasure[4] = {510,510,510,510};
  static uint16_t prevRangeMeasure[1] = {510};


  if (MeasureRunning[Sn] != RANGE_ONLY ) {
    Kickoff_VL6180X_ProximitySen(RANGE_ONLY,Sn);
    MeasureRunning[Sn] = RANGE_ONLY;
  }

  dev =  BoardDevs + PresentDevIds[Sn];
  if( !dev->Ready ){
    printf ("ERROR RdFligthSense_Distance device %ld not ready\n\r", Sn);
    return prevRangeMeasure[Sn];
  }

  status = VL6180x_RangeGetMeasurement(dev, &Range);

  if( status == 0 ){
    prevRangeMeasure[Sn] = Range.range_mm;
#ifdef DISPLAY_MEASURE_ENABLED
    sprintf (str, "%-3d", prevRangeMeasure[Sn]);
#endif /* DISPLAY_MEASURE_ENABLED */
    return (uint16_t)Range.range_mm;
  }
  else if( status <0 ){
    printf("S%ld  VL6180x_AlsGetMeasurement fail",Sn);
    return prevRangeMeasure[Sn];
  }
  return prevRangeMeasure[Sn];
}
/*---------------------------------------------------------------------------*/

/**
 * VL6180x CubeMX F401 multiple device i2c implementation
 */

int VL6180x_I2CWrite(VL6180xDev_t dev, uint8_t *buff, uint8_t len) {
    int status;
    status = HAL_I2C_Master_Transmit(I2C_BUS, dev->I2cAddr, buff, len, I2C_TIME_OUT);

    return status? -1 : 0;
}
/*---------------------------------------------------------------------------*/
int VL6180x_I2CRead(VL6180xDev_t dev, uint8_t *buff, uint8_t len) {
    int status;
    status = HAL_I2C_Master_Receive(I2C_BUS, dev->I2cAddr, buff, len, I2C_TIME_OUT);

    return status? -1 : 0;
}
/*---------------------------------------------------------------------------*/
void Flight_Clear_Int(void)
{
	  VL6180xDev_t dev;

	  dev =  BoardDevs + PresentDevIds[0];

	  VL6180x_ClearAllInterrupt(dev);
}
/*---------------------------------------------------------------------------*/
static void Disable_I2C_Expander_Interface(void)
{
	uint8_t reg_value;
	uint8_t mask = 1<<6; //shift 6 positions since I2C_SHDN is the 7th bit

	//first, read expander
	V2_ExpanderRd(SYS_CTRL, &reg_value, 1);

	//then mask it with 01000000
	reg_value = reg_value | mask;

	//write the register
	V2_ExpanderWR(SYS_CTRL, &reg_value, 1);
}
/*---------------------------------------------------------------------------*/


/**
  * @}
  */

