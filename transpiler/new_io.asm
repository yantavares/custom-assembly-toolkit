%define INPUT_BUFFER [ebp+12]
%define BUFFER [ebp+12]
%define NEG_FLAG dword[ebp-4]

MAX_BUFFER_SIZE_INPUT equ 12 ; Maximum size of the input buffer = 32 bit integer + '\n' + '\0'

section .text
global _start, input, output, OVERFLOW, s_input, s_output

OVERFLOW:
	mov eax, 4                    
    mov ebx, 1                    
    mov ecx, output_overflow                     
    mov edx, len_overflow
    int 0x80

	mov eax, 4                   ; sys_write
    mov ebx, 1                   ; stdout
    mov ecx, newline             ; Newline character
    mov edx, 1                   ; Length of newline
    int 0x80

    mov eax, 1
    mov ebx, 0
    int 0x80

; -----------------------------------------------
; Input: Read a decimal value from stdin and convert it to an integer
;        EBP+8 - Pointer to variable to store the result
;        EBP+12 - Pointer to the buffer to store the input
; Output: Integer in EAX - Bytes read
input:
    enter 0, 0
    mov edi, [ebp+8]                   ; Destination pointer for result
    mov esi, INPUT_BUFFER              ; Input buffer pointer

    ; Read input from stdin
    mov eax, 3
    mov ebx, 0                         ; stdin
    mov ecx, esi                       ; Buffer pointer
    mov edx, MAX_BUFFER_SIZE_INPUT     ; Buffer size
    int 0x80                           ; Perform syscall to read from stdin

    pusha
    push eax                          ; Save the number of bytes read
    call output_read                  ; Print the input message
    popa

    mov ecx, eax                       ; Store number of bytes read to ecx
    mov [edi], eax                     ; Store bytes read at the result pointer location for return

    dec ecx                            ; Adjust for newline character
    movzx eax, byte [esi]              ; Load first character to check for negative

    cmp al, '-'
    jne .convert_positive

; Handle negative number
.negative:
    inc esi                            ; Increment buffer pointer to skip '-'
    dec ecx                            ; Adjust length excluding '-'
    call convert_to_int
    neg eax                            ; Convert result to negative
    jmp .store_result

; Handle positive number
.convert_positive:
    call convert_to_int                ; Call conversion on unmodified buffer

.store_result:
    mov [edi], eax                     ; Store the converted integer
    leave
    ret

; -----------------------------------------------
; Convert buffer to integer
; Assumes buffer pointer in esi and length in ecx
convert_to_int:
    enter 0, 0
    xor ebx, ebx                       ; Clear ebx for accumulating the result
    .loop_convert:
        test ecx, ecx                  ; Check if there's more characters to process
        jz .done                       ; If zero, finish
        movzx eax, byte [esi]          ; Load next character
        sub eax, '0'                   ; Convert from ASCII to integer
        imul ebx, ebx, 10              ; Multiply current result by 10
        add ebx, eax                   ; Add new digit to result
        inc esi                        ; Move to next character
        dec ecx                        ; Decrease count
        jmp .loop_convert
    .done:
        mov eax, ebx                   ; Place result in eax
        leave
        ret

; -----------------------------------------------
; Show output message and print integer result
; Input: EBP+8 - Pointer to the integer value to print
output_read:
    enter 0, 0
    mov edi, [ebp + 8]            ; Get the integer to print
    mov esi, buffer               ; Buffer to store the converted string

    ; Print the output message
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    mov ecx, read_msg             ; Message to print
    mov edx, len_read_msg         ; Length of the message
    int 0x80

    ; Convert the integer to string
    push edi                      ; Push integer value
    push esi                      ; Push pointer to buffer
    call output_to_string         ; Convert integer to string
    pop esi                       ; Restore buffer pointer
    pop edi                       ; Restore integer value

    ; Print the converted string
	mov edx, ecx
	mov ecx, eax
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    int 0x80

    ; Print a new line
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    mov ecx, newline              ; Newline character
    mov edx, 1                    ; Length of newline
    int 0x80
    leave
    ret 4


; -----------------------------------------------
; Show output message and print integer result
; Input: EBP+8 - Pointer to the integer value to print
output_written:
    enter 0, 0
    mov edi, [ebp + 8]            ; Get the integer to print
    mov esi, buffer               ; Buffer to store the converted string

    ; Print the output message
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    mov ecx, written_msg           ; Message to print
    mov edx, len_written_msg       ; Length of the message
    int 0x80

    ; Convert the integer to string
    push edi                      ; Push integer value
    push esi                      ; Push pointer to buffer
    call output_to_string             ; Convert integer to string
    pop esi                       ; Restore buffer pointer
    pop edi                       ; Restore integer value

    ; Print the converted string
	mov edx, ecx                  ; Length of string
    mov ecx, eax                      ; Push pointer to string
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    int 0x80

    ; Print a new line
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    mov ecx, newline             ; Newline character
    mov edx, 1                    ; Length of newline
    int 0x80
    leave
    ret 4

; --------------------------------------------------
; Output: Print an integer to stdout
; Input: EBP+8 - Pointer to the integer to print
;        EBP+12 - Buffer to store the converted string
output:
    enter 4, 0
    mov dword [ebp - 4], 0          ; Clear the negative flag

    mov edi, [ebp + 8]          ; Get the pointer to the integer value
    mov esi, INPUT_BUFFER       ; Buffer to store the string representation
    mov eax, [edi]              ; Load integer into EAX for checking

    ; Check if the number is negative
    cmp eax, 0
    jge .positive_output        ; If non-negative, jump to positive case

    ; Handle negative numbers
    neg eax                     ; Negate the number
    mov NEG_FLAG, 1           ; Set the negative flag

.positive_output:
    ; Convert the integer to a string
    push eax                    ; Preserve integer value
    push esi                    ; Push buffer pointer
    call output_to_string       ; Convert integer to string
    pop esi                     ; Restore buffer pointer
    pop ebx                     

    cmp dword NEG_FLAG, 1           ; Check if the number was negative
    jne .end_output             ; If not negative, jump to done

    dec eax
    mov byte [eax], '-'
    inc ecx

.end_output:
    push eax                    ; Pointer to string
    mov eax, 4                  ; sys_write
    mov ebx, 1                  ; stdout
    mov edx, ecx                ; Length of the string
    pop ecx                     ; Pointer to string
    int 0x80

    mov ecx, edx                ; Store string length in ECX

    pusha

    ; Print a new line
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    mov ecx, newline             ; Newline character
    mov edx, 1                    ; Length of newline
    int 0x80

    popa

    ; Call show_output_msg to display output
    pusha
    push ecx                    ; Push length of string
    call output_written         ; Call show_output_msg
    popa

    ; Return the length of the string in EAX
    mov eax, ecx
    leave
    ret

; -----------------------------------------------
; Convert an integer to a string
; Input: EBP+8 - Pointer to the buffer to store the string
;        EBP+12 - Integer to convert
; Output: Pointer to the beginning of the string in EAX
output_to_string:
    enter 0, 0                 ; Set up the stack frame
    
    mov eax, [ebp + 12]        ; Load the integer to convert (from EBP+12) into EAX
    mov esi, [ebp + 8]         ; Load the destination buffer address (from EBP+8) into ESI
    add esi, 9                 ; Move to the end of the buffer
    mov byte [esi], 0          ; Null-terminate the string (set last byte to 0)
    
    mov ecx, 0                 ; ECX will count the digits
    mov ebx, 10                ; Set divisor to 10 (for base-10 division)
    
    .next_digit:
        inc ecx                ; Increment the digit counter
        xor edx, edx           ; Clear EDX (high part of EAX) before division
        div ebx                ; Divide EAX by 10, result in EAX, remainder in EDX
        add dl, '0'            ; Convert the remainder (DL) to ASCII by adding '0'
        dec esi                ; Move back in the buffer
        mov [esi], dl          ; Store the ASCII digit in the buffer
        test eax, eax          ; Check if EAX (quotient) is zero
        jnz .next_digit        ; If not zero, continue with the next digit

        mov eax, esi           ; Return the pointer to the beginning of the string (ESI)
        leave                  ; Restore the stack frame
        ret                    ; Return to the caller


; --------------------------------------------------
; Input: EBP+8 - Pointer to the memory location with the data to print
;        EBP+12 - Number of characters to print
; Output: Prints the data to stdout, returns number of bytes written in EAX
s_output:
    enter 0, 0
    
    ; Get parameters from the stack
    mov ecx, [ebp + 8]  ; Memory address of the data to be printed (buffer)
    mov edx, [ebp + 12] ; Number of characters to print

    ; Syscall for writing data to stdout
    mov eax, 4          ; Syscall number for sys_write
    mov ebx, 1          ; File descriptor for stdout (1)
    int 0x80            ; Call kernel

    push eax

    ; Print a new line
    mov eax, 4                    ; sys_write
    mov ebx, 1                    ; stdout
    mov ecx, newline             ; Newline character
    mov edx, 1                    ; Length of newline
    int 0x80

    pop ecx
    push ecx 
    call output_written 
    pop ecx

    mov eax, ecx
    
    leave
    ret


; --------------------------------------------------
; Input: EBP+8 - Pointer to the memory location to store the number of bytes read
;        EBP+12 - Number of characters to read
; Output: Reads data from stdin and stores it in memory, returns number of bytes read in EAX
s_input:
    enter 0, 0

    ; Get parameters from the stack
    mov ecx, [ebp + 8]  ; Memory address where data will be stored (buffer)
    mov edx, [ebp + 12] ; Number of characters to read

    ; Syscall for reading data from stdin
    mov eax, 3          ; Syscall number for sys_read
    mov ebx, 0          ; File descriptor for stdin (0)
    int 0x80            ; Call kernel

    push eax
    pop ecx
    push ecx 
    call output_read 
    pop ecx

    mov eax, ecx

    leave
    ret