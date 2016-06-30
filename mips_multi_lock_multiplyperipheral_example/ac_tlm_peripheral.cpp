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
  result(0),
  operand1(0),
  operand2(0),
  lock(0)
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
//  cout << "addr: " <<  std::hex  << a << " data: " << bswap_32(d) << endl;
  //OPERAND1 ADDRESS DEFINED IN SW
  if ((*(uint32_t *) &a) == 0xc900000 ){
    operand1 = bswap_32(d);   
    cout << "operand1: " << hex << operand1 << endl; 
    return SUCCESS;
  }
  //OPERAND2 ADDRESS DEFINED IN SW
  if ((*(uint32_t *) &a) == 0xca00000 ){
    operand2 = bswap_32(d); 
    cout << "operand2: " << hex << operand2 << endl; 
    return SUCCESS;
  }
  lock = bswap_32(d);
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

  //RESULT ADDRESS DEFINED IN SW
  if ((*(uint32_t *) &a) == 0xcb00000 ){
    // cout << "RESULT:" << hex << result << endl;
    result = operand1*operand2; 

    wait(10,SC_NS); //TIME DEFINED BY USER : TIME REQUIRED BY HW_MODULE TO CALCULATE A VALUE
    
    *((uint32_t *) &d) = bswap_32(result); // d = bswap_32(result);
    cout << "D: " << hex << *((uint32_t *) &d) << endl;
    return SUCCESS;
  }
  
  else{
    d = bswap_32(lock);
    lock = 1;
    return SUCCESS;
  }
}




