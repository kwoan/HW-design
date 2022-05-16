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

ON_EXT_INT: andi r22, et, 0b10
            beq r22, r0, END_ISR   # no interrupt
			call ON_KEY_PRESS

END_ISR: ldw et, (sp)
         ldw ea, 4(sp)
		 ldw ra, 8(sp)
		 ldw r22, 12(sp)
		 addi sp, sp, 16     ## reg restore
		 eret
                    .end