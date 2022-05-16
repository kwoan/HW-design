#include <stdio.h>
#include <stdlib.h>
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"

#define PIXEL(r, g, b) \
   (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))

#define FILL_PIXEL(x,y,r,g,b)\
   *(short int *)(pixel_buffer_start + (((y)&0xff)<<10) + (((x)&0x1ff)<<1))=PIXEL(r,g,b)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

volatile int timeout; // used to synchronize with the timer
volatile int * interval_timer_ptr = (int *) TIMER_BASE;
volatile int* key_ptr=(int*) KEY_BASE;
volatile int* sw_ptr=(int*) SW_BASE;	
volatile int pixel_buffer_start;
volatile int* pixel_ctrl_ptr;

short int front_buffer[512 * 256];
short int back_buffer[512 * 256];
int x = SCREEN_WIDTH/2;
int y = SCREEN_HEIGHT/2;
int mode = 0;	
long int mat[320*240]={0};
long int mat_capture_1[320*240]={0};
long int mat_capture_2[320*240]={0};
long int mat_capture_3[320*240]={0};
long int mat_capture_4[320*240]={0};
int capture_num=0;
int capture_display=0;

int r=0, g=0, b=0;
int color_r[4]={0x00, 0x1f, 0x00, 0x00};
int color_g[4]={0x00, 0x00, 0x3f, 0x00};
int color_b[4]={0x00, 0x00, 0x00, 0x1f};

char LEVEL[11]={-5,-4,-3,-2,-1,0,1,2,3,4,5};

void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b) {
	int x, y;
	for (x = x1; x <= x2; x++) {
		for (y = y1; y <= y2; y++) {
			FILL_PIXEL(x, y, r, g, b);
		}
	}
}

void draw_square_with_save(int x1, int y1, int x2, int y2, int r, int g, int b) {
	int x, y;
	for (x = x1; x <= x2; x++) {
		for (y = y1; y <= y2; y++) {
            
			FILL_PIXEL(x, y, r, g, b);
            mat[320*y+x]=(r<<29) | (g<<23) | (b<<18) | x<<9 | y ;
		}
	}
}

void draw_square_with_unsave(int x1, int y1, int x2, int y2, int r, int g, int b) {
	int x, y;
	for (x = x1; x <= x2; x++) {
		for (y = y1; y <= y2; y++) {
			FILL_PIXEL(x, y, r, g, b);
            mat[320*y+x]=(r<<29) | (g<<23) | (b<<18) | x<<9 | y ;
            
            
		}
	}
}

void wait_for_vsync() {
	register int status;
	*pixel_ctrl_ptr = 1;

	status = *(pixel_ctrl_ptr + 3);
	while ((status & 0x01) != 0)
		status = *(pixel_ctrl_ptr + 3);
}

void clear_screen(int r, int g, int b) {
	draw_square(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, r, g, b);
}

void clear_screen_white(int r, int g, int b) {
	draw_square(0, 10 , SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, r, g, b);
}

void draw_brush(int x, int y, int r, int g, int b) {
	draw_square(x-7, y-7, x+7, y+7, r, g, b);
}

void draw_eraser(int x, int y, int r, int g, int b) {
	draw_square(x-1, y-1, x+1, y+1, r, g, b);
}
void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
  *character_buffer = c;
}

void swap(int *x, int *y){
   int temp;
   temp=*x;
   *x=*y;
   *y=temp;
   
}

    
void draw_line(int x0, int y0, int x1, int y1, int r, int g, int b) {
	int is_steep;
    
	(abs(y1 - y0) > abs(x1 - x0)) ? 1 : 0;
	is_steep = (abs(y1 - y0) > abs(x1 - x0));
	if (is_steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
	int y_step;
	if (y0 < y1)  y_step = 1;
	else  y_step = -1;
	for (int x = x0; x <= x1; x++) {
		if (is_steep) {FILL_PIXEL(y,x, r, g, b); mat[320*x+y]= (r<<29) | (g<<23) | (b<<18) | (y <<9) | x;}
		else {FILL_PIXEL(x,y,r, g, b); mat[320*y+x]=(r<<29) | (g<<23) | (b<<18) | (x <<9) | y;}


		error+=deltay;
		
		if (error >= 0) {
			y = y + y_step;
			error = error - deltax;
		}
	}
   
    
}



volatile int * PS2_ptr = (int *)PS2_BASE;

void config_intt(void){
    int counter = 10000; // 1/(100 MHz) x (20000000) = 200 msec
    *(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
    *(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

    /* start interval timer, enable its interrupts */
    *(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1

    *(key_ptr+2)=0b01111;    

    NIOS2_WRITE_IENABLE(0x3); 
               
    NIOS2_WRITE_STATUS(1); // enable Nios II interrupts
}

/* function prototypes */
 void command_ps2_with_ack(int command){
     *(PS2_ptr)=command;
    char byte1=0, byte2=0, byte3=0;
    volatile int PS2_data, RAVAIL;

    while(1){
        PS2_data=*(PS2_ptr);

        RAVAIL= (PS2_data & 0xFFFF0000) >> 16;

        if (RAVAIL > 0){
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;
            if(byte3==0xfffffffa) {
                printf("success \n");return;
            }
        }
    }

}

void command_ps2_selftest(void){
    char byte1=0, byte2=0, byte3=0;
    volatile int PS2_data, RAVAIL;

    while(1){
        PS2_data=*(PS2_ptr);

        RAVAIL= (PS2_data & 0xFFFF0000) >> 16;

        if (RAVAIL > 0){
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;
            if ((byte2 == (char)0xAA) && (byte3 == (char)0x00))
                // mouse inserted; initialize sending of data
                {
                    printf("success \n");return;
                }

        }
    }

}
int num=0;



/*******************************************************************************
 * This program demonstrates use of the PS/2 port by displaying the last three
 * bytes of data received from the PS/2 port on the HEX displays.
 ******************************************************************************/
int main(void) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
       and store instructions will be used to access these pointer locations,
       instead of regular memory loads and stores) */


    pixel_ctrl_ptr=(int*)PIXEL_BUF_CTRL_BASE;
    *(pixel_ctrl_ptr+1)=front_buffer;
    wait_for_vsync();

    pixel_buffer_start=*pixel_ctrl_ptr;
    clear_screen(0x00,0x00,0x00);
    *(pixel_ctrl_ptr+1)=back_buffer;

    char byte1 = 0, byte2 = 0, byte3 = 0;
    char PS2_byte1 = 0, rel_x = 0, rel_y = 0;
    

    // PS/2 mouse needs to be reset (must be already plugged in)
    command_ps2_with_ack(0xFF);
    command_ps2_selftest();
    command_ps2_with_ack(0xF4);
    command_ps2_with_ack(0xF3);
    command_ps2_with_ack(0x0A);
    command_ps2_with_ack(0xE7);

    config_intt();

    int  PS2_data, RVALID;
    int prev_pix=0;
    int prev_tag=0;


    while (1) {

        while(!timeout);

        int bf=5;
        char* hw[] = { "pencil_mode  ","eraser_mode  ","brush_mode  ", "                                                   "};  
	    while (*hw[mode]) {
		    write_char(bf, 1, *hw[mode]);
            bf++;
		    hw[mode]++;
	    }


        PS2_data = *(PS2_ptr);        // read the Data register in the PS/2 port
        RVALID   = PS2_data & 0x8000; // extract the RVALID field
        if (RVALID) {
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;

            if(num==0){
                    PS2_byte1=byte3;
                    num++;
            }
            else if(num==1){
                rel_x=byte3;
                if(rel_x>90) rel_x=LEVEL[10];
                else if((rel_x<=90)&&(rel_x>70)) rel_x=LEVEL[9];
                else if((rel_x<=70)&&(rel_x>45)) rel_x=LEVEL[8];
                else if((rel_x<=45)&&(rel_x>20)) rel_x=LEVEL[7];
                else if((rel_x<=20)&&(rel_x>10)) rel_x=LEVEL[6];
                else if((rel_x<=10)&&(rel_x>-10)) rel_x=LEVEL[5];
                else if((rel_x<=-10)&&(rel_x>-20)) rel_x=LEVEL[4];
                else if((rel_x<=-20)&&(rel_x>-45)) rel_x=LEVEL[3];
                else if((rel_x<=-45)&&(rel_x>-70)) rel_x=LEVEL[2];
                else if((rel_x<=-70)&&(rel_x>-90)) rel_x=LEVEL[1];
                else if((rel_x<=-90)) rel_x=LEVEL[0];
                num++;
            }
            else if(num==2){  

                pixel_buffer_start=*(pixel_ctrl_ptr+1);

                clear_screen_white(0xFF, 0xFF, 0xFF);    

                for (int i = 10; i < SCREEN_HEIGHT; i++) {
			        for (int j = 0; j < SCREEN_WIDTH; j++) {
				        FILL_PIXEL(((mat[320 * i + j]>>9) & 0x1FF), ((mat[320 * i + j]) & 0xFF), ((mat[320 * i + j]>>29) & 0x1F), ((mat[320 * i + j]>>23) & 0x3F), ((mat[320 * i + j]>>18) & 0x1F));
			        }
		        }      

                    
                rel_y=byte3;
                if(rel_y>90) rel_y=LEVEL[0];
                else if((rel_y<=90)&&(rel_y>70)) rel_y=LEVEL[1];
                else if((rel_y<=70)&&(rel_y>45)) rel_y=LEVEL[2];
                else if((rel_y<=45)&&(rel_y>20)) rel_y=LEVEL[3];
                else if((rel_y<=20)&&(rel_y>10)) rel_y=LEVEL[4];
                else if((rel_y<=10)&&(rel_y>-10)) rel_y=LEVEL[5];
                else if((rel_y<=-10)&&(rel_y>-20)) rel_y=LEVEL[6];
                else if((rel_y<=-20)&&(rel_y>-45)) rel_y=LEVEL[7];
                else if((rel_y<=-45)&&(rel_y>-70)) rel_y=LEVEL[8];
                else if((rel_y<=-70)&&(rel_y>-90)) rel_y=LEVEL[9];
                else if((rel_y<=-90)) rel_y=LEVEL[10];

                x+=(int)rel_x;
                y+=(int)rel_y; 
                if(x>(SCREEN_WIDTH-1)) x=(SCREEN_WIDTH-1);
                else if(x<0) x=0;

                if(y>(SCREEN_HEIGHT-1)) y=(SCREEN_HEIGHT-1);
                else if(y<10) y=10;


                if ((PS2_byte1 & 0b01)&&(mode==0)) {
                    prev_tag=1;
                    if(prev_pix==0){
                        FILL_PIXEL(x, y, color_r[r], color_g[g], color_b[b]);
                        prev_pix=(color_r[r]<<29) | (color_g[g]<<23) | (color_b[b]<<18) | (x <<9) | y;
                        mat[320 * y + x] = (color_r[r]<<29) | (color_g[g]<<23) | (color_b[b]<<18) | (x <<9) | y;
                    }
                    else{
                        draw_line(((prev_pix>>9)&0x1FF), prev_pix&0xFF, x, y, color_r[r], color_g[g], color_b[b]);
                        prev_pix=(color_r[r]<<29) | (color_g[g]<<23) | (color_b[b]<<18) | (x <<9) | y;
                        
                    }
                }

                else if((PS2_byte1 & 0b01)&&(mode==1)){
                    draw_square_with_unsave(x-5, y-5, x+5, y+5, 0xff, 0xff, 0xff);
                }
                else if((PS2_byte1 & 0b01)&&(mode==2)){
                    draw_square_with_save(x-7, y-7, x+7, y+7, color_r[r], color_g[g], color_b[b]);
                    
                }
                
                if(prev_tag==0) prev_pix=0;
                else prev_tag=0;

                if(mode==0) FILL_PIXEL(x, y, 0x00, 0x00, 0x00);
                else if(mode==1) draw_eraser(x, y, 0x00, 0x00, 0x00);
                else if(mode==2) draw_brush(x, y, color_r[r], color_g[g], color_b[b]);

		        wait_for_vsync();
                   
                num=0;
               
            }    
  
        }              
        

            
    timeout=0;    
    }
}

