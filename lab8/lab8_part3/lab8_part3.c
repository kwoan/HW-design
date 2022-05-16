#define PIXEL_BUF_CTRL_BASE		0xFF203020
#include<math.h>
#include<stdlib.h>

#define PIXEL(r, g, b) \
   (short int)((((r)&0x1f)<<11)|(((g)&0x3f)<<5)|(((b)&0x1f)))

#define FILL_PIXEL(x,y,r,g,b)\
   *(short int *)(pixel_buffer_start + (((y)&0xff)<<10) + (((x)&0x1ff)<<1))=PIXEL(r,g,b)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

typedef struct coordinates {
	int x[8];
	int y[8];
}dot;

volatile int pixel_buffer_start;
volatile int* pixel_ctrl_ptr;

short int front_buffer[512 * 256];
short int back_buffer[512 * 256];

void swap(int*, int*);
void clear_screen(int, int, int);
void draw_square(int, int, int, int, int, int, int);
void draw_line(int, int, int, int, short int);
void plot_pixel(int, int, short int);
void wait_for_vsync(void);

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

void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b) {
	int x, y;
	for (x = x1; x <= x2; x++) {
		for (y = y1; y <= y2; y++) {
			FILL_PIXEL(x, y, r, g, b);
		}
	}
}

void swap(int* x, int* y) {
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;

}

void plot_pixel(int x, int y, short int line_color) {
	*(short int*)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color) {
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
		if (is_steep) plot_pixel(y, x, line_color);
		else plot_pixel(x, y, line_color);

		error += deltay;

		if (error >= 0) {
			y = y + y_step;
			error = error - deltax;
		}
	}
}



int main(void) {
	pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
	pixel_buffer_start = *pixel_ctrl_ptr;

	*(pixel_ctrl_ptr + 1) = front_buffer;
	wait_for_vsync();

	pixel_buffer_start = *pixel_ctrl_ptr;
	clear_screen(0x00, 0x00, 0x00);
	*(pixel_ctrl_ptr + 1) = back_buffer;
	dot* dots = (dot*)malloc(sizeof(dot));
	int i;
	for (i = 0; i < 8; i++) {
		dots->x[i] = (rand() % SCREEN_WIDTH);
		dots->y[i] = (rand() % SCREEN_HEIGHT);
	}
	int x = 0, y = 0;
	int xmode[8] = { 1,1,1,0,0,0,0,1 };
	int ymode[8] = { 0,1,0.0,0,1,1,1 };

	while (1) {
		pixel_buffer_start = *(pixel_ctrl_ptr + 1);
		clear_screen(0x00, 0x00, 0x00);

		for (i = 0; i < 8; i++) {
			plot_pixel(dots->x[i], dots->y[i], 0xffff);
			plot_pixel(dots->x[(i + 1) % 8], dots->y[(i + 1) % 8], 0xffff);
			draw_line(dots->x[i], dots->y[i], dots->x[(i + 1) % 8], dots->y[(i + 1) % 8], 0xffff);
		}
		for (int i = 0; i < 8; i++) {
			if (xmode[i]) {
				if (dots->x[i] >= SCREEN_WIDTH) { xmode[i] ^= 1; }
				else dots->x[i] += 1;
			}
			else {
				if (dots->x[i] <= 0) { xmode[i] ^= 1; }
				else dots->x[i] -= 1;
			}

			if (ymode[i]) {
				if (dots->y[i] >= SCREEN_HEIGHT) { ymode[i] ^= 1; }
				else dots->y[i] += 1;
			}

			else {
				if (dots->y[i] <= 0) { ymode[i] ^= 1; }
				else dots->y[i] -= 1;
			}
		}

		wait_for_vsync();
	}
}