.text
.global _start
_start: movia r5, TEST_NUM
        ldw r4, (r5)
		mov r6, r4
        call ONES
        call DISPLAY
		
END: br END

ONES: beq r4, r0, STOP
      beq r6, r0, LOOP
      srli r8, r6, 0x01   # r8 - shift value
	  and r6, r6, r8     
	  addi r7, r7, 0x01   # r7 - longest string of 1's
	  br ONES
	  
LOOP: addi r5, r5, 4      # to read each word of TEST_NUM
      ldw r4, (r5)
	  mov r6, r4
	  mov r9, r7          # r9 - to compare with max value
	  mov r7, r0
      bge r10, r9, ONES    # r10 - max value store
	  mov r10, r9
	  
	  br ONES
	  
STOP: mov r5, r0
      mov r7, r0
	  mov r8, r0
	  mov r9, r0
      ret
	  
   
DIVIDE: mov r5, r4     #r5를 사용해 연산 수행
        movi r7, 10    #divider
        movi r8, 0     # r8 초기화
        movia r9, RESULT    # r9 초기화
		mov r2, r0
		mov r3, r0
		movi r11, 0
				
CONT:  blt r5, r7, LOOP2  # divider보다 작을 때 까지 뺄셈 수행
       sub r5,r5,r7       # 나머지 계산
       addi r8,r8,1       # 몫 계산
       br CONT
	  
LOOP2: stb r5, (r9)      # 계산된 나머지(n번째 자리의 숫자) r9에 저장
       addi r9, r9, 1  # 다음자리 저장을 위해 주소+1 (32bit processor이므로 2자리가 건너뛴 것으로 나타남)
       mov r5, r8     # 다음 계산을 위해 몫 넘겨줌
       mov r11, r8     # divider보다 큰지 비교하기 위해 r11에도 몫 저장
	   movi r8, 0     # 다음 몫 계산을 위해 0으로 초기화
	   blt r11, r7, LOOP3   # divider보다 크다면 연산 다시 수행
	   br CONT
LOOP3: stb r11, (r9)    # 마지막 연산의 결과 저장
       ldb r3, (r9)     # r9 주소에 있는 값을 ldw하여 r2에 숫자 자체가 나타나도록 함. 
	   ldb r2, RESULT(r0)
	 
DIV_END: mov r5, r0
         mov r7, r0
		 mov r8, r0
		 mov r9, r0
		 mov r11, r0
		 
         ret



	
SEG7_CODE: movia r15, BIT_CODES # starting address of bit codes
           add r15, r15, r4 # index into the bit codes
           ldb r2, (r15) # read the bit code needed for our digit
           ret
		   
BIT_CODES: .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
           .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
		   .skip  2  # pad with 2 bytes to maintain word alignment

DISPLAY: mov r16, ra
         movia r17, 0xff200020
         mov r4, r10         # display r10 on HEX1−0
		 call DIVIDE         # ones digit will be in r2; tens digit in r3
		 mov r4, r2          # pass ones digit to SEG7_CODE
		 call SEG7_CODE
		 mov r14, r2         # save bit code
		 mov r4, r3          # retrieve tens digit, pass to SEG7_CODE
		 call SEG7_CODE
		 slli r2, r2, 8
		 or r14, r14, r2     # bit code for tens|bit code for ones
		 stwio r14, (r17)

FIN: callr r16
		 
		 
RESULT: .skip 4

TEST_NUM: .word 0x00aaaaaa, 0x00bbbbbb, 0x00cccccc, 0x00dddddd, 0x00eeeeee
          .word 0x00ffffff, 0x00111111, 0x00222222, 0x00333333, 0x00444444, 0
          .end