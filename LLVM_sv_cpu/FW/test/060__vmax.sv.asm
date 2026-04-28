

#define R_i $t1
#define R_N $t2
#define R_tmp $t3
#define R_addr $t4

.sdata
end_of_program: 0


.vdata
x:
	0
	1
	2
	3
y:
	4
	3
	2
	1
max:
	0
	0
	0
	0


.text
		nop
		s_d64_lea             $t0 = $0, max
		nop
		v_d64_ld              %t0 = $zero, x
		v_d64_ld              %t1 = $zero, y
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		v_i64_cmp_u_l         %p0 = %t0, %t1
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	(y)	v_d64_mov            %t2 = %t1
	(n)	v_d64_mov            %t2 = %t0
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		v_d64_st              %t2, $t0
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		
		nop
		nop
		nop
		nop
		nop
		nop
		nop

		nop
		nop
		nop
		nop
		nop
		s_d64_st              $0, $1 // *end_of_program = 1
infinite_loop:
		jmp infinite_loop
		// Min 6 nops after jmp
		nop
		nop
		nop
		nop
		nop
		nop
