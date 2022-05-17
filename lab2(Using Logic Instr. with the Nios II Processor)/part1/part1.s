.text
.global _start
_start:
	ldw r9, TEST_NUM(r0)
	
	mov r10, r0
	
LOOP: beq r9, r0, END
      srli r11, r9, 0x01
	  and r9, r9, r11
	  addi r10, r10, 0x01
	  br LOOP

END: br END

TEST_NUM: .word 0x3fabedef
          .end