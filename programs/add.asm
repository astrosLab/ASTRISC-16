	cal .start

.start
	ldi r0 5
	ldi r1 6
	add r0 r1 r2
	str r0 segment_display
	jmp .halt

.halt
	halt

#define segment_display 0xFFF2
