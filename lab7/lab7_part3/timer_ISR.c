#include"address_map_nios2.h"

extern volatile int* KEY_ptr;
extern volatile int* Timer_ptr;
extern int count;
extern int run;

void timer_ISR(void) {
	if (run) {
		count++;
		if (count == 0x400) count = 0;


	}
	*(Timer_ptr) = (run << 1);
	return;
}