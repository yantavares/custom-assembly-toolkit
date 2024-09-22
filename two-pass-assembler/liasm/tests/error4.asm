; IDK

       LOAD    N
FAT:   SUB     ONE
       JMPZ   FIM
       STORE  ACC ; Reserved word
       MUL    N
       STORE  N
       LOAD   ACC
       JMP    FAT
FIM:   LOAD N
       SUB FINAL
       JMPZ  OK
       THROW
OK:    STOP
ACC:   SPACE
N:     CONST  5
FINAL: CONST 120
ONE:   CONST   1
