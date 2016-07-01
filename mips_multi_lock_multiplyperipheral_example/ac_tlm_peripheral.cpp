/**
 * @file      ac_tlm_mem.cpp
 * @author    Bruno de Carvalho Albertini
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 *
 * @brief     Implements a ac_tlm memory.
 *
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *
 */

//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ac_tlm_peripheral.h"
#include <byteswap.h>

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate peripheral from ArchC
using user::ac_tlm_peripheral;

/// Constructor
ac_tlm_peripheral::ac_tlm_peripheral( sc_module_name module_name , int k ) :
  sc_module( module_name ),
  target_export("iport"),
  lock(0),
  port1(0),
  port2(0),
  port3(0),
  port4(0)
{
    /// Binds target_export to the peripheral
    target_export( *this );
}

/// Destructor
ac_tlm_peripheral::~ac_tlm_peripheral() {
}

// sc_event e1,e2;

/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_peripheral::writem( const uint32_t &a , const uint32_t &d )
{
  if (a == ((uint32_t)(200<<20))) {
    lock = bswap_32(d);
  } else if (a == ((uint32_t)(200<<20) +  4)) {
    port1 = bswap_32(d);
  } else if (a == ((uint32_t)(200<<20) +  8)) {
    port2 = bswap_32(d);
  } else if (a == ((uint32_t)(200<<20) + 12)) {
    port3 = bswap_32(d);
  } else if (a == ((uint32_t)(200<<20) + 16)) {
    port4 = bswap_32(d);
  }
  // std::cout << ((uint32_t)(200<<20)) << "addr: " << a << ", lock = " << (int)bswap_32(d) << std::endl;
  return SUCCESS;
}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
  // std::cout << ((uint32_t)(200<<20)) << "addr: " << a << ", lock: " << (int)lock << " -> lock = " << 1 << std::endl;
  if (a == ((uint32_t)(200<<20))) {
    d = bswap_32(lock);
    lock = 1;
  } else if (a == ((uint32_t)(200<<20) +  4)) {
    d = bswap_32(port1);
  } else if (a == ((uint32_t)(200<<20) +  8)) {
    d = bswap_32(port2);
  } else if (a == ((uint32_t)(200<<20) + 12)) {
    d = bswap_32(port3);
  } else if (a == ((uint32_t)(200<<20) + 16)) {
    d = bswap_32(port4);
  }
  return SUCCESS;
}
