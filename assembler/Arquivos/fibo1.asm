FIB1: BEGIN   
FIB: EXTERN
ND2: EXTERN
PUBLIC ST 
PUBLIC ND
PUBLiC PREV
PUBLIC CUR
PUBLIC TEMP
PUBLIC LIMIT
ST:    INPUT   LIMIT          ; Input the limit for Fibonacci sequence
       LOAD    ZERO           ; Load the initial Fibonacci number (0) to ACC
       STORE   CUR            ; Store 0 in CUR
       LOAD    ONE            ; Load the second Fibonacci number (1) to ACC
       STORE   PREV           ; Store 1 in PREV (current becomes previous for the next iteration)
       STORE   TEMP           ; Store 1 in TEMP (this is the first number to be output)
       JMP   FIB            ; Jump to the FIB subroutine to start the sequence
       

       
ND:   JMP ND2                   ; End of the program

; Variables and constants
PREV:  SPACE                  ; Space for the previous Fibonacci number
CUR:   SPACE                  ; Space for the current Fibonacci number
TEMP:  SPACE                  ; Temporary storage for the new Fibonacci number
LIMIT: SPACE                  ; Space for the user-specified limit
ZERO:  CONST  0               ; Constant for the initial Fibonacci number
ONE:   CONST  1               ; Constant for the second Fibonacci number
END