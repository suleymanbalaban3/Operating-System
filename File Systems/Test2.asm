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
FILECREATE  equ 12
FILECLOSE   equ 13
FILEOPEN    equ 14
FILEREAD    equ 15
FILEWRITE   equ 16
FILESEEK    equ 17
DIRREAD     equ 18

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

data: dw 'merhaba dunya. Bu file system icin ornek metindir.',00AH,00H ;
asking: dw 'Enter for a filename',00AH,00H ;
sum	ds 2 ; will keep the sum
begin:
	LXI SP,stack 				
    
    LXI B, asking
	MVI A, PRINT_STR
	CALL GTU_OS
	
	MVI A, READ_STR
	CALL GTU_OS
	
	MVI A, FILEOPEN
	CALL GTU_OS
	
	MOV H, B
	
    MVI D, 92
    MOV E, H
	MVI A, FILEREAD
	CALL GTU_OS
	
	MVI A, PRINT_STR
	CALL GTU_OS


	HLT		
