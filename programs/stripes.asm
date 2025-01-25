#define color 0xFFF3
#define function 0xFFF4
#define parameters 0xFFF5

	jmp .start

.start
	ldi r2 65535
	ldi r3 1
	jmp .loop

.loop
	str r0 parameters
	str r1 color
	str r3 function
	inc r0 r0
	str r0 parameters
	str r2 color
	str r3 function
	inc r0 r0
	jmp .start

