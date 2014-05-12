#include "systemc.h"
#include "counter.h"
//bool check_for_32(int *counted){return true;};
bool check_for_32(int *counted) {
	bool temp;
	// a simple software block that checks and reset the counted variable
	if (*counted == 32){ 
		printf("32 reached ... downcountting\n");	
	//	*counted = 32;	
		temp = true;}
	else{
		temp = false;}
	return temp;	
}
bool check_for_1(int *counted){
	bool temp;
	if(*counted == 1){
		printf("1 reached ... upcountting\n");
	//	*counted = 1;
		temp = true;}
	else{
		temp = false;}
	return temp;
}
int sc_main(int, char *[]) {
	sc_signal<sc_uint<6> >  counting; // the signal for the counting variable
	sc_signal<int> in;
	sc_signal<int> s;
	s.write(0);
	//sc_clock signal (label, period(,) dutycycle, start_time(,) posedge_first
	sc_clock clock("clock", 20,SC_NS, 0.5, 0,SC_NS, true);  // duty cycle = 50% high and 50% low
	int counted; // internal variable, stores the value in the counting signal
	// create the tracefile by the name of "counter_tracefile" and extension ".vcd"
	sc_trace_file *tf = sc_create_vcd_trace_file("counter_tracefile");
	// trace the clock and the counting signals
	// the quotations are the names of the variables as viewed in a waveform generator
	sc_trace(tf, clock, "clock");
	sc_trace(tf, counting, "counting");
	counting.write(1); // reset the counting signal to zero at begin of execution
			// so we know where we are starting from
	counter COUNT("counter"); // call the counter module. COUNT is just a temp name
	// map the ports by name, other method would be positional
	COUNT.counter_in(counting);
	COUNT.clk(clock);
	COUNT.flag(s);
	//COUNT.counter_out(in);
	// generate clock signal 21 times
	for (unsigned char i = 0; i < 64; i++) {
		counted = counting.read(); // copy the signal onto the variable
		if(check_for_32(&counted) == true){ // call the software block and check for 10
			s.write(1);}
			if(check_for_1(&counted) == true){
			s.write(0);
			}
	//	counting.write(counted); // copy the variable onto the signal
		sc_start(20, SC_NS); // run the clock for one period
	}
	sc_close_vcd_trace_file(tf); // close the tracefile
	return 0;
}
