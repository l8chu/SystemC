#include "counter.h"

void counter :: counter_fn() {
	
	if(flag == 0){
	counter_in.write(counter_in.read() * 2); // add one to the port
//	printf("in=%s\n", counter_in.read());
	cout << counter_in.read() << "\n"; } // print it for viewing

	else{
	
	counter_in.write(counter_in.read() / 2);
//	printf("in=%s\n", counter_in.read());
	cout << counter_in.read() << "\n"; }
}
