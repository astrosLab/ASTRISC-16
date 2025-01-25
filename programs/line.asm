#define color 0xFFF3
#define function 0xFFF4
#define parameter 0xFFF5

	cal .start

.start
	ldi r0 0
	ldi r1 5
	ldi r2 1
	ldi r3 1
	ldi r4 -5
	ldi r5 65535
	str r5 color
	ldi r5 1
	cal .loop

.loop


