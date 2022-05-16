#include "address_map_nios2.h"
#include<stdio.h>

extern volatile int* key_ptr;
extern volatile int* sw_ptr;
extern int mode;
extern int r, g, b;
extern long int mat[320*240];
extern long int mat_capture_1[320*240];
extern long int mat_capture_2[320*240];
extern long int mat_capture_3[320*240];
extern long int mat_capture_4[320*240];

extern int capture_num;
extern int capture_display;

void Pushbutton_ISR(void){

    
    if((*(key_ptr+3))&0b001) mode=(*sw_ptr)&0b11;
    else if ((*(key_ptr+3))&0b010) {r=((r+1)%4); g=((g+1)%4); b=((b+1)%4);}
    else if ((*(key_ptr+3))&0b100) {
        for(int i=0;i<320;i++){
            for(int j=0;j<240;j++){
                mat[320*j+i]=0;
            }
        }
    }
    else if((*(key_ptr+3))&0b1000){
        if((*sw_ptr)>>9){
            if(capture_num==0){
                for(int i=0;i<320;i++){
                    for(int j=0;j<240;j++){
                        mat_capture_1[320*j+i]=mat[320*j+i];
                    }
                }
                capture_num++;
            }
            else if(capture_num==1){
                for(int i=0;i<320;i++){
                    for(int j=0;j<240;j++){
                        mat_capture_2[320*j+i]=mat[320*j+i];
                    }
                }
                capture_num=0;
            }
            
            
        }

        else{
            if(capture_display==0){
                for(int i=0;i<320;i++){
                    for(int j=0;j<240;j++){
                        mat[320*j+i]=mat_capture_1[320*j+i];
                    }
                }
                capture_display++;
            }
            else if(capture_display==1){
                for(int i=0;i<320;i++){
                    for(int j=0;j<240;j++){
                        mat[320*j+i]=mat_capture_2[320*j+i];
                    }
                }
                capture_display=0;
            }
            
            
        }
        
    }
    
    
    
    
    

    int press=*(key_ptr+3);
    *(key_ptr+3)=press;

    return;
}