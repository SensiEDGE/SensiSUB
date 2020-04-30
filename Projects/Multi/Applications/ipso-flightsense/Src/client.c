/**
******************************************************************************
  * @file    client.c
  * @author  Central LAB
  * @version V1.0.0
  * @date    20-June-2016
  * @brief   lwm2m client
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
#include "ipso-objects.h"
#include "lwm2m-engine.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/rpl/rpl.h"
#include "net/ip/resolv.h"
#include "net/ip/ip64-addr.h"
#include "rest-engine.h"
#include "net/ip/uip-debug.h"
#include "cube_hal.h"
#include "er-coap.h"

/** @addtogroup LWM2M_example
  * @{
  */


/*---------------------------------------------------------------------------*/
#ifndef REGISTER_WITH_LWM2M_BOOTSTRAP_SERVER
#define REGISTER_WITH_LWM2M_BOOTSTRAP_SERVER 0
#endif
/*---------------------------------------------------------------------------*/
#ifndef REGISTER_WITH_LWM2M_SERVER
#define REGISTER_WITH_LWM2M_SERVER 1
#endif
/*---------------------------------------------------------------------------*/
uip_ipaddr_t server_ipaddr;
/*---------------------------------------------------------------------------*/
/*0 means default port for remote server, otherwise specify a specific port  */
#define SERVER_PORT 0
/*---------------------------------------------------------------------------*/
#ifdef USE_PUBLIC_LWM2M_SERVER
static uip_ipaddr_t *addrptr;
static struct etimer et;
static char host[40] = "leshan.eclipse.org";
#else
/*Define ONE of the following macros*/
#define LWM2M_SERVER_ADDRESS_v4 "192.168.0.11" //for ipv4
//#define LWM2M_SERVER_ADDRESS_v6 "aaaa::1" //for ipv6
#endif /*USE_PUBLIC_LWM2M_SERVER*/

uip_ip4addr_t ip4addr;

PROCESS(rd_client, "OMA LWM2M /rd Client");
AUTOSTART_PROCESSES(&rd_client);

PROCESS_THREAD(rd_client, ev, data)
{
  PROCESS_BEGIN();
  static char server_found = 0;

  printf("RD Client process started.\n\n");

#ifdef USE_PUBLIC_LWM2M_SERVER
  static int ret;
  uip_ip6addr_t ip6addr;

  printf("Looking for LWM2M server: '%s'\n", host);
  uip_ipaddr(&ip4addr, 8,8,8,8);
  ip64_addr_4to6(&ip4addr, &ip6addr);
  uip_nameserver_update(&ip6addr, UIP_NAMESERVER_INFINITE_LIFETIME);

  /*DNS request for server address*/
  etimer_set(&et, 2 * CLOCK_SECOND);
  resolv_query(host);

  HAL_Delay(3000);

  while((ret = resolv_lookup(host, &addrptr)) != RESOLV_STATUS_CACHED) {
	if (ret != RESOLV_STATUS_RESOLVING){
	  resolv_query(host);
	}
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	etimer_reset(&et);
  }

  server_found = 1;
  server_ipaddr = *addrptr;

#else
#ifdef LWM2M_SERVER_ADDRESS_v4
  printf("Looking for LWM2M server: '%s'\n", LWM2M_SERVER_ADDRESS_v4);

  // uip_ipaddr(&ip4addr, 192, 168, 0, 1); //Old code
  if(uiplib_ip4addrconv(LWM2M_SERVER_ADDRESS_v4, &ip4addr)) {
	    ip64_addr_4to6(&ip4addr, &server_ipaddr);
	    server_found = 1;
  }
#endif /* LWM2M_SERVER_ADDRESS_v4 */

#ifdef LWM2M_SERVER_ADDRESS_v6
  printf("Looking for LWM2M server: '%s'\n", LWM2M_SERVER_ADDRESS_v6);

  if(uiplib_ip6addrconv(LWM2M_SERVER_ADDRESS_v6, &server_ipaddr)) {
	    server_found = 1;
  }
#endif /* LWM2M_SERVER_ADDRESS_v6 */
#endif /*USE_PUBLIC_LWM2M_SERVER*/

  if (!server_found) {
    printf("ERROR with the Server IP Address, please check the provided configuration.\n");
  } else {

	printf("LWM2M Server Address:\n");
    uip_debug_ipaddr_print(&server_ipaddr);
    printf("\n");

    /*Notify the address to lwm2m for registration*/
    //lwm2m_engine_register_with_bootstrap_server(&server_ipaddr, 0);
    lwm2m_engine_register_with_server(&server_ipaddr, SERVER_PORT);

    //lwm2m_engine_use_bootstrap_server(REGISTER_WITH_LWM2M_BOOTSTRAP_SERVER);
    lwm2m_engine_use_registration_server(REGISTER_WITH_LWM2M_SERVER);

    /*Initialize objects and start lwm2m engine*/
    lwm2m_engine_init();
    lwm2m_engine_register_default_objects();

    ipso_objects_init();
  }

  PROCESS_END();

}

/**
  * @}
  */

