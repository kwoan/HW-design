#include "address_map_nios2.h"

#define PIXEL(r,g,b)             (short int)((((r)&0x1F)<<11) | (((g)&0x3F)<<5) | (((b)&0x1F)))
#define FILL_PIXEL(x,y,r,g,b)         *(short int*)(pixel_buffer_start + (((y)&0xFF)<<10 + (((x)&0x1FF)<<1))) = PIXEL(r,g,b)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

short int pixel_buffer_start;
volatile int* pixel_ctrl_ptr;

void clear_screen(int r, int g, int b);
void draw_square(int x1, int y1, int x2, int y2, int r, int g, int b);
void wait_for_vsync();

int main(void) {
	pixel_ctrl_ptr = (int*)PIXEL_BUF_CTRL_BASE;
	pixel_buffer_start = *pixel_ctrl_ptr;

	int x = 0, y = 0;
	while (1) {
		clear_screen(0x00, 0x00, 0x00);
		x = (x + 1) % SCREEN_WIDTH;
		y = (y + 1) % SCREEN_WIDTH;
		draw_square(x, y, x + 10, y + 10, 0xff, 0xff, 0xff);
		wait_for_vsync();
	}
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

void wait_for_vsync() {
	register int status;
	*pixel_ctrl_ptr = 1;

	status = *(pixel_ctrl_ptr + 3);
	while ((status & 0x01) != 0)
		status = *(pixel_ctrl_ptr + 3);
}