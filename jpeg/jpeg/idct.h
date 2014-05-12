#include "systemc.h"
#include <math.h>

#ifndef IDCT_H
#define IDCT_H

/* Insert your new header code here */
struct idct : sc_module {
	sc_out<double> out64[8][8]; // the dc transformed 8x8 block
	sc_in<double> fcosine[8][8]; // cosine table input
	sc_in<FILE *> sc_input; // input file pointer port
	sc_in<bool> clk; // clock signal
    
	char input_data[8][8]; // the data read from the input file
    
	void read_data( void ); // read the 8x8 block
	void calculate_dct( void ); // perform dc transform
    
	// define fdct as a constructor
	SC_CTOR( idct ) {
		// make read_data method sensitive to the positive clock edge, and
		// the calculate_dct method sensitive to the negative clock edge
		// this way, the entire read and performing dct takes only one clock cycle
		// as apposed to two
		SC_METHOD( read_data ); // define read_data as a method
		dont_initialize();
		sensitive_pos << clk;
        
		SC_METHOD( calculate_dct ); // define calculate_dct as a method
		dont_initialize();
		sensitive_neg << clk;
	}
};

#endif
