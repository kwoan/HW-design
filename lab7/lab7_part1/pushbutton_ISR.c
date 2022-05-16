#include"address_map_nios2.h"

extern volatile int* KEY_ptr;
extern int HEX_value;

unsigned int BCD[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
			 0b01100110, 0b01101101, 0b01111101, 0b00000111,
			 0b01111111, 0b01100111 };


void pushbutton_ISR(void) {
	int hex0, hex1, hex2, hex3;
	hex0 = (HEX_value) & 0x000000ff;
	hex1 = (HEX_value) & 0x0000ff00;
	hex2 = (HEX_value) & 0x00ff0000;
	hex3 = (HEX_value) & 0xff000000;      // to check hex on/off

	if ((*(KEY_ptr + 3) & 0b0001)!=0) {
		if (hex0==0) {
			HEX_value = (HEX_value) | BCD[0];
		}
		else {
			HEX_value = (HEX_value) & 0xffffff00;
		}
	}
	else if ((*(KEY_ptr + 3) & 0b0010)!=0) {
		if (hex1==0) {
			HEX_value = (HEX_value) | BCD[1]<<8;
		}
		else {
			HEX_value = (HEX_value) & 0xffff00ff;
		}
	}
	else if ((*(KEY_ptr + 3) & 0b0100)!=0) {
		if (hex2==0) {
			HEX_value = (HEX_value) | BCD[2]<<16;
		}
		else {
			HEX_value = (HEX_value) & 0xff00ffff;
		}
	}
	else if ((*(KEY_ptr + 3) & 0b1000)!=0) {
		if (hex3==0) {
			HEX_value = (HEX_value) | BCD[3]<<24;
		}
		else {
			HEX_value = (HEX_value) & 0x00ffffff;
		}
	}
	
	int press = *(KEY_ptr + 3);
	*(KEY_ptr + 3) = press;         // IRQ off
	
	return;
}