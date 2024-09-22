LOAD   N1      ; Lê o primeiro número
LOAD   N2      ; Lê o segundo número
LOAD    N1      ; Carrega N1 no acumulador
ADD     N2      ; Soma está no acumulador
STORE   N3      ; Coloca soma N1+N2 em N3
LOAD    FINAL
SUB     N3
JMPZ    OK
THROW
OK:     STOP            ; Termina a execução do programa
N1:     CONST 20   
N2:     CONST 22
FINAL:  CONST 42
N3:     SPACE   ; Reserva espaço para o resultado

