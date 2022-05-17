#include"address_map_nios2.h"

extern volatile int* KEY_ptr;
extern volatile int* Timer_ptr;
extern int count;
extern int run;

void pushbutton_ISR(void) {
	
	int Period=0;
	
	if(*(KEY_ptr+3)==0b1000){
		if(run){
		   run=0;
		   *(Timer_ptr) = (run << 1);
	    }
	    else{
		   run=1;
		   *(Timer_ptr) = (run << 1);
	    }
	}
	else if(*(KEY_ptr+3)==0b0010){    // key1 pressed - faster
		Period=*(Timer_ptr+2);
		Period=Period|(*(Timer_ptr+3)<<16);
		Period=Period/2;
		*(Timer_ptr+2)=Period;
		*(Timer_ptr+3)=Period>>16;
		*Timer_ptr=(run<<1);
	    *(Timer_ptr+1)=0b0111;
		
	}
	else if(*(KEY_ptr+3)==0b0100){    // key2 pressed - slow
		Period=*(Timer_ptr+2);
		Period=Period|(*(Timer_ptr+3)<<16);
		Period=Period*2;
		*(Timer_ptr+2)=Period;
		*(Timer_ptr+3)=Period>>16;
		*Timer_ptr=(run<<1);
	    *(Timer_ptr+1)=0b0111;
	}
	
	int press = *(KEY_ptr+3);
	*(KEY_ptr+3) = press;
	
	
	return;
}