.text
.global _start
_start:
	movia sp, 0x20000       ## stack set at dram
	movia r8, 0xFF200000    ## LEDR base addr
	movia r9, 0xFF202000    ## timer base addr
	movia r10, 0xFF200050   ## key base addr
	stwio r0, 0(r8)
	
	call CONFIG_TIMER
	call CONFIG_KEYS
		
	movi r23, 3
	wrctl ienable, r23      ## set interrupt enable key/timer 
	movi r23, 1
	wrctl status, r23       ## turn on Nios II interrupt processing
	
	
IDLE: br IDLE

CONFIG_TIMER: 

	
	movia r11, 0xe100       ## period_l setting
	stwio r11, 8(r9)
	movia r11, 0x05f5       ## period_h setting
	stwio r11, 0xC(r9)
	
	movi r11, 0b10          ## set timer status
	stwio r11, 0(r9)
    
	movi  r11, 0b0111        ## set timer control interrupt enable
	stwio r11, 4(r9)
	
	ret

CONFIG_KEYS: 
    movi r12, 0b1111       ## set all interrupt mask bit 1
	stwio r12, 8(r10)
	ret
.section .reset, "ax"
                   movia r2, _start
				   jmp r2
.section  .exceptions, "ax"
.global ISR 

ISR: subi sp, sp, 16    ## et, ea, ra, r22 will be stored stack
     stw et, (sp) 
	 rdctl et, ctl4     ## read ipending register
	 beq et, r0, SKIP_EA_DEC   ## if hw interrupt occur
	 subi ea, ea, 4      ## to handle exception occur reason
	 
SKIP_EA_DEC: stw ea, 4(sp)   
             stw ra, 8(sp)
			 stw r22, 12(sp)
			 rdctl et, ctl4
			 bne et, r0, ON_EXT_INT    
 
ON_TRAP: br END_ISR      ## not define sw interrupt

ON_EXT_INT: andi r22, et, 0b01
            beq  r22, r0, KEY_INT
			call TIMER_sec		 


END_ISR: ldw et, (sp)
         ldw ea, 4(sp)
		 ldw ra, 8(sp)
		 ldw r22, 12(sp)
		 addi sp, sp, 16     ## reg restore
		 eret

KEY_INT: andi r22, et, 0b10
         beq r22, r0, END_ISR
         call ON_KEY_PRESS	
		 br END_ISR
		 

.global ON_KEY_PRESS
ON_KEY_PRESS: subi sp, sp, 20     ## save reg at stack
              stw r22, (sp)
			  stw r23, 4(sp)
			  stw r21, 8(sp)
			  stw r4, 12(sp)
			  stw ra, 16(sp)
			  
			  movia r22, 0xFF200050  ## key base addr
			  ldwio r23, 0xC(r22)    ## key edgebit
			  stwio r23, 0xC(r22)    ## turn off IRQ
			  
			  movia r20, 0xFF202000  ## Timer base addr
			  ldwio r23, 4(r20)      ## Timer control reg check
			  
			  andi  r21, r23, 0b0100
			  beq  r21, r0, START     ## if TIMER run 
			  br STOP
			  			  
ON_KEY_PRESS_END:
              ldw r22, (sp)
			  ldw r23, 4(sp)
			  ldw r21, 8(sp)
			  ldw r4, 12(sp)
			  ldw ra, 16(sp)
			  addi sp, sp, 20
			  ret
START: 
			  movi r4, 0b0111
			  stwio r4, 4(r20)
			  br ON_KEY_PRESS_END

STOP:         movi r4, 0b1011
              stwio r4, 4(r20)
			  br ON_KEY_PRESS_END
			  
			  
			  
.global TIMER_sec
TIMER_sec:  subi sp, sp, 16     ## save reg at stack
            stw r20, (sp)
		    stw r21, 4(sp)
			stw r22, 8(sp)
			stw ra, 12(sp)
			
			movia r20, 0xff202000   ## Timer base addr
			movia r21, 0xff200000   ## LEDR base addr
			
			movi r22, 0b10        ## turn off IRQ
			stwio r22, (r20)
			
			ldwio r22, (r21)        
			addi r22, r22, 1        ## real second ++ at ledr
			stwio r22, (r21)
			
			ldw r20, (sp)
			ldw r21, 4(sp)
			ldw r22, 8(sp)
			ldw ra, 12(sp)
			addi sp, sp, 16
			
			ret

.end
			  