#include"nios2_ctrl_reg_macros.h"

void main(main);
void interrupt_handler(void);
void pushbutton_ISR(void);

void the_reset(void)  __attribute__((section(".reset")));
void the_reset(void) {
	asm(".set noat");
	asm(".set nobreak");
	asm("movia r2, main");
	asm("jmp r2");
}

void the_exception(void)  __attribute__((section(".exceptions")));
void the_exception(void) {
	asm(".set noat");
	asm(".set nobreak");
	asm("subi sp,sp,128");
	asm("stw et, 96(sp)");
	asm("rdctl et, ctl4");
	asm("beq et, r0,SKIP_EA_DEC");
	asm("subi ea, ea, 4");
	asm("SKIP_EA_DEC:");
	asm("stw r1, 4(sp)");
	asm("stw r2, 8(sp)");
	asm("stw r3, 12(sp)");
	asm("stw r4, 16(sp)");
	asm("stw r5, 20(sp)");
	asm("stw r6, 24(sp)");
	asm("stw r7, 28(sp)");
	asm("stw r8, 32(sp)");
	asm("stw r9, 36(sp)");
	asm("stw r10, 40(sp)");
	asm("stw r11, 44(sp)");
	asm("stw r12, 48(sp)");
	asm("stw r13, 52(sp)");
	asm("stw r14, 56(sp)");
	asm("stw r15, 60(sp)");
	asm("stw r16, 64(sp)");
	asm("stw r17, 68(sp)");
	asm("stw r18, 72(sp)");
	asm("stw r19, 76(sp)");
	asm("stw r20, 80(sp)");
	asm("stw r21, 84(sp)");
	asm("stw r22, 88(sp)");
	asm("stw r23, 92(sp)");
	asm("stw r24, 96(sp)");
	asm("stw r25, 100(sp)");
	asm("stw r26, 104(sp)");
	asm("stw r27, 108(sp)");
	asm("stw r28, 112(sp)");
	asm("stw r29, 116(sp)");
	asm("stw r30, 120(sp)");
	asm("stw r31, 124(sp)");
	asm("addi fp, sp, 128");
	asm("call interrupt_handler");
	asm("ldw r1, 4(sp)");
	asm("ldw r2, 8(sp)");
	asm("ldw r3, 12(sp)");
	asm("ldw r4, 16(sp)");
	asm("ldw r5, 20(sp)");
	asm("ldw r6, 24(sp)");
	asm("ldw r7, 28(sp)");
	asm("ldw r8, 32(sp)");
	asm("ldw r9, 36(sp)");
	asm("ldw r10, 40(sp)");
	asm("ldw r11, 44(sp)");
	asm("ldw r12, 48(sp)");
	asm("ldw r13, 52(sp)");
	asm("ldw r14, 56(sp)");
	asm("ldw r15, 60(sp)");
	asm("ldw r16, 64(sp)");
	asm("ldw r17, 68(sp)");
	asm("ldw r18, 72(sp)");
	asm("ldw r19, 76(sp)");
	asm("ldw r20, 80(sp)");
	asm("ldw r21, 84(sp)");
	asm("ldw r22, 88(sp)");
	asm("ldw r23, 92(sp)");
	asm("ldw r24, 96(sp)");
	asm("ldw r25, 100(sp)");
	asm("ldw r26, 104(sp)");
	asm("ldw r27, 108(sp)");
	asm("ldw r28, 112(sp)");
	asm("ldw r29, 116(sp)");
	asm("ldw r30, 120(sp)");
	asm("ldw r31, 124(sp)");
	asm("addi sp, sp, 128");
	asm("eret");
}

void interrupt_handler(void) {
	int ipending;
	NIOS2_READ_IPENDING(ipending);
	if (ipending & 0x2)
		pushbutton_ISR();
	return;
}