#define color 0xFFF3
#define function 0xFFF4
#define parameters 0xFFF5

	cal .start

.start
	ldi r0 0
	ldi r1 1
	ldi r2 1
	ldi r3 0 
	cal .loop

.loop
	str r3 color
	str r0 parameters
	str r1 function
	add r0 r2 r0
	add r2 r3 r3
	jmp .loop
	
