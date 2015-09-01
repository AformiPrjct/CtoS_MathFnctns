/*
 * SystemC Synthesizable division Implementation using fixed point numbers for Cadence Ctos HLS tool.
 * 
 * This implementation is based on Bit mathematics cookbook By Joel Yliluoma, January 2014
 * http://bisqwit.iki.fi/story/howto/bitmath/
 * 
 * This code requires 2 clock cycles, while the user can minimize the area managing 
 * the wait() statements.
 * 
 * INPUT : sc_ufixed<64,32> dividend, sc_ufixed<64,32>divisor
 * OUTPUT: sc_fixed<64,32>  
 * 
 * INPUT divisor must not be zero
 */



#if !defined(__CTOS__) && !defined(CTOS_MODEL)
#define SC_INCLUDE_FX
#endif

#include <systemc.h>

#if defined(__CTOS__) || defined(CTOS_MODEL)
#include "ctos_fx.h"
using namespace ctos_sc_dt;
#endif


sc_ufixed<64,32> fix_div(sc_ufixed<64,32> dividend, sc_ufixed<64,32>divisor){
    sc_ufixed<64,32> remain  = dividend; // The left-hand side of division, i.e. what is being divided
    sc_ufixed<64,32> part1   = divisor;  // The right-hand side of division
    sc_ufixed<64,32> result  = (sc_ufixed<64,32>)0;
    sc_ufixed<64,32> mask    = (sc_ufixed<64,32>)1;

    
    for (unsigned long i = 0; i < 32; i++) {
      if(part1 >= remain){
	break;
      }
	// Alternative: while(!(part1 & 0x8000)) // For 16-bit, test highest order bit.
	// Alternative: while(not_signed(part1)) // Same as above: As long as sign bit is not set in part1.
	
	part1 = part1 << 1; // Logical bit-shift left.
	mask  = mask  << 1; // Logical bit-shift left.
    }
    
    wait();
    
    for (unsigned long i = 0; i < 32; i++) {
      if(mask <= (sc_ufixed<64,32>)0){
	break;
      }
      if(remain >= part1)
      {
	remain = remain - part1;
	result = result + mask;
      }
      
      part1 = part1 >> 1;  // Bit-wise logical shift right
      mask  = mask  >> 1;
    }
    
    return result;
}
