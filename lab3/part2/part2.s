.text
.global _start
_start:
    movia r4, N(r0)
	ldw r4, (r4)     # r4 - N argument passing 
	movia sp, 0x20000  # init stack at DRAM address(just at DE1-soC)
	call FINDSUM
END: br END

FINDSUM: bne r4, r0, RECURSE
         mov r2, r0
		 ret
RECURSE: subi sp, sp, 8
         stw r4, 0(sp)   # save N at stack 
		 stw ra, 4(sp)   # save ra at stack (to ret first FINDSUM)
		 
		 subi r4, r4, 1   # N=N-1
		 call FINDSUM     # FINDSUM(N-1)
		 
		 ldw r4, 0(sp)   # restore N at stack 
		 ldw ra, 4(sp)   # restore ra at stack 
		 addi sp, sp, 8
		 
		 add r2, r4, r2  # sum = N+FINDSUM(N-1)
		 ret
		 
N: .word 9
   .end
