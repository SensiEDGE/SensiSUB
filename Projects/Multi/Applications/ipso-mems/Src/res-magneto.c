/**
******************************************************************************
  * @file    res-magneto.c
  * @author  Central LAB
  * @version V1.0.0
  * @date    15-September-2016
  * @brief   magneto resource
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/**
 * \file
 *         magnetometer sensor
 * \author
 *         Marco Grella
 */

#include "ipso-objects.h"
#include "dev/magneto-sensor.h"
#include "dev/sensor-common.h"
#include "sensors.h"


/** @addtogroup LWM2M_example
  * @{
  */

/*---------------------------------------------------------------------------*/
static int
read_value_x(int32_t *value)
{

	*value = magneto_sensor.value(0);

    return 0;
}
/*---------------------------------------------------------------------------*/
static int
read_value_y(int32_t *value)
{

	*value = magneto_sensor.value(1);

    return 0;
}
/*---------------------------------------------------------------------------*/
static int
read_value_z(int32_t *value)
{

	*value = magneto_sensor.value(2);

    return 0;
}
/*---------------------------------------------------------------------------*/
static void
init_sensor(void)
{
	SENSORS_ACTIVATE(magneto_sensor);
}
/*---------------------------------------------------------------------------*/
const struct ipso_objects_sensor res_magneto = {
  .init = init_sensor
};

const struct ipso_objects_sensor res_magneto_x = {
  .read_value = read_value_x
};

const struct ipso_objects_sensor res_magneto_y = {
  .read_value = read_value_y
};

const struct ipso_objects_sensor res_magneto_z = {
  .read_value = read_value_z
};
/*---------------------------------------------------------------------------*/

/**
 * @}
 */

