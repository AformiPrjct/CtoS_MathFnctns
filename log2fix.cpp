/*
 * SystemC Synthesizable log2 Implementation using fixed point numbers for Cadence Ctos HLS tool.
 * 
 * This implementation is based on Clay. S. Turner's fast binary logarithm:
 * C. S. Turner,  "A Fast Binary Logarithm Algorithm", IEEE Signal 
 * Processing Mag., pp. 124,140, Sep. 2010.
 * 
 * This code requires 3 clock cycles, while the user can minimize the area managing 
 * the wait() statements.
 * 
 * INPUT : sc_ufixed<64,32>
 * OUTPUT: sc_fixed<64,32>  
 * 
 * PRECISION must be: 1< PRECISION < 31
 * INPUT must be:     in > 0
 */



#if !defined(__CTOS__) && !defined(CTOS_MODEL)
#define SC_INCLUDE_FX
#endif

#include <systemc.h>

#if defined(__CTOS__) || defined(CTOS_MODEL)
#include "ctos_fx.h"
using namespace ctos_sc_dt;
#endif


#define PRECISION 24

sc_fixed<64,32> log2fix (sc_ufixed<64,32> in)
{
    sc_ufixed<32,32> temp = (in<<PRECISION);
    sc_uint<32> x =  static_cast<sc_dt::sc_uint<32> >(temp.range());
  
    int b = 1U << (PRECISION - 1);
    int y = 0;
    
    sc_uint<32> precision1 = 1U << PRECISION;
    sc_uint<32> precision2 = 2U << PRECISION;

    for (unsigned long i = 0; i < PRECISION; i++) {
	if (x >= precision1) {
	  break;
	}
	x <<= 1;
	y -= precision1;
	//wait();
    }
    
    wait();
    
    while (x >= precision2) {
        x >>= 1;
        y += precision1;
	//wait();
    }
    
    wait();
    sc_uint<64> z = x;
    
    for (unsigned long i = 0; i < PRECISION; i++) {
        z = (z * z);
	z = z >> PRECISION;
        if (z >= (precision2)) {
            z = z >> 1;
            y = y + b;
        }
        b = b >> 1;
	//wait();
    }
        
    sc_fixed<64,32> res = y;
    
    sc_fixed<64,32> final_res = (res>>PRECISION);

    return final_res;
}
