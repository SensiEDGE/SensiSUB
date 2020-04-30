/**
******************************************************************************
* @file    ultraviolet-sensor.c
* @author  
* @version V1.0.0
* @date    08-August-2017
* @brief   Enable UV sensor functionality 
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
 * @defgroup ultraviolet_sensor
 * @ingroup dev
 * @{
 */

/**
 * @addtogroup ultraviolet_sensor
 * @ingroup dev
 * @{
 * @file Driver for the UV sensor (on expansion board)
 */
/*---------------------------------------------------------------------------*/
#if COMPILE_SENSORS
/*---------------------------------------------------------------------------*/
#include "lib/sensors.h"
#include "ultraviolet-sensor.h"
#include "st-lib.h"
/*---------------------------------------------------------------------------*/
static int _active = 0;
static void *ultraviolet_handle = NULL;
/*---------------------------------------------------------------------------*/
static void init(void)
{
#ifdef USE_IKS01A2
  if (COMPONENT_ERROR == st_lib_bsp_ultraviolet_init(ULTRAVIOLET_SENSORS_AUTO, 
                                                     &ultraviolet_handle)){
	  printf("ERROR initializing UV Sensor\n");
  }
#else
  if (COMPONENT_ERROR == st_lib_bsp_ultraviolet_init(VEML6075_0, 
                                                     &ultraviolet_handle)){
	  printf("ERROR initializing Humidity Sensor\n");
  }
#endif
}
/*---------------------------------------------------------------------------*/
static void activate(void)
{
  if (st_lib_bsp_ultraviolet_sensor_enable(ultraviolet_handle) ==COMPONENT_OK){
    _active=1;
  }
}
/*---------------------------------------------------------------------------*/
static void deactivate(void)
{
  if (st_lib_bsp_ultraviolet_sensor_disable(ultraviolet_handle) ==COMPONENT_OK){
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
  uint32_t ultraviolet;
  volatile uint16_t ultraviolet_value;
  uint8_t status = 0;
  
  if (st_lib_bsp_ultraviolet_is_initialized(ultraviolet_handle, &status) == COMPONENT_OK
  		&& status == 1){
    st_lib_bsp_ultraviolet_get_uv(ultraviolet_handle, (uint16_t *) &ultraviolet_value);
  }
  ultraviolet = ultraviolet_value;
  return ultraviolet;
}
/*---------------------------------------------------------------------------*/
static int configure(int type, int value)
{
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
SENSORS_SENSOR(ultraviolet_sensor, ULTRAVIOLET_SENSOR, value, configure, status);
/*---------------------------------------------------------------------------*/
#endif /*COMPILE_SENSORS*/
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */
