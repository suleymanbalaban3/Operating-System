        ; 8080 assembler code
        .hexfile Test2.hex
        .binfile Test2.com
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
TEXIT       equ 8
TJOIN       equ 9
TCREATE     equ 11

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


sum	ds 2 ; will keep the sum
begin:
	LXI SP,stack 				
    
	LXI B, F1Before
	MVI A, TCREATE
	CALL GTU_OS
	;mov B, 1
	;MVI A, TJOIN
	;call GTU_OS
	
	LXI B, F3Before
	MVI A, TCREATE
	CALL GTU_OS
	
	HLT		

F1Before:
    mvi c, 51	
	mvi b, 0	
F1:	
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR B
	DCR c		; --C
	JNZ F1  	; goto loop if C!=0
	MVI A, TEXIT	
	call GTU_OS	
	hlt		    ; end program


F3Before:
    mvi c, 51	
	mvi b, 50
F3:
    MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR B
	DCR c		; --C
	JNZ F3  	; goto loop if C!=0
	MVI A, TEXIT	
	call GTU_OS	
	hlt		    ; end program

