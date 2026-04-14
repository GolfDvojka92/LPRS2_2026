
.text
		nop
infinite_loop:
		jmp                   infinite_loop
		// Min 6 nops after jmp
		nop
		nop
		nop
		nop
		nop
		nop
