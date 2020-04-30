/**
******************************************************************************
  * @file    ipso-light.c
  * @author  
  * @version V1.0.0
  * @date    08-August-2017
  * @brief   IPSO Light Object
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
 * \addtogroup ipso-objects
 * @{
 */

/**
 * \file
 *         Implementation of OMA LWM2M / IPSO Light
 *         
 */

#include <stdint.h>
#include "ipso-objects.h"
#include "lwm2m-object.h"
#include "lwm2m-engine.h"
#include "er-coap-engine.h"
/*---------------------------------------------------------------------------*/
#define LIGHT_NOTIFICATION_THRESH 1 // 1000
#define LIGHT_TIMER               1 // 60 sec
/*---------------------------------------------------------------------------*/
#ifdef IPSO_LIGHT
extern const struct ipso_objects_sensor IPSO_LIGHT;
#endif /* IPSO_LIGHT */
/*---------------------------------------------------------------------------*/
#ifndef IPSO_LIGHT_MIN
#define IPSO_LIGHT_MIN (1 * LWM2M_FLOAT32_FRAC)
#endif
/*---------------------------------------------------------------------------*/
#ifndef IPSO_LIGHT_MAX
#define IPSO_LIGHT_MAX (100 * LWM2M_FLOAT32_FRAC)
#endif
/*---------------------------------------------------------------------------*/
static struct ctimer periodic_timer;
static int32_t min_light;
static int32_t max_light;
static int read_light(int32_t *value);
/*---------------------------------------------------------------------------*/
static int
rd_light(lwm2m_context_t *ctx, uint8_t *outbuf, size_t outsize)
{
  int32_t value;
  if(read_light(&value)) {
    return ctx->writer->write_float32fix(ctx, outbuf, outsize,
                                         value, LWM2M_FLOAT32_BITS);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
LWM2M_RESOURCES(light_resources,
                /* Temperature (Current) */
                LWM2M_RESOURCE_CALLBACK(5700, { rd_light, NULL, NULL }),
                /* Units */
                LWM2M_RESOURCE_STRING(5701, "units"),
                /* Min Range Value */
                LWM2M_RESOURCE_FLOATFIX(5603, IPSO_LIGHT_MIN),
                /* Max Range Value */
                LWM2M_RESOURCE_FLOATFIX(5604, IPSO_LIGHT_MAX),
                /* Min Measured Value */
                LWM2M_RESOURCE_FLOATFIX_VAR(5601, &min_light),
                /* Max Measured Value */
                LWM2M_RESOURCE_FLOATFIX_VAR(5602, &max_light),
                );
LWM2M_INSTANCES(light_instances,
                LWM2M_INSTANCE(0, light_resources));
LWM2M_OBJECT(light, 3319, light_instances);
/*---------------------------------------------------------------------------*/
static int
read_light(int32_t *value)
{
#ifdef IPSO_LIGHT
  int32_t light_t;
  if(IPSO_LIGHT.read_value == NULL ||
     IPSO_LIGHT.read_value(&light_t) != 0) {
    return 0;
  }
  *value = (light_t * LWM2M_FLOAT32_FRAC);
  // printf(">>>>>> ipso-light val is %i\n", *value);

  /*Decide if to apply the Threshold also to min and max*/
  if(*value < min_light) {
    min_light = *value;
    lwm2m_object_notify_observers(&light, "/0/5601");
  }
  if(*value > max_light) {
    max_light = *value;
    lwm2m_object_notify_observers(&light, "/0/5602");
  }
  return 1;
#else /* IPSO_LIGHT */
  return 0;
#endif /* IPSO_LIGHT */
}
/*---------------------------------------------------------------------------*/
static void
handle_periodic_timer(void *ptr)
{
#ifdef IPSO_LIGHT
  static int32_t last_value = IPSO_LIGHT_MIN;
  int32_t v;

  /* Only notify when the value has changed since last */
  if(read_light(&v) && v != last_value) {
	if ((v < last_value - LIGHT_NOTIFICATION_THRESH) ||
	 (v > last_value + LIGHT_NOTIFICATION_THRESH)){
	 last_value = v;
	 lwm2m_object_notify_observers(&light, "/0/5700");
	}
  }
  #endif /* IPSO_LIGHT */
  ctimer_reset(&periodic_timer);
}
/*---------------------------------------------------------------------------*/
void
ipso_light_init(void)
{
  int32_t v;
  min_light = IPSO_LIGHT_MAX;
  max_light = IPSO_LIGHT_MIN;

#ifdef IPSO_LIGHT
  if(IPSO_LIGHT.init) {
    IPSO_LIGHT.init();
  }


  /* register this device and its handlers - the handlers automatically
     sends in the object to handle */
  lwm2m_engine_register_object(&light);
#endif /* IPSO_LIGHT */
  
  /* update light and min/max + notify any listeners */
  read_light(&v);
  ctimer_set(&periodic_timer, CLOCK_SECOND * LIGHT_TIMER, handle_periodic_timer, NULL);
}
/*---------------------------------------------------------------------------*/
/** @} */
