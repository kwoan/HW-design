.global ON_KEY_PRESS
ON_KEY_PRESS: subi sp, sp, 16     ## save r22, r23, r21, ra at stack
              stw r22, (sp)
			  stw r23, 4(sp)
			  stw r21, 8(sp)
			  stw ra, 12(sp)
			  
			  movia r22, 0xFF200020   ## hex base
			  movia r21, 0xFF200050   ## key base
			  ldwio r23, 0xC(r21)     
			  stwio r23, 0xC(r21)     ## Turn off IRQ
			  
			  movia r10, 0x3f         ## word size bcd value 
			  movia r11, 0x0600     
			  movia r12, 0x5b0000
			  movia r13, 0x4f000000
			  
			  andi r4, r23, 0x01      ## detect which key pressed
			  bne r4, r0, Display_0
			  srli r23, r23, 0x01
			  andi r4, r23, 0x01
			  bne r4, r0, Display_1
			  srli r23, r23, 0x01
			  andi r4, r23, 0x01
			  bne r4, r0, Display_2
			  srli r23, r23, 0x01
			  andi r4, r23, 0x01
			  bne r4, r0, Display_3
			  

	
			  
ON_KEY_PRESS_END:
		      ldw r22, (sp)     ## reg restore
			  ldw r23, 4(sp)
			  ldw r21, 8(sp)
			  ldw ra, 12(sp)
			  addi sp, sp, 16
			  ret
			  
			  
Display_0: ldbio r7, (r22)
           bne r7, r0, OFF_0      ## detect hex0 on/off                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
           ldwio r7,(r22)         ## reload word 
		   or r7, r7, r10         ## to keep other hex digits
		   stwio r7,(r22)
		   br ON_KEY_PRESS_END
		   
Display_1: ldbio r7, 1(r22)
           bne r7, r0, OFF_1      ## detect hex1 on/off
           ldwio r7,(r22)         ## reload word 
		   or r7, r7, r11         ## to keep other hex digits
		   stwio r7,(r22)
		   br ON_KEY_PRESS_END
		   
Display_2: ldbio r7, 2(r22)
           bne r7, r0, OFF_2      ## detect hex2 on/off
           ldwio r7,(r22)         ## reload word 
		   or r7, r7, r12         ## to keep other hex digits
		   stwio r7,(r22)
		   br ON_KEY_PRESS_END
		   
Display_3: ldbio r7, 3(r22)        
           bne r7, r0, OFF_3      ## detect hex3 on/off
           ldwio r7,(r22)         ## reload word 
		   or r7, r7, r13         ## to keep other hex digits
		   stwio r7,(r22)
		   br ON_KEY_PRESS_END

OFF_0: 	ldwio r9, (r22)
		movia r8, 0xffffff00      ## off hex0/keep other hex digits
		and r9,r9,r8
		stwio r9, (r22)
        br ON_KEY_PRESS_END
		
OFF_1:  ldwio r9, (r22)
		movia r8,0xffff00ff      ## off hex1/keep other hex digits 
		and r9,r9,r8
		stwio r9, (r22)
        br ON_KEY_PRESS_END
		
OFF_2:  ldwio r9, (r22)
		movia r8,0xff00ffff      ## off hex2/keep other hex digits
		and r9,r9,r8
		stwio r9, (r22)
        br ON_KEY_PRESS_END
		
OFF_3:  ldwio r9, (r22)
		movia r8,0x00ffffff      ## off hex3/keep other hex digits
		and r9,r9,r8
		stwio r9, (r22)
        br ON_KEY_PRESS_END
			  

.end