/**
******************************************************************************
* @file    light-sensor.c
* @author  
* @version V1.0.0
* @date    13-July-2018
* @brief   Enable light sensor functionality 
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
/**
 * @defgroup light_sensor
 * @ingroup dev
 * @{
 */

/**
 * @addtogroup light_sensor
 * @ingroup dev
 * @{
 * @file Driver for the UV sensor (on expansion board)
 */
/*---------------------------------------------------------------------------*/
#if COMPILE_SENSORS
/*---------------------------------------------------------------------------*/
#include "lib/sensors.h"
#include "light-sensor.h"
#include "st-lib.h"
/*---------------------------------------------------------------------------*/
static int _active = 0;
static void *light_handle = NULL;
/*---------------------------------------------------------------------------*/
static void init(void)
{
  if (COMPONENT_ERROR == st_lib_bsp_light_init(LIGHT_SENSORS_AUTO, 
                                                     &light_handle)){
	  printf("ERROR initializing light Sensor\n");
  }
}
/*---------------------------------------------------------------------------*/
static void activate(void)
{
  if (st_lib_bsp_light_sensor_enable(light_handle) ==COMPONENT_OK){
    _active=1;
  }
}
/*---------------------------------------------------------------------------*/
static void deactivate(void)
{
  if (st_lib_bsp_light_sensor_disable(light_handle) ==COMPONENT_OK){
    _active=0;
  }
}
/*---------------------------------------------------------------------------*/
static int active(void)
{
  return _active;
}
/*---------------------------------------------------------------------------*/
static int value(int type)
{
  uint32_t light;
  volatile uint16_t light_value = 0xFFFF;
  uint8_t status = 0;
  
  if (st_lib_bsp_light_is_initialized(light_handle, &status) == COMPONENT_OK
  		&& status == 1){
    st_lib_bsp_light_get_light(light_handle, (uint16_t *) &light_value);
    // printf(">>>>>> Light sensor value is %u\n", light_value);
  }
  light = light_value;
  return light;
}
/*---------------------------------------------------------------------------*/
static int configure(int type, int value)
{
  init();
  switch(type) {
    case SENSORS_HW_INIT:
      init();
      return 1;
    case SENSORS_ACTIVE:
      if(value) {      
        activate();
      } else {
        deactivate();
      }
      return 1;
  }
 
  return 0;
}
/*---------------------------------------------------------------------------*/
static int status(int type)
{
  switch(type) {
    case SENSORS_READY:
      return active();
  }
  
  return 0;
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(light_sensor, LIGHT_SENSOR, value, configure, status);
/*---------------------------------------------------------------------------*/
#endif /*COMPILE_SENSORS*/
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */
