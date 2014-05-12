#include "systemc.h"

#ifndef COUNTER
#define COUNTER

// the counter module
struct counter : sc_module {
	sc_inout<sc_uint<6> > counter_in; // the input/output port of int type
	sc_in<bool> clk; // the clock port
	sc_inout<int> flag;
	void counter_fn(); // internal function

	SC_CTOR( counter ) { // counter constructor
		SC_METHOD( counter_fn ); // declare the counter_fn as a method
		dont_initialize(); // dont run it at first execution
		sensitive << clk.pos(); // make it sensitive to the positive clock edge
	}
};

#endif
