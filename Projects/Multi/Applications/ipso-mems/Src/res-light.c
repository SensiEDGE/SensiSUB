/**
******************************************************************************
  * @file    res-light.c
  * @author  
  * @version V1.0.0
  * @date    10-August-2017
  * @brief   light resource
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
 * \file
 *         light sensor
 */

#include "ipso-objects.h"
#include "dev/light-sensor.h"
#include "dev/sensor-common.h"
#include "sensors.h"


/** @addtogroup LWM2M_example
  * @{
  */

/*---------------------------------------------------------------------------*/
static int
read_value(int32_t *value)
{
  *value = light_sensor.value(0);

  return 0;
}


static void
init_sensor(void)
{
	SENSORS_ACTIVATE(light_sensor);
}
/*---------------------------------------------------------------------------*/
const struct ipso_objects_sensor res_light = {
  .read_value = read_value,
  .init = init_sensor
};
/*---------------------------------------------------------------------------*/

/**
 * @}
 */
