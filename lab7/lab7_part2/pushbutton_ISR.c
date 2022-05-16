#include"address_map_nios2.h"

extern volatile int* KEY_ptr;
extern volatile int* Timer_ptr;
extern int count;
extern int run;

void pushbutton_ISR(void) {
	
	if(run){
		run=0;
		*(Timer_ptr) = (run << 1);
	}
	else{
		run=1;
		*(Timer_ptr) = (run << 1);
	}
	int press = *(KEY_ptr+3);
	*(KEY_ptr+3) = press;
	
	
	return;
}