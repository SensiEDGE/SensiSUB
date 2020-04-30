/**
******************************************************************************
  * @file    ipso-ultraviolet.c
  * @author  
  * @version V1.0.0
  * @date    08-August-2017
  * @brief   IPSO Ultraviolet Object
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
 *         Implementation of OMA LWM2M / IPSO Ultraviolet
 *         
 */

#include <stdint.h>
#include "ipso-objects.h"
#include "lwm2m-object.h"
#include "lwm2m-engine.h"
#include "er-coap-engine.h"
/*---------------------------------------------------------------------------*/
#define ULTRAVIOLET_NOTIFICATION_THRESH 1 // 1000
#define ULTRAVIOLET_TIMER               1 // 60 sec
/*---------------------------------------------------------------------------*/
#ifdef IPSO_ULTRAVIOLET
extern const struct ipso_objects_sensor IPSO_ULTRAVIOLET;
#endif /* IPSO_ULTRAVIOLET */
/*---------------------------------------------------------------------------*/
#ifndef IPSO_ULTRAVIOLET_MIN
#define IPSO_ULTRAVIOLET_MIN (1 * LWM2M_FLOAT32_FRAC)
#endif
/*---------------------------------------------------------------------------*/
#ifndef IPSO_ULTRAVIOLET_MAX
#define IPSO_ULTRAVIOLET_MAX (100 * LWM2M_FLOAT32_FRAC)
#endif
/*---------------------------------------------------------------------------*/
static struct ctimer periodic_timer;
static int32_t min_uv;
static int32_t max_uv;
static int read_uv(int32_t *value);
/*---------------------------------------------------------------------------*/
static int
uv(lwm2m_context_t *ctx, uint8_t *outbuf, size_t outsize)
{
  int32_t value;
  if(read_uv(&value)) {
    return ctx->writer->write_float32fix(ctx, outbuf, outsize,
                                         value, LWM2M_FLOAT32_BITS);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
LWM2M_RESOURCES(ultraviolet_resources,
                /* Temperature (Current) */
                LWM2M_RESOURCE_CALLBACK(5700, { uv, NULL, NULL }),
                /* Units */
                LWM2M_RESOURCE_STRING(5701, "units"),
                /* Min Range Value */
                LWM2M_RESOURCE_FLOATFIX(5603, IPSO_ULTRAVIOLET_MIN),
                /* Max Range Value */
                LWM2M_RESOURCE_FLOATFIX(5604, IPSO_ULTRAVIOLET_MAX),
                /* Min Measured Value */
                LWM2M_RESOURCE_FLOATFIX_VAR(5601, &min_uv),
                /* Max Measured Value */
                LWM2M_RESOURCE_FLOATFIX_VAR(5602, &max_uv),
                );
LWM2M_INSTANCES(ultraviolet_instances,
                LWM2M_INSTANCE(0, ultraviolet_resources));
LWM2M_OBJECT(ultraviolet, 3317, ultraviolet_instances);
/*---------------------------------------------------------------------------*/
static int
read_uv(int32_t *value)
{
#ifdef IPSO_ULTRAVIOLET
  int32_t uv_t;
  if(IPSO_ULTRAVIOLET.read_value == NULL ||
     IPSO_ULTRAVIOLET.read_value(&uv_t) != 0) {
    return 0;
  }

  *value = (uv_t * LWM2M_FLOAT32_FRAC);

  /*Decide if to apply the Threshold also to min and max*/
  if(*value < min_uv) {
    min_uv = *value;
    lwm2m_object_notify_observers(&ultraviolet, "/0/5601");
  }
  if(*value > max_uv) {
    max_uv = *value;
    lwm2m_object_notify_observers(&ultraviolet, "/0/5602");
  }
  return 1;
#else /* IPSO_ULTRAVIOLET */
  return 0;
#endif /* IPSO_ULTRAVIOLET */
}
/*---------------------------------------------------------------------------*/
static void
handle_periodic_timer(void *ptr)
{
#ifdef IPSO_ULTRAVIOLET
  static int32_t last_value = IPSO_ULTRAVIOLET_MIN;
  int32_t v;

  /* Only notify when the value has changed since last */
  if(read_uv(&v) && v != last_value) {
	if ((v < last_value - ULTRAVIOLET_NOTIFICATION_THRESH) ||
	 (v > last_value + ULTRAVIOLET_NOTIFICATION_THRESH)){
	 last_value = v;
	 lwm2m_object_notify_observers(&ultraviolet, "/0/5700");
	}
  }
  #endif /* IPSO_ULTRAVIOLET */
  ctimer_reset(&periodic_timer);
}
/*---------------------------------------------------------------------------*/
void
ipso_ultraviolet_init(void)
{
  int32_t v;
  min_uv = IPSO_ULTRAVIOLET_MAX;
  max_uv = IPSO_ULTRAVIOLET_MIN;

#ifdef IPSO_ULTRAVIOLET
  if(IPSO_ULTRAVIOLET.init) {
    IPSO_ULTRAVIOLET.init();
  }


  /* register this device and its handlers - the handlers automatically
     sends in the object to handle */
  lwm2m_engine_register_object(&ultraviolet);
#endif /* IPSO_ULTRAVIOLET */
  
  /* update uv and min/max + notify any listeners */
  read_uv(&v);
  ctimer_set(&periodic_timer, CLOCK_SECOND * ULTRAVIOLET_TIMER, handle_periodic_timer, NULL);
}
/*---------------------------------------------------------------------------*/
/** @} */
