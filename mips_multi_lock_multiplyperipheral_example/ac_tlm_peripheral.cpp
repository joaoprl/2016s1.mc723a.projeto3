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
  status01(0),
  base01(0),
  exponent01(0)
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
  uint32_t temp = 0;
  if (a == ((uint32_t)(200<<20))) {
    this->lock = bswap_32(d);
  } else if (a == ((uint32_t)(200<<20) +  4)) {
    this->status01 = bswap_32(d);
  } else if (a == ((uint32_t)(200<<20) +  8)) {
    this->base01 = bswap_32(d);
    std::cout << "base01: " << *((float*)(&this->base01)) << std::endl;
  } else if (a == ((uint32_t)(200<<20) + 12)) {
    this->exponent01 = bswap_32(d);
    std::cout << "exponent01: " << *((float*)(&this->exponent01)) << std::endl;
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

  float *p_base = NULL, *p_exponent = NULL;
  float result = 0.;

  if (a == ((uint32_t)(200<<20))) {
    d = bswap_32(this->lock);
    this->lock = 1;
  } else if (a == ((uint32_t)(200<<20) +  4)) {
    if (this->status01 != 0) {
      p_base = (float*)&(this->base01);
      p_exponent = (float*)&(this->exponent01);
      result = pow(*p_base, *p_exponent);
      printf("%10.2f ^ %10.2f = %10.2f\n", *p_base, *p_exponent, result);
      this->base01 = *((uint32_t*)(&result));
      this->status01 = 0;
    }
    d = bswap_32(this->status01);
  } else if (a == ((uint32_t)(200<<20) +  8)) {
    d = bswap_32(this->base01);
  } else if (a == ((uint32_t)(200<<20) + 12)) {
    d = bswap_32(this->exponent01);
  }
  return SUCCESS;
}
