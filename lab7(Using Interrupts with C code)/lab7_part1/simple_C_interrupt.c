#include "address_map_nios2.h"
#include"nios2_ctrl_reg_macros.h"

volatile int* KEY_ptr = (int*)KEY_BASE;
volatile int* HEX_ptr = (int*)HEX3_HEX0_BASE;

int HEX_value=0;


int main() {
	*(KEY_ptr + 2) = 0b1111;
	NIOS2_WRITE_IENABLE(0x2);
	NIOS2_WRITE_STATUS(1);
	
	while(1) *HEX_ptr=HEX_value;
}
