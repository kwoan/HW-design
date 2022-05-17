.text
.global _start
_start:
    movia r4, N(r0)
	ldw r4, (r4)   # r4 - N argument passing 
	mov r5, r0     # r5 - sum
	call FINDSUM
END: br END

FINDSUM: beq r4, r0, STOP
         add r5, r5, r4      # sum=sum+N
		 subi r4, r4, 1      # N=N-1
		 br FINDSUM
		 
STOP: mov r2, r5       # r2 - return value passing
      ret

N: .word 9
   .end