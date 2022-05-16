#include <stdio.h>

#define LEDR_BASE 0xFF200000
static volatile int *LEDR_ptr = (int*)LEDR_BASE;

void display_led(int number){
	*LEDR_ptr=number;
}

void main(){
    int LIST[8]={7,4,5,3,6,1,8,2};
    int i;
	int max=0;
    for(i=0;i<8;i++){
        if(max<LIST[i]) max=LIST[i];
    }
    display_led(max);
}