		addi $t0, $0, 4 968
		lh $s0, 0($t0)	// $s0 = a;
		lw $s1, 4($t0)	// $s1 = b;
		lw $s2, 8($t0)	// $s2 = i;
Loop: 	slti $t1, $s2, 1000
		beq $t1, $0, 1008
		slt $t1, $s0, $s1
		beq $t1, $0, 1008
		add $s1, $s1, $s0
		j 984
Exit:	addi $t2, $s2, -1
		sw $t2, 12(t0)
		lw $s3, 12(t0)
		halt 