FIB2: BEGIN

PUBLIC ND2
PUBLIC FIB

ST: EXTERN
LIMIT: EXTERN
CUR: EXTERN
ND: EXTERN
PREV: EXTERN
TEMP: EXTERN

FIB:   LOAD    CUR            ; Load the current Fibonacci number
       ADD     PREV           ; Add the previous Fibonacci number to ACC (calculating next Fibonacci number)
       STORE   TEMP           ; Store the result temporarily
       LOAD    PREV           ; Load the previous number into ACC
       STORE   CUR            ; Move it into CUR (this is the old current number)
       OUTPUT  CUR           ; Output the current Fibonacci number (from CUR)
       LOAD    TEMP           ; Load the new Fibonacci number into ACC
       STORE   PREV           ; Store it in PREV (this is now the current number for the next iteration)
       SUB     LIMIT          ; Subtract LIMIT from the current Fibonacci number
       JMPP    ND            ; If the result is positive, we've reached the limit, end
       JMP     FIB            ; Otherwise, continue calculating the sequence

ND2: STOP