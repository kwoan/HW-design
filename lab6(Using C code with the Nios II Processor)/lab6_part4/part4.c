#include<stdio.h>
#define Timer_base 0xff202000
#define KEY_base 0xff200050
#define HEX_BASE 0xFF200020
#define HEX_BASE_45 0xFF200030

int BCD[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
			 0b01100110, 0b01101101, 0b01111101, 0b00000111,
			 0b01111111, 0b01100111 };

static volatile int* Timer_status_ptr = (int*)Timer_base;
static volatile int* Timer_control_ptr = (int*)(Timer_base+4);
static volatile int* Timer_periodl_ptr = (int*)(Timer_base + 8);
static volatile int* Timer_periodh_ptr = (int*)(Timer_base + 12);

static volatile int* KEY_ptr = (int*)(KEY_base);
static volatile int* HEX_ptr = (int*)HEX_BASE;
static volatile int* HEX_ptr_45 = (int*)HEX_BASE_45;

void HEX_display(int number_0, int number_1, int number_2, int number_3) {
	*HEX_ptr = BCD[number_0] | BCD[number_1] << 8 | BCD[number_2] << 16 | BCD[number_3] << 24;
}
void HEX_45_display(int number_4, int number_5) {
	*HEX_ptr_45 = BCD[number_4] | BCD[number_5] << 8;
}

int polled_time_over() {
	int time_over;
	time_over = *Timer_status_ptr & 0b01;
	return (time_over);
}

void start() {
	     // if timer start
		*Timer_control_ptr = 0b0110;
	
}
void stop() {
	
		*Timer_control_ptr = 0b1010;
	
}
int DIVIDE_min_5(int number) {
	int value;
	value = number / 6000;
	value = value / 10;
	return value;
}
int DIVIDE_min_4(int number) {
	int value;
	value = number / 6000;
	value = value % 10;
	return value;
}
int DIVIDE_sec_3(int number) {
	int value;
	value = (number % 6000) / 100;
	value = value / 10;
	return value;
}
int DIVIDE_sec_2(int number) {
	int value;
	value = (number % 6000) / 100;
	value = value % 10;
	return value;
}
int DIVIDE_milisec_1(int number) {
	int value;
	value = (number % 6000 % 100);
	value = value / 10;
	return value;
}
int DIVIDE_milisec_0(int number) {
	int value;
	value = (number % 6000 % 100);
	value = value % 10;
	return value;
}

int main() {
	int value = 0;
	int hex0, hex1, hex2, hex3, hex4, hex5;
	*Timer_periodl_ptr = 0x4240;
	*Timer_periodh_ptr = 0xf;
	*Timer_status_ptr = 0b10;
	*Timer_control_ptr = 0b0110;

	while (1) {
		
			hex0 = DIVIDE_milisec_0(value);
		    hex1 = DIVIDE_milisec_1(value);
		    hex2 = DIVIDE_sec_2(value);
		    hex3 = DIVIDE_sec_3(value);
		    hex4 = DIVIDE_min_4(value);
		    hex5 = DIVIDE_min_5(value);
		    HEX_display(hex0, hex1, hex2, hex3);
		    HEX_45_display(hex4, hex5);
		    if (polled_time_over() == 1) { value++; *Timer_status_ptr = 0b10; }
		    if (*KEY_ptr!=0) {
			   
				stop();
				while(1){	
					if(*KEY_ptr==0) { break;}
				}
				while(1){
					if(*KEY_ptr!=0) {break;}
				}
				while(1){
					if(*KEY_ptr==0) {break;}
				}
				start(); 
		    }
		
			
		
		    if (value == 363999) { value = 0; }
	}
		
}


