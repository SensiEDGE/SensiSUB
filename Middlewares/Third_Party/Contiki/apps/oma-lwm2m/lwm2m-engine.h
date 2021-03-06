/*
 * Copyright (c) 2015, Yanzi Networks AB.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \addtogroup oma-lwm2m
 * @{
 */

/**
 * \file
 *         Header file for the Contiki OMA LWM2M engine
 * \author
 *         Joakim Eriksson <joakime@sics.se>
 *         Niclas Finne <nfi@sics.se>
 */

#ifndef LWM2M_ENGINE_H
#define LWM2M_ENGINE_H

#include "lwm2m-object.h"

#define LWM2M_FLOAT32_BITS  10
#define LWM2M_FLOAT32_FRAC (1L << LWM2M_FLOAT32_BITS)

/* LWM2M / CoAP Content-Formats */
typedef enum {
  LWM2M_TEXT_PLAIN = 1541,
  LWM2M_TLV_OLD    = 1542, //MGR: for backward compatibility
  LWM2M_JSON_OLD   = 1543, //MGR: for backward compatibility
  LWM2M_OPAQUE     = 1544,
  LWM2M_TLV        = 11542,//MGR: to align with final value on OMNA
  LWM2M_JSON       = 11543 //MGR: to align with final value on OMNA
} lwm2m_content_format_t;

void lwm2m_engine_init(void);
void lwm2m_engine_register_default_objects(void);
void lwm2m_engine_use_bootstrap_server(int use);
void lwm2m_engine_use_registration_server(int use);
void lwm2m_engine_register_with_server(const uip_ipaddr_t *server, uint16_t port);
void lwm2m_engine_register_with_bootstrap_server(const uip_ipaddr_t *server, uint16_t port);
void update_to_lwm2m_server(); //MGR

const lwm2m_object_t *lwm2m_engine_get_object(uint16_t id);

int lwm2m_engine_register_object(const lwm2m_object_t *object);

void lwm2m_engine_handler(const lwm2m_object_t *object,
                          void *request, void *response,
                          uint8_t *buffer, uint16_t preferred_size,
                          int32_t *offset);

void lwm2m_engine_delete_handler(const lwm2m_object_t *object,
                                 void *request, void *response,
                                 uint8_t *buffer, uint16_t preferred_size,
                                 int32_t *offset);

#endif /* LWM2M_ENGINE_H */
/** @} */
