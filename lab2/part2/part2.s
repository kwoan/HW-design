.text
.global _start
_start: 
	movia r5, TEST_NUM   # r5 - addr of TEST_NUM
	ldw r4, (r5)            # r4 - value of TEST_NUM
	mov r8, r4           # r8 - argument reg of ONES r4와 같은 값.  Shift/and연산 수행을 위해 저장한다.
	call ONES
	
END:  br END
	
ONES: beq r4, r0, STOP    # TEST_NUM 값이 0 이면 STOP 
      beq r8, r0, LOOP    # 연산결과가 0이면 
      srli r6, r8, 0x01      # r6 - shift value
      and r8, r8, r6     
      addi r10, r10, 0x01  # r10 - longest string of 1s
      br ONES
	  
LOOP:  addi r5, r5, 4      # to read each word of TEST_NUM
       ldw  r4, (r5)       # r4 update
       mov r8, r4         # r8 update
       mov r11, r10        # r11 - to compare with max
mov r10, r0         # r10 다음 연산 count를 위해 초기화
       bge  r7, r11, ONES    # r7 - max value store
       mov r7, r11          # max보다 큰 값이 나타나면 저장
       br ONES

STOP:  mov r2, r7           # r2로 최댓값 return
       ret

TEST_NUM: .word 0x00aaaaaa, 0x00bbbbbb, 0x00cccccc, 0x00dddddd, 0x00eeeeee
          .word 0x00ffffff, 0x00111111, 0x00222222, 0x00333333, 0x00444444, 0
          .end
