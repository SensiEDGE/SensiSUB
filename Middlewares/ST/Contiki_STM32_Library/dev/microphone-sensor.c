/**
******************************************************************************
* @file    microphone-sensor.c
* @author  
* @version V1.0.0
* @date    24-July-2017
* @brief   Enable microphone sensor functionality
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

/**
 * @defgroup microphone_sensor
 * @ingroup dev
 * @{
 */

/**
 * @addtogroup microphone_sensor
 * @ingroup dev
 * @{
 * @file Driver for the Microphone sensor (on expansion board)
 */

/*---------------------------------------------------------------------------*/
#if COMPILE_SENSORS
/*---------------------------------------------------------------------------*/
#include "lib/sensors.h"
#include "microphone-sensor.h"
#include "st-lib.h"
#include "x_nucleo_iks01a1_audio_l4.h"

/*---------------------------------------------------------------------------*/
static int _active = 0;
/*---------------------------------------------------------------------------*/


static void init(void)
{
  st_lib_bsp_audio_init(AUDIO_SAMPLING_FREQUENCY, AUDIO_BIT_RESOLUTION, 
                        AUDIO_CHANNELS);  
}
/*---------------------------------------------------------------------------*/
static void activate(void)
{
  st_lib_bsp_audio_record(PCM_Buffer, 0);
	_active=1;
}
/*---------------------------------------------------------------------------*/
static void deactivate(void)
{
  BSP_AUDIO_IN_Stop();
	_active=0;
}
/*---------------------------------------------------------------------------*/
static int active(void)
{
  return _active;
}
/*---------------------------------------------------------------------------*/
static int value(int channel)
{
  int32_t ret_val = 0;
  
  /* force wrong parameter to prevent receiving corrupted data */
  if(channel >= AUDIO_CHANNELS)
  {
      channel = 0;
  }
  
  ret_val = (int32_t) ((DBNOISE_Value_Old_Ch[channel] \
                      + DBNOISE_Value_Old_Ch[channel]) * 1000 / 2);

  return ret_val;
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
SENSORS_SENSOR(microphone_sensor, MICROPHONE_SENSOR, value, configure, status);
/*---------------------------------------------------------------------------*/
#endif /*COMPILE_SENSORS*/
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */
