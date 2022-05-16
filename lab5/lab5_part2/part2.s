.text
.global _start
_start:
        movia sp, 0x20000     # stack 초기화
        movia r8, 0xFF200000 
      movia r9, 0xFF202000
      movia r22, 0xFF200050   # 각 주소 값 받아옴
   stwio r0,0(r8)            # LEDR값 초기화

      call  CONFIG_TIMER      #TIMER setting
      call  CONFIG_KEYS      #KEY setting
      
      movi    r23,    3      
        wrctl   ienable, r23   #interrupt받을 I/O지정
        
      movi    r23,    1
        wrctl   status, r23      #interrupt processing turn on (PIE=1)
IDLE:
        br IDLE

CONFIG_TIMER:
      movia r12, 0xe100
      stwio r12, 8(r9)
      movia r12, 0x05f5
      stwio r12, 12(r9)      # start value 설정 (1초)
      movi r12,0b0111         # start=1, CONT=1, ITO=1
      stwio r12, 4(r9)      # timer start 
      ret
      
CONFIG_KEYS:
        movi   r23,    0b1111   
        stwio  r23,    8(r22)   # key mask 설정 
      ret


.section .reset,"ax"
      movia r2,_start
      jmp r2
.section .exceptions,"ax"
.global ISR
ISR:
      subi sp, sp, 16      #stack 공간 마련
      stw et, 0(sp)      
      rdctl et, ctl4      #peding bit load
      beq et, r0,SKIP_EA_DEC   #et가 0이면 hardware interrupt가 아니므로 SKIP
      subi ea, ea, 4      #hardware interrupt이면 instruction 재실행 
SKIP_EA_DEC:
      stw ea, 4(sp)   
      stw ra, 8(sp)   #ea, ra저장
      stw r22, 12(sp)   
      rdctl et, ctl4   #peding bit load
      bne et, r0,ON_EXIT_INT   #hardware interrupt인 경우 분기
ON_TRAP:
      br END_ISR   #software interrupt의 경우 되돌아감
ON_EXIT_INT:  # timer를 먼저 판별하고 key에 대해 판별함
      andi r22, et, 0b01   # timer에 대한 interrupt인지 판별
      beq r22, r0, KEY   # timer에 대한 interrupt가 아니면 KEY로 분기
      call TIMER_INT      # timer interrupt에 대한 ISR로 분기
KEY:   
      andi r22, et, 0b10   # key에 대한 interrupt인지 판별
      beq r22, r0,END_ISR   # 아니라면 END_ISR
      call KEY_PRESS      # 맞다면 KEY에 대한 ISR로 분기

END_ISR:
      ldw et,0(sp)
      ldw ea, 4(sp)
      ldw ra, 8(sp)
      ldw r22, 12(sp)
      addi sp, sp, 16
      eret            # 스택에서 데이터 로드 후 원상복귀


.global KEY_PRESS

KEY_PRESS:   #KEY가 눌리면 timer의 stop과 start 토글
      movia r8, 0xFF200000
      movia r9, 0xFF202000 
      movia r10,  0xFF200050
      subi sp,sp,8
      stw r22,0(sp)
      stw r23,4(sp)      #스택에 data 저장
      
      ldwio r23, 0xC(r10)   #mask key값을 읽음
      stwio r23, 0xC(r10)   #다시 쓰면서 IRQ를 끈다
      
      beq r23, r0, END   # key mask bit가 0이면 END로 분기
      ldwio r4,4(r9)      # timer의 control register 값 load
      andi r4, r4, 0b0100   # START의 bit 뽑아냄
      beq r4, r0, START   # START=0이면 START로 분기
STOP:   
      movi r4, 0b1011      # STOP=1, START=0, CONT=1, ITO=1
      stwio r4, 4(r9)
      
      ldw r22,0(sp)
      ldw r23,4(sp)
      addi sp, sp,8
      ret               # 복귀
START:
      movi r4, 0b0111      # STOP=0, START=1, CONT=1, ITO=1
      stwio r4, 4(r9)      
      ldw r22,0(sp)
      ldw r23,4(sp)
      addi sp, sp,8      # 복귀
      ret
      
END:       
      ldw r22,0(sp)
      ldw r23,4(sp)
      addi sp, sp,8
      ret

.global TIMER_INT

TIMER_INT:

      movia r8, 0xFF200000
      movia r9, 0xFF202000 
      movia r10,  0xFF200050
      subi sp,sp,8
      stw r22,0(sp)
      stw r23,4(sp)      #스택에 데이터 저장
      
      ldwio r23, 0(r9)
      stwio r23, 0(r9)   # Turn off IRQ
      
      ldwio r23, 0(r8)   #read the previous value
      addi r23, r23, 1   #increment
      stwio r23, 0(r8)   #write the new value
      
      ldw r22,0(sp)
      ldw r23,4(sp)
      addi sp, sp,8
      ret               #복귀
      
      .end