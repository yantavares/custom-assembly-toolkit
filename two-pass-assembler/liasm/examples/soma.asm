INPUT   N1      ; Lê o primeiro número
INPUT   N2      ; Lê o segundo número
LOAD    N1      ; Carrega N1 no acumulador
ADD     N2      ; Soma está no acumulador
STORE   N3      ; Coloca soma N1+N2 em N3
OUTPUT  N3      ; Escreve o resultado
STOP            ; Termina a execução do programa
N1:     SPACE   ; Reserva espaço para o primeiro número
N2:     SPACE   ; Reserva espaço para o segundo número
N3:     SPACE   ; Reserva espaço para o resultado

