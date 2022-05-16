.text
.global _start
_start:
	movia r4, TEST_NUM    # r4 - TEST_NUM addr
	ldw r5, (r4)          # r5 - TEST_NUM value
	mov r2, r0            # return value
	call ONES
	mov r10, r2           # r10 - longest 1 string
	call ZEROS
	mov r11, r2           # r11 - longest 0 string
	call ALTERNATE
	mov r12, r2           # r12 - longest alternate string

END: br END

ONES: beq r5, r0, LOOP1
	  srli r8, r5, 0x01        # r8 - shifted value store
	  and r5, r5, r8
	  addi r9, r9, 0x01        # r9 - counter of string 1
	  br ONES
	  
LOOP1: ldw r5, (r4)
	   mov r2, r9           # return value
	   mov r8, r0           # for reg reuse
	   mov r9, r0           # for reg reuse
       ret

ZEROS: addi r13, r5, 0x01     # r13 - detect when r5's all digit 1
       and r13, r13, r5
       beq r13, r0, LOOP0
       srli r8, r5, 0x01      # r5 - TEST_NUM value
	   or r5, r5, r8
	   addi r9, r9, 0x01
	   br ZEROS
	   
LOOP0: ldw r5, (r4)
	   mov r2, r9           # return value
	   mov r8, r0           # for reg reuse
	   mov r9, r0           # for reg reuse
	   mov r13, r0
       ret

ALTERNATE: srli r8, r5, 0x01
           xor r5, r5, r8
		   br ONES_ALT
		
ONES_ALT: beq r5, r0, LOOP_A
	      srli r13, r5, 0x01        # r13 - shifted value store
	      and r5, r5, r13
	      addi r9, r9, 0x01        # r9 - counter of string 1
	      br ONES_ALT
		  
LOOP_A: addi r9, r9, 0x01   # detect for start point
        mov r2, r9         # return value
        ret
	
TEST_NUM: .word 0b1110101010111111100000
          .end