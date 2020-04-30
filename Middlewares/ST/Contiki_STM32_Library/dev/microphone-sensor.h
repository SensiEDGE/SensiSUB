/**
******************************************************************************
* @file    microphone-sensor.h
* @author  
* @version V1.0.0
* @date    24-July-2017
* @brief   Enable gyroscope sensor functionality 
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
/*---------------------------------------------------------------------------*/
/**
 * @addtogroup microphone_sensor
 * @ingroup dev
 * Maps the microphone sensor on the STM32 Sensor Expansion board.
 * @{
 * @file Header file for the Microphone Sensor Driver
 */
/*---------------------------------------------------------------------------*/
#ifndef MICROPHONE_SENSOR_H_
#define MICROPHONE_SENSOR_H_
/*---------------------------------------------------------------------------*/
#include "lib/sensors.h"
/*---------------------------------------------------------------------------*/
extern const struct sensors_sensor microphone_sensor;
extern float DBNOISE_Value_Old_Ch[];
/*---------------------------------------------------------------------------*/
#define MICROPHONE_SENSOR "Microphone"
/*---------------------------------------------------------------------------*/
#endif /* MICROPHONE_SENSOR_H_ */
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */
