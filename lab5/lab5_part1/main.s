.text
.global _start
_start:
	movia sp, 0x20000      ## stack set at dram
	movia r22, 0xFF200020   ## hex base addr
	stwio r0, (r22)
	
	movia r22, 0xFF200050   ## key base addr
	movi r23, 0b01111       ## set all interrupt mask bit 1
	stwio r23, 8(r22)
	
	movi r23, 2
	wrctl ienable, r23      ## set interrupt enable bit 1 (key interrupt) 
	movi r23, 1
	wrctl status, r23       ## turn on Nios II interrupt processing
	
IDLE: br IDLE
.end