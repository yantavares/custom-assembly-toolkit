       LOAD    N
FAT:   SUB     ONE
       JMPZ   FIM
       STORE   AUX
       MUL    N
       STORE   N
       LOAD    AUX
       JMP    FAT
FIM:   LOAD N
       SUB FINAL
       JMPZ  OK
       THROW
OK:    STOP
AUX:   SPACE
N:     CONST  5
FINAL: CONST 120
ONE:   CONST   1
