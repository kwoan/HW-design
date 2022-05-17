#include "address_map_nios2.h"
#include"nios2_ctrl_reg_macros.h"

volatile int* KEY_ptr = (int*)KEY_BASE;
volatile int* Timer_ptr = (int*)TIMER_BASE;

int count=0;
int run=1;

void config_timer(void){
	*(Timer_ptr+2)=0x7840;
	*(Timer_ptr+3)=0x17d;
	*Timer_ptr=(run<<1);
	*(Timer_ptr+1)=0b0111;
}
	

void config_KEYs(void){
	*(KEY_ptr + 2) = 0b01110;
}

void enable_nios2_interrupts(void){
	NIOS2_WRITE_IENABLE(0x3);
	NIOS2_WRITE_STATUS(1);
}

int main() {
	volatile int* LEDR_ptr=(int*)0xff200000;
	
	config_timer();
	
	config_KEYs();
	
	enable_nios2_interrupts();
	
	while(1) *LEDR_ptr=count;
}