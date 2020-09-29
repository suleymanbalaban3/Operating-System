        ; 8080 assembler code
        .hexfile Test4.hex
        .binfile Test4.com
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
TYIELD      equ 10
TCREATE     equ 11


    ; Position for stack pointer
stack   equ 0F000h

    org 000H
    jmp begin

    ; Start of our Operating System
GTU_OS: PUSH D
    push D
    push H
    push psw
    nop ; This is where we run our OS in C++, see the CPU8080::isSystemCall()
        ; function for the detail.
    pop psw
    pop h
    pop d
    pop D
    ret
    ; ---------------------------------------------------------------
    ; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

    ; This program adds that sorts the numbers in increasing order 
    ; and prints the results on the screen.

    

numbers:    dw 0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,
numbers1:    dw 0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,0H,
string: dw '*********** Incremented Order Sorted ***********',00AH,00H ;
string1: dw '************* Result of Search Adress *************',00AH,00H ;
error:  dw 'error',00H
begin:
    LXI SP,stack    ;always initialize the stack pointer
    mvi L, 50        
    LXI D,numbers   
    
    LXI B, f1
	MVI A, TCREATE
	CALL GTU_OS
	
	mvi L, 50
	LXI D,numbers
	LXI B, f3
	MVI A, TCREATE
	CALL GTU_OS
    
    hlt

    
;-------------------------- Generate Random NUmbers -----------------------------
f1:
        MVI A,GET_RND           
        CALL GTU_OS
        MOV A,B 
        STAX D
        LDAX D
        mov B,A
        MVI A, PRINT_B          
        call GTU_OS 
        INR E                  
        INR E                  
        DCR L
        JNZ f1 
        mvi L, 50       
         
;-------------------------- Sort -----------------------------     
start:
    mvi c, 49       
    LXI D,numbers  
    DCR L          
    JZ finish       

mainLoop:
    LDAX D 
    MOV B,A         
    INR E           
    INR E           
    LDAX D 
    MOV H,A        
    jmp control1
    
continue:    
    DCR C           
    JZ  start       
    JMP mainLoop    
    
control1:
    mvi A,0
    sub H           
    jm control2     
    jmp control3    
    
control2:    
    mvi A,0
    sub B           
    jm check        
    jmp swap        
    
control3:    
    mvi A,0
    sub B           
    jp check        
    jmp continue     

check:
    mov A,H
    sub B           
    jm swap         
    jmp continue    


swap:
    MOV A,B         
    STAX D          
    DCR E           
    DCR E           
    MOV A,H
    STAX D           
    INR E           
    INR E           
    JMP continue
    
finish:
    LXI D,numbers   
    mvi L, 50       
    LXI B, string   
    MVI A, PRINT_STR    
    call GTU_OS 

print:
    LDAX D          
    MOV B,A         
    MVI A, PRINT_B  
    call GTU_OS    
    INR E           
    INR E           
    DCR L           
    JNZ print   
    MVI A, TEXIT	
	call GTU_OS	   
    hlt




    
    
    
    
    
    
    
    
    
;-------------------------- Generate Random Numbers-2 -----------------------------
f3:
        MVI A,GET_RND           
        CALL GTU_OS
        MOV A,B 
        STAX D
        LDAX D
        mov B,A
        MVI A, PRINT_B          
        call GTU_OS 
        INR E                  
        INR E                  
        DCR L
        JNZ f3 
        mvi L, 50       
         
;-------------------------- Sort-2 -----------------------------     
startSecond:
    mvi c, 49       
    LXI D,numbers1  
    DCR L          
    JZ finishSecond       

mainLoopSecond:
    LDAX D 
    MOV B,A         
    INR E           
    INR E           
    LDAX D 
    MOV H,A        
    jmp control1Second
    
continueSecond:    
    DCR C           
    JZ  startSecond       
    JMP mainLoopSecond    
    
control1Second:
    mvi A,0
    sub H           
    jm control2Second     
    jmp control3Second    
    
control2Second:    
    mvi A,0
    sub B           
    jm checkSecond        
    jmp swapSecond        
    
control3Second:    
    mvi A,0
    sub B           
    jp checkSecond        
    jmp continueSecond     

checkSecond:
    mov A,H
    sub B           
    jm swapSecond         
    jmp continueSecond    


swapSecond:
    MOV A,B         
    STAX D          
    DCR E           
    DCR E           
    MOV A,H
    STAX D           
    INR E           
    INR E           
    JMP continueSecond
    
finishSecond:
    LXI D,numbers1   
    mvi L, 0       
    
    jmp begin2 
;-------------------------- Search-2/1 -----------------------------

begin2:   
    
    mvi C, 50                   
    mvi H, 0
    LXI D,numbers1  
    jmp read2
    
read2:                           

    MVI A, READ_B               
    call GTU_OS                 
    MOV L,B                    

loop2:   
    LDAX D                  
    CMP L                   
    JZ found2                
    INR E                   
    INR E
    INR H                   
    DCR C
    JNZ loop2
    jmp notFound2

found2:
    LXI B, string1   
    MVI A, PRINT_STR          
    call GTU_OS 
    MOV B,H                 
    MVI A, PRINT_B          
    call GTU_OS 
    jmp begin22
        
notFound2:
    LXI B, string1   
    MVI A, PRINT_STR          
    call GTU_OS 
    LXI B,error               
    MVI A, PRINT_STR        
    call GTU_OS 

        
        
      
;-------------------------- Search-2/2 -----------------------------

begin22:              
    mvi C, 50    
    mvi L, 0               
    mvi H, 0
    ;LXI D,numbers1  
    jmp read22
    
read22:                           

    MVI A, READ_B               
    call GTU_OS                 
    MOV L,B                    

loop22:   
    LDAX D                  
    CMP L                   
    JZ found22               
    INR E                   
    INR E
    INR H                   
    DCR C
    JNZ loop22
    jmp notFound22

found22:
    LXI B, string1   
    MVI A, PRINT_STR          
    call GTU_OS 
    MOV B,H                 
    MVI A, PRINT_B          
    call GTU_OS 
    jmp begin222
        
notFound22:
    LXI B, string1   
    MVI A, PRINT_STR          
    call GTU_OS 
    LXI B,error               
    MVI A, PRINT_STR        
    call GTU_OS 
        
        
        
;-------------------------- Search-2/3 -----------------------------

begin222:     
    mvi L, 0         
    mvi C, 50                   
    mvi H, 0
    ;LXI D,numbers1  
    jmp read222
    
read222:                           

    MVI A, READ_B               
    call GTU_OS                 
    MOV L,B                    

loop222:   
    LDAX D                  
    CMP L                   
    JZ found222                
    INR E                   
    INR E
    INR H                   
    DCR C
    JNZ loop222
    jmp notFound222

found222:
    LXI B, string1   
    MVI A, PRINT_STR          
    call GTU_OS 
    MOV B,H                 
    MVI A, PRINT_B          
    call GTU_OS 
    jmp ending                     
        
notFound222:
    LXI B, string1   
    MVI A, PRINT_STR          
    call GTU_OS 
    LXI B,error               
    MVI A, PRINT_STR        
    call GTU_OS 
    
  
ending:
    MVI A, TEXIT	
	call GTU_OS	
    hlt              
        
