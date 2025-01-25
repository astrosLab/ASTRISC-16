#define color 0xFFF3
#define function 0xFFF4
#define parameters 0xFFF5

	cal .start

.start
	ldi r0 2
	ldi r1 10
	ldi r2 1
	ldi r3 1
	ldi r4 65535
	ldi r5 1
	cal .loop

.loop
	ldi r4 0
	str r7 parameters
	str r4 color
	str r5 function
	ldi r4 65535
	add r0 r2 r0
	add r1 r3 r1
	shl r0 r7
	shl r7 r7
	shl r7 r7
	shl r7 r7
	shl r7 r7
	add r1 r7 r7
	str r7 parameters
	str r4 color
	str r5 function
	jmp .compare

.compare
	ldi r6 0
	cmp r0 r6
	brh-z .vx_min
	cmp r1 r6
	brh-z .vy_min
	ldi r6 31
	cmp r0 r6
	brh-z .vx_max
	ldi r6 25
	cmp r1 r6
	brh-z .vy_max
	jmp .loop

.vx_min
	ldi r2 1
	jmp .loop

.vy_min
	ldi r3 1
	jmp .loop

.vx_max
	ldi r2 -1
	jmp .loop

.vy_max
	ldi r3 -1
	jmp .loop

