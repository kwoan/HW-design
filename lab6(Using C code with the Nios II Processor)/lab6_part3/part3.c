#include <stdio.h>

#define HEX_BASE 0xFF200020
#define HEX_BASE_45 0xFF200030

unsigned int TEST_NUM[11] = { 0x0000e000, 0x3fabedef,0x00000001,0x00000002,
			  0x75a5a5a5,0x01ffC000, 0x03ffC000,0x55555555,
			  0x77777777,0x08888888,0x00000000 };

int BCD[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
			 0b01100110, 0b01101101, 0b01111101, 0b00000111,
			 0b01111111, 0b01100111 };

static volatile int* HEX_ptr = (int*)HEX_BASE;
static volatile int* HEX_ptr_45=(int*)HEX_BASE_45;

void HEX_display(int number_0, int number_1, int number_2, int number_3) {
	*HEX_ptr=BCD[number_0]|BCD[number_1]<<8|BCD[number_2]<<16|BCD[number_3]<<24;
}
void HEX_45_display(int number_4, int number_5){
	*HEX_ptr_45=BCD[number_4]|BCD[number_5]<<8;
}


int longest_str_1(unsigned int number) {
	int shifted;
	int string = 0;
	if (number == 0) return 0;
	while (number != 0) {
		shifted = number >> 1;
		number = shifted & number;
		string++;
	}
	return string;
}

int longest_str_0(unsigned int number) {
	number = ~number;
	int string;
	string = longest_str_1(number);
	return string;
}

int longest_str_alt(unsigned int number){
	unsigned int cmp=0xaaaaaaaa;
	int string_1, string_0;
	int string;
	int value=number^cmp;
	string_0=longest_str_0(value);
	string_1=longest_str_1(value);
	string=(string_0>string_1)?string_0:string_1;
	return string;
}

int DIVIDE_1(unsigned int number) {
	int digit_1;
	digit_1 = number % 10;
	return digit_1;
}

int DIVIDE_10(int number) {
	int digit_10;
	digit_10 = number / 10;
	return digit_10;
}


int main() {
	int i=0;
	int max_1 = 0;
	int max_0 = 0;
	int max_alt=0;
	int hex0, hex1, hex2, hex3, hex4, hex5;
	int str_1, str_0, str_alt;
	while(TEST_NUM[i]!=0) {
		str_1 = longest_str_1(TEST_NUM[i]);
		if (max_1 < str_1) {
			max_1 = str_1;
		}
		i++;
	}
	i=0;
	while(TEST_NUM[i]!=0) {
		str_0 = longest_str_0(TEST_NUM[i]);

		if (max_0 < str_0) {
			max_0 = str_0;
		}
		i++;
	}
	i=0;
	while(TEST_NUM[i]!=0) {
		str_alt = longest_str_alt(TEST_NUM[i]);

		if (max_alt < str_alt) {
			max_alt = str_alt;
		}
		i++;
	}
	

	hex5=DIVIDE_10(max_alt);
	hex4=DIVIDE_1(max_alt);
	hex3 = DIVIDE_10(max_0);
	hex2 = DIVIDE_1(max_0);
	hex1 = DIVIDE_10(max_1);
	hex0 = DIVIDE_1(max_1);

	HEX_display(hex0, hex1, hex2, hex3);
	HEX_45_display(hex4, hex5);

}	