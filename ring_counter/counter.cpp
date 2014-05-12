#include "counter.h"

void counter :: counter_fn() {
	in.write(in.read() + 1); // add one to the port
	printf("in=%d\n", in.read()); // print it for viewing
}
