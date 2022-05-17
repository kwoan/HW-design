#include <stdio.h>
void main(){
    int LIST[8]={7,4,5,3,6,1,8,2};
    int i;
	int max=0;
    for(i=0;i<8;i++){
        if(max<LIST[i]) max=LIST[i];
    }
    printf("Max value is %d", max);
}