.text
.global _start

_start:  movia r7, 0xFF200050   ## KEY0 baseaddr
       movia r20, 0xFF200020    ## HEX3_HEX0 base addr
       mov r9, r0               ## HEX display digit
       movi r11, 9              ## Prevent overflow (for one digit)
       movia sp, 0x20000

MAIN: call SEG7_CODE 
      stwio r2, (r20)
      
     
     
KEY_PRESS_WAIT:   ldwio r6, 0(r7)             ## polling KEY press
                  beq r6, r0, KEY_PRESS_WAIT  ## polling KEY press
            
KEY_RELEASE_WAIT: ldwio r5, 0(r7)               ## polling KEY release
                  bne r5, r0, KEY_RELEASE_WAIT  ## polling KEY release
              br KEY_DETECT
              
KEY_DETECT:                    
              andi r4, r6, 0x1               ## if KEY press, KEY value=0
              bne  r4, r0, RESET_CALL        ## detect index of KEY value=0
              srli r6, r6, 0x1
              andi r4, r6, 0x1
              bne  r4, r0, INCREMENT_CALL
              srli r6, r6, 0x1
              andi r4, r6, 0x1
              bne  r4, r0, DECREMENT_CALL
              srli r6, r6, 0x1
              andi r4, r6, 0x1
              bne  r4, r0, OFF_CALL



INCREMENT_CALL: call INCREMENT
                br MAIN
DECREMENT_CALL: call DECREMENT
                br MAIN
RESET_CALL:     call RESET
                br MAIN
OFF_CALL:       call OFF
                br MAIN


INCREMENT:  bge   r9, r11, MAIN
            addi  r9, r9, 1 
            ret
DECREMENT:  beq   r9, r0, MAIN
            subi  r9, r9, 1
            ret
RESET:      mov   r9, r0
            ret
OFF:        subi  sp, sp, 4
            stw   ra, 0(sp)
			
            call  RESET
            movi  r10, 1
			
			ldw   ra, 0(sp)
			addi  sp, sp, 4
			
            ret
            
            

           
       
       
SEG7_CODE: bne r10, r0, SEG7_OFF
           movia r15, BIT_CODES   # starting address of bit codes
           add r15, r15, r9       # index into the bit codes
           ldb r2, (r15)          # read the bit code needed for our digit
           ret
SEG7_OFF:  ldw r2, HEX_OFF(r0)
           mov r10, r0
           ret
         
BIT_CODES: .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
           .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
         .skip  2    # pad with 2 bytes to maintain word alignment
HEX_OFF: .word 0b00000000
         .end
   