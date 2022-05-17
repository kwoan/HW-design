.text
.global _start
_start:
	movia r4, N(r0)
	ldw r4, (r4)
	movia sp, 0x20000
	mov r5, r0
	addi r5, r5, 1
	call FINDFAC
END: br END

FINDFAC: bne r4, r5, RECURSE
         mov r2, r5
		 ret                                                           
RECURSE: subi sp, sp, 8
         stw r4, 0(sp)
		 stw ra, 4(sp)    # ra - to ret past FINDFAC
		 
		 subi r4, r4, 1
		 call FINDFAC     
		 
		 ldw r4, 0(sp)  
		 ldw ra, 4(sp)    # ra - restore past FINDFAC ra
		 addi sp, sp, 8
		 
		 mul r2, r4, r2
		 ret
	
N: .word 5
   .end