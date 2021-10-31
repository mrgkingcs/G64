		;!to "tiny.o", cbm	; set output file and format -- G64 piggy-backs on C64
		*= $1000		; set program counter
		
		lda #$00
		sta $0004
		lda #$C0
		sta $0005

		ldy #0

		ldx #0
		beq +

-		sta ($04), y
		iny
		
		inx
+		lda .string,x
		bne -
		
		rts
		
		
.string	!text "Hello world!", 0	