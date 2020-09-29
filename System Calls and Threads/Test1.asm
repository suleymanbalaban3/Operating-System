        ; 8080 assembler code
        .hexfile Test1.hex
        .binfile Test1.com
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
	mvi B, 1
	MVI A, TJOIN
	call GTU_OS
	
	LXI B, F2Before
	MVI A, TCREATE
	CALL GTU_OS
	mvi B, 2
	MVI A, TJOIN
	call GTU_OS
	
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


F2Before:
    mvi c, 1000	; init C with 1000
	mvi a, 0	; A = 0
F2:
    ADD c		; A = A + C
	DCR c		; --C
	JNZ F2	; goto loop if C!=0
	STA SUM		; SUM = A
	LDA SUM		; A = SUM
			; Now we will call the OS to print the value of sum
	MOV B, A	; B = A
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	MVI A, TEXIT	
	call GTU_OS	
	hlt		; end program

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

