/**
******************************************************************************
  * @file    ipso-gyroscope.c
  * @author  Central LAB
  * @version V1.0.0
  * @date    12-July-2018
  * @brief   IPSO Acceleration Object
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2018 Droid Technologies</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of Droid Technologies nor the names of its contributors
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

/**
 * \addtogroup ipso-objects
 * @{
 */

/**
 * \file
 *         Implementation of OMA LWM2M / IPSO Acceleration
 * \author
 *         Taras Chornenkyi
 */

#include <stdint.h>
#include "ipso-objects.h"
#include "lwm2m-object.h"
#include "lwm2m-engine.h"
#include "er-coap-engine.h"
#include "lwm2m-server.h"
#include "leds.h"
/*---------------------------------------------------------------------------*/
#define X_AXIS 0x00
#define Y_AXIS 0x01
#define Z_AXIS 0x02
/*---------------------------------------------------------------------------*/
#define GYROSCOPE_IDLE_THRESH      100
#define LED_BLINK_THRESH 	   500
#define GYROSCOPE_TIMER            1
/*---------------------------------------------------------------------------*/
#ifdef IPSO_GYROSCOPE
extern const struct ipso_objects_sensor IPSO_GYROSCOPE;
#endif /* IPSO_GYROSCOPE */
/*---------------------------------------------------------------------------*/
#ifdef IPSO_GYROSCOPE_X
extern const struct ipso_objects_sensor IPSO_GYROSCOPE_X;
#endif /* IPSO_GYROSCOPE_X */
/*---------------------------------------------------------------------------*/
#ifdef IPSO_GYROSCOPE_Y
extern const struct ipso_objects_sensor IPSO_GYROSCOPE_Y;
#endif /* IPSO_GYROSCOPE_Y */
/*---------------------------------------------------------------------------*/
#ifdef IPSO_GYROSCOPE_Z
extern const struct ipso_objects_sensor IPSO_GYROSCOPE_Z;
#endif /* IPSO_GYROSCOPE_Z */
/*---------------------------------------------------------------------------*/
#ifndef IPSO_GYROSCOPE_MIN
#define IPSO_GYROSCOPE_MIN (-2000)//2G fullscale
#endif
/*---------------------------------------------------------------------------*/
#ifndef IPSO_GYROSCOPE_MAX
#define IPSO_GYROSCOPE_MAX (2000)//2G fullscale
#endif
/*---------------------------------------------------------------------------*/
static struct ctimer periodic_timer;
static int32_t last_X;
static int32_t last_Y;
static int32_t last_Z;
/*---------------------------------------------------------------------------*/
static int32_t pmin = GYROSCOPE_TIMER;
/*---------------------------------------------------------------------------*/
static int get_velocity(int32_t axis, int32_t *value);
/*---------------------------------------------------------------------------*/
static int
read_accX(lwm2m_context_t *ctx, uint8_t *outbuf, size_t outsize)
{
  int32_t value;
  if(get_velocity(X_AXIS, &value)) {
    return ctx->writer->write_float32fix(ctx, outbuf, outsize,
                                         value, LWM2M_FLOAT32_BITS);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
read_accY(lwm2m_context_t *ctx, uint8_t *outbuf, size_t outsize)
{
  int32_t value;
  if(get_velocity(Y_AXIS, &value)) {
    return ctx->writer->write_float32fix(ctx, outbuf, outsize,
                                         value, LWM2M_FLOAT32_BITS);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
read_accZ(lwm2m_context_t *ctx, uint8_t *outbuf, size_t outsize)
{
  int32_t value;
  if(get_velocity(Z_AXIS, &value)) {
    return ctx->writer->write_float32fix(ctx, outbuf, outsize,
                                         value, LWM2M_FLOAT32_BITS);
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
LWM2M_RESOURCES(gyroscope_resources,
                /* Acceleration (Current) */
                LWM2M_RESOURCE_CALLBACK(5702, { read_accX, NULL, NULL }),
                LWM2M_RESOURCE_CALLBACK(5703, { read_accY, NULL, NULL }),
                LWM2M_RESOURCE_CALLBACK(5704, { read_accZ, NULL, NULL }),
                /* Units */
                LWM2M_RESOURCE_STRING(5701, "dps"),
                /* Min Range Value */
                LWM2M_RESOURCE_FLOATFIX(5603, IPSO_GYROSCOPE_MIN),
                /* Max Range Value */
                LWM2M_RESOURCE_FLOATFIX(5604, IPSO_GYROSCOPE_MAX),
                );
LWM2M_INSTANCES(gyroscope_instances,
                LWM2M_INSTANCE(0, gyroscope_resources));
LWM2M_OBJECT(gyroscope, 3318, gyroscope_instances);
/*---------------------------------------------------------------------------*/
static int
get_velocity(int32_t axis, int32_t *val)
{
#ifdef IPSO_GYROSCOPE
  if (axis == X_AXIS){
	  IPSO_GYROSCOPE_X.read_value(val);
  } else if (axis == Y_AXIS){
	  IPSO_GYROSCOPE_Y.read_value(val);
  } else if (axis == Z_AXIS) {
	  IPSO_GYROSCOPE_Z.read_value(val);
  } else {
	  printf("Gyroscope Axis not recognized\n");
	  return 0;
  }
#endif
  return 1;
}
/*---------------------------------------------------------------------------*/
static void
handle_periodic_timer(void *ptr)
{
  int32_t v;

  /* Only notify when the value has changed since last */
  if(get_velocity(X_AXIS, &v) && v != last_X) {
	  if((v > (last_X + GYROSCOPE_IDLE_THRESH)) || (v < (last_X - GYROSCOPE_IDLE_THRESH))){
		  lwm2m_object_notify_observers(&gyroscope, "/0/5702");
	  }

	  last_X = v;
  }
  if(get_velocity(Y_AXIS, &v) && v != last_Y) {

	  if((v > (last_Y + GYROSCOPE_IDLE_THRESH)) || (v < (last_Y - GYROSCOPE_IDLE_THRESH))){
		  lwm2m_object_notify_observers(&gyroscope, "/0/5703");
	  }
	  last_Y = v;
  }
  if(get_velocity(Z_AXIS, &v) && v != last_Z) {

	  if((v > (last_Z + GYROSCOPE_IDLE_THRESH)) || (v < (last_Z - GYROSCOPE_IDLE_THRESH))){
		  lwm2m_object_notify_observers(&gyroscope, "/0/5704");
	  }
	  last_Z = v;
  }

  int32_t pt = get_def_pmin();
  if (pt > 0 && pt != pmin)
  {
	  pmin = pt;
	  ctimer_set(&periodic_timer, CLOCK_SECOND * pmin, handle_periodic_timer, NULL);
  }
  else
  {
	  ctimer_reset(&periodic_timer);
  }
}
/*---------------------------------------------------------------------------*/
void
ipso_gyroscope_init(void)
{

#ifdef IPSO_GYROSCOPE
  IPSO_GYROSCOPE.init();
#endif

  /* register this device and its handlers - the handlers automatically
     sends in the object to handle */
  lwm2m_engine_register_object(&gyroscope);

  int32_t pt = get_def_pmin();
  if (pt > 0)
  {
	  pmin = pt;
  }
  /* update temp and min/max + notify any listeners */
  ctimer_set(&periodic_timer, CLOCK_SECOND * pmin, handle_periodic_timer, NULL);
}
/*---------------------------------------------------------------------------*/
/** @} */
