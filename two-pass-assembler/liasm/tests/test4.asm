LOAD   N1       ; Lê o primeiro número
N1:     CONST 20   
N2:     CONST -64
FINAL:  CONST -44
N3:     SPACE            ; Reserva espaço para o resultado


LOAD   N2       ; Lê o segundo número
LOAD    N1      ; Carrega N1 no acumulador
ADD     N2      ; Soma está no acumulador
STORE   N3      ; Coloca soma N1+N2 em N3
LOAD    FINAL
SUB     N3

; Testa se o programa ignora linhas vazias e comentários

JMPZ    OK
THROW

; Testa labels em lugares difrenetes

OK:     NOP
STOP                  