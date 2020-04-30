/**
******************************************************************************
  * @file    project-conf.h
  * @author  Central LAB
  * @version V1.0.0
  * @date    18-Oct-2016
  * @brief   Per project specific configuration
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
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
#ifdef USE_STM32F4XX_NUCLEO
#define LWM2M_ENGINE_CLIENT_ENDPOINT_PREFIX "STF4-mems-"
#endif /*USE_STM32F4XX_NUCLEO*/
#ifdef USE_STM32L1XX_NUCLEO
#define LWM2M_ENGINE_CLIENT_ENDPOINT_PREFIX "STL1-mems-"
#endif /*USE_STM32L1XX_NUCLEO*/
#define LWM2M_DEVICE_MANUFACTURER 			"ST Microelectronics"
#define LWM2M_DEVICE_TYPE 					"IPSO Smart Object"
#ifdef USE_STM32F4XX_NUCLEO
#define LWM2M_DEVICE_MODEL_NUMBER 			"STM32F401"
#endif /*USE_STM32F4XX_NUCLEO*/
#ifdef USE_STM32L1XX_NUCLEO
#define LWM2M_DEVICE_MODEL_NUMBER 			"STM32L152"
#endif /*USE_STM32L1XX_NUCLEO*/
#define LWM2M_DEVICE_SERIAL_NO 				serial_number
/*---------------------------------------------------------------------------*/
#define IPSO_TEMPERATURE 					res_temperature
#define IPSO_HUMIDITY 						res_humidity
#define IPSO_BAROMETER 						res_pressure
#define IPSO_MICROPHONE                                         res_noise
#define IPSO_ULTRAVIOLET                                        res_ultraviolet
#define IPSO_LIGHT                                              res_light
/*---------------------------------------------------------------------------*/
#define IPSO_MAGNETO_X 						res_magneto_x
#define IPSO_MAGNETO_Y 						res_magneto_y
#define IPSO_MAGNETO_Z 						res_magneto_z
#define IPSO_MAGNETO 						  res_magneto
/*---------------------------------------------------------------------------*/
#define IPSO_ACCELERATION_X 				res_acceleration_x
#define IPSO_ACCELERATION_Y 				res_acceleration_y
#define IPSO_ACCELERATION_Z 				res_acceleration_z
#define IPSO_ACCELERATION 					res_acceleration
/*---------------------------------------------------------------------------*/
#define IPSO_GYROSCOPE_X 				res_gyroscope_x
#define IPSO_GYROSCOPE_Y 				res_gyroscope_y
#define IPSO_GYROSCOPE_Z 				res_gyroscope_z
#define IPSO_GYROSCOPE 			                res_gyroscope
/*---------------------------------------------------------------------------*/
//#undef IEEE802154_CONF_PANID
//#define IEEE802154_CONF_PANID 0xBEEF
/*---------------------------------------------------------------------------*/
/**
 * Disabling RDC and CSMA to save memory on constrained devices.
 */
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC              nullrdc_driver

#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC              csma_driver
/*---------------------------------------------------------------------------*/
/* Disabling TCP on CoAP nodes. */
#undef UIP_CONF_TCP
#define UIP_CONF_TCP                   0
/*---------------------------------------------------------------------------*/
/* Increase rpl-border-router IP-buffer when using more than 64. */
/* This is the maximum size for CoAP Payload                     */
/* COAP_MAX_BLOCK_SIZE is the 2^x value equal or minor to this   */
#undef REST_MAX_CHUNK_SIZE
#define REST_MAX_CHUNK_SIZE            64 // 48 // original value
/*---------------------------------------------------------------------------*/
/* Default value is 65, you can check how many options you need  */
#undef COAP_MAX_HEADER_SIZE
#define COAP_MAX_HEADER_SIZE           70 // 176 // old value

/* COAP_MAX_PACKET_SIZE = REST_MAX_CHUNK_SIZE + COAP_MAX_HEADER_SIZE */
/*---------------------------------------------------------------------------*/
/* Multiplies with chunk size, be aware of memory constraints. */
#undef COAP_MAX_OPEN_TRANSACTIONS
#define COAP_MAX_OPEN_TRANSACTIONS     10 // 4

/* Filtering .well-known/core per query can be disabled to save space. */
#undef COAP_LINK_FORMAT_FILTERING
#define COAP_LINK_FORMAT_FILTERING     0
#undef COAP_PROXY_OPTION_PROCESSING
#define COAP_PROXY_OPTION_PROCESSING   0
/*---------------------------------------------------------------------------*/
/*Reduce to save memory*/
#define QUEUEBUF_CONF_NUM 16
/*---------------------------------------------------------------------------*/
/*To be validated in your scenario */
//#define UIP_CONF_RECEIVE_WINDOW 150
#undef UIP_CONF_RECEIVE_WINDOW
#undef UIP_CONF_TCP_MSS

/*
 * Constraints and suggestions:
 ** 60 <= UIP_CONF_BUFFER_SIZE <= 1514 (uipopt.h)
 ** (UIP_CONF_BUFFER_SIZE - UIP_UDPIP_HLEN) < 548 (ip64-dhcp.c)
 ** COAP_MAX_PACKET_SIZE > (UIP_CONF_BUFFER_SIZE - UIP_IPH_LEN - UIP_UDPH_LEN) (er-coap.h)
 * The smaller, the lower the TCP throughput, and vv.
 * IP buffer size must match all other hops, in particular the border router.
 * This value limits the maximum size for an IP packet to be received by the system.
 * A good value is 600.
*/
#undef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE 1300
/*---------------------------------------------------------------------------*/
#ifdef UIP_CONF_ROUTER
#undef UIP_CONF_ROUTER
#endif
#define UIP_CONF_ROUTER			1

#ifdef UIP_CONF_IPV6_RPL
#undef UIP_CONF_IPV6_RPL
#endif
#define UIP_CONF_IPV6_RPL		1
/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
