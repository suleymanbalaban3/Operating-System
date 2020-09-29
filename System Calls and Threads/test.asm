        ; 8080 assembler code
        .hexfile test.hex
        .binfile test.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
GET_RND     equ 7

	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        



begin:

	LXI SP,stack 				

	
	MVI A,READ_B  				;READ_B and PRINT_B testing
	CALL GTU_OS
	MVI A, PRINT_B
	CALL GTU_OS

	MVI A,READ_MEM     			;READ_MEM and PRINT_MEM testing
	CALL GTU_OS
	MVI A,PRINT_MEM    			
	CALL GTU_OS
	
	MVI B,12 				    ;READ_STR and PRINT_STR testing
	MVI C,1
	MVI A,READ_STR
	CALL GTU_OS
	MVI B,12  					
	MVI C,1
	MVI A,PRINT_STR
	CALL GTU_OS

	

	MVI A,GET_RND  				;GET_RND and PRINT_B testing
	CALL GTU_OS
	MVI A, PRINT_B
	CALL GTU_OS

	HLT		
