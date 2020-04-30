/**
******************************************************************************
  * @file    ipso-noise.c
  * @author  
  * @version V1.0.0
  * @date    04-August-2017
  * @brief   IPSO Noise Object
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
 * \addtogroup ipso-objects
 * @{
 */

/**
 * \file
 *         Implementation of OMA LWM2M / IPSO Noise
 * \author
 *         
 */

#include <stdint.h>
#include "ipso-objects.h"
#include "lwm2m-object.h"
#include "lwm2m-engine.h"
#include "er-coap-engine.h"
/*---------------------------------------------------------------------------*/
#define NOISE_NOTIFICATION_THRESH 20 // 1000
#define NOISE_TIMER               1 // 60 sec
/*---------------------------------------------------------------------------*/
#ifdef IPSO_MICROPHONE
extern const struct ipso_objects_sensor IPSO_MICROPHONE;
#endif /* IPSO_MICROPHONE */
/*---------------------------------------------------------------------------*/
#ifndef IPSO_NOISE_MIN
#define IPSO_NOISE_MIN (1)
#endif
/*---------------------------------------------------------------------------*/
#ifndef IPSO_NOISE_MAX
#define IPSO_NOISE_MAX (200)
#endif
/*---------------------------------------------------------------------------*/
static struct ctimer periodic_timer;
static int32_t min_noise;
static int32_t max_noise;
static int read_noise(int32_t *value);
/*---------------------------------------------------------------------------*/
static int
nois(lwm2m_context_t *ctx, uint8_t *outbuf, size_t outsize)
{
  int32_t value;
  if(read_noise(&value)) {
    return ctx->writer->write_float32fix(ctx, outbuf, outsize,
                                         value, LWM2M_FLOAT32_BITS);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
LWM2M_RESOURCES(noise_resources,
                /* Temperature (Current) */
                LWM2M_RESOURCE_CALLBACK(5700, { nois, NULL, NULL }),
                /* Units */
                LWM2M_RESOURCE_STRING(5701, "dB"),
                /* Min Range Value */
                LWM2M_RESOURCE_FLOATFIX(5603, IPSO_NOISE_MIN),
                /* Max Range Value */
                LWM2M_RESOURCE_FLOATFIX(5604, IPSO_NOISE_MAX),
                /* Min Measured Value */
                LWM2M_RESOURCE_FLOATFIX_VAR(5601, &min_noise),
                /* Max Measured Value */
                LWM2M_RESOURCE_FLOATFIX_VAR(5602, &max_noise),
                );
LWM2M_INSTANCES(noise_instances,
                LWM2M_INSTANCE(0, noise_resources));
LWM2M_OBJECT(noise, 3316, noise_instances);
/*---------------------------------------------------------------------------*/
static int
read_noise(int32_t *value)
{
#ifdef IPSO_MICROPHONE
  int32_t noise_t;
  if(IPSO_MICROPHONE.read_value == NULL ||
     IPSO_MICROPHONE.read_value(&noise_t) != 0) {
    return 0;
  }

  *value = noise_t;

  /*Decide if to apply the Threshold also to min and max*/
  if(*value < min_noise) {
    min_noise = *value;
    lwm2m_object_notify_observers(&noise, "/0/5601");
  }
  if(*value > max_noise) {
    max_noise = *value;
    lwm2m_object_notify_observers(&noise, "/0/5602");
  }
  return 1;
#else /* IPSO_MICROPHONE */
  return 0;
#endif /* IPSO_MICROPHONE */
}
/*---------------------------------------------------------------------------*/
static void
handle_periodic_timer(void *ptr)
{
#ifdef IPSO_MICROPHONE
  static int32_t last_value = IPSO_NOISE_MIN;
  int32_t v;

  /* Only notify when the value has changed since last */
  if(read_noise(&v) && v != last_value) {
	  if ((v < last_value - NOISE_NOTIFICATION_THRESH) ||
		  (v > last_value + NOISE_NOTIFICATION_THRESH)){
		  last_value = v;
		  lwm2m_object_notify_observers(&noise, "/0/5700");
	  }
  }
#endif /* IPSO_MICROPHONE */
  ctimer_reset(&periodic_timer);
}
/*---------------------------------------------------------------------------*/
void
ipso_noise_init(void)
{
  int32_t v;
  min_noise = IPSO_NOISE_MAX;
  max_noise = IPSO_NOISE_MIN;

#ifdef IPSO_MICROPHONE
  if(IPSO_MICROPHONE.init) {
    IPSO_MICROPHONE.init();
  }


  /* register this device and its handlers - the handlers automatically
     sends in the object to handle */
  lwm2m_engine_register_object(&noise);
#endif /* IPSO_MICROPHONE */
  /* update noise and min/max + notify any listeners */
  read_noise(&v);
  ctimer_set(&periodic_timer, CLOCK_SECOND * NOISE_TIMER, handle_periodic_timer, NULL);
}
/*---------------------------------------------------------------------------*/
/** @} */
