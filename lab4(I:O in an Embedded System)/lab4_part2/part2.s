.text
.global _start

_start:  movia r18, 0xff202000   ## Timer_base_addr
         movi  r23, 100
		 movia r22, 0xff200020   ## HEX_base_addr
		 movui r20, 0xe100       ## period_l setting
		 stwio r20, 8(r18)
		 movui r20, 0x5f5        ## period_h setting
		 stwio r20, 12(r18)	
		 mov   r9, r0                ## timer digit
		 ldw   r10, INITIAL(r0)   
		 stwio r10, (r18)             ## initial status
		 ldw   r21, CONTROL(r0)
		 stwio r21, 4(r18)           ## timer control  
	 
Time_Over_Wait:   ldwio  r11, (r18)
                  addi r12, r11, 1
				  and  r12, r12, r11
				  bne  r12, r0, Time_Over_Wait    ## polling TO
				  stwio r10, (r18)    
				  addi r9, r9, 1      ## number of TO
				  beq  r9, r23, END
                  


DISPLAY: call DIVIDE         # ones digit will be in r2; tens digit in r3
		 mov r4, r2          # pass ones digit to SEG7_CODE
		 call SEG7_CODE
		 mov r14, r2         # save bit code
		 mov r4, r3          # retrieve tens digit, pass to SEG7_CODE
		 call SEG7_CODE
		 slli r2, r2, 8
		 or r14, r14, r2     # bit code for tens|bit code for ones
		 stwio r14, (r22)
		 br Time_Over_Wait



INITIAL: .word 0b10
CONTROL: .word 0b0110


DIVIDE: mov    r2, r9     # r5로 계산
        movi   r5, 10    # divider
        movi   r3, 0     # 몫
		
CONT:  blt r2, r5, DIV_END  # divider보다 작을 때 까지 뺄셈 수행
       sub r2, r2, r5       # 나머지 계산
       addi r3, r3, 1       # 몫 계산
       br CONT	  
	 
DIV_END: ret
	
SEG7_CODE: movia r15, BIT_CODES # starting address of bit codes
           add r15, r15, r4 # index into the bit codes
           ldb r2, (r15) # read the bit code needed for our digit
           ret
		   
BIT_CODES: .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
           .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
		   .skip  2  # pad with 2 bytes to maintain word alignment

END:      movi r24, 0b1010
		  stbio r24, 4(r18)
END_end:  br END_end

Digits: .space 4
        .end