
.sdata
end_of_program: 0
out_data_size: 0

.text
		nop
		s_d64_st              $zero,$one // *end_of_program = 1
infinite_loop:
		jmp                   infinite_loop
		// Min 6 nops after jmp
		nop
		nop
		nop
		nop
		nop
		nop
