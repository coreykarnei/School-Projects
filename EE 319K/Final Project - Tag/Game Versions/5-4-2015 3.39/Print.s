; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

 IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
;ASCII numbers are +#0x30
Remainder       EQU -16
LinkReg			EQU -12
StackFrame		EQU -4
LCD_OutDec
	MOV R12, SP
	PUSH {R12, R0}			; Store stack frame, R0 is just filler
; Modulus: R1=R0%10, R2=R0/10
	MOV R1, #10
	UDIV R2, R0, R1			; R2 <- R0 / R1
	MLS R1, R1, R2, R0		; R1 <- R0 - (R1 * R2 )
; Put remainder & LR in stack (remember to pop twice later)
	PUSH {R1, LR}
; check for termination (R0/10=0)
	CMP R2, #0
	BEQ terminate
; recursion progression
	MOV R0, R2
	BL LCD_OutDec
; Terminating case
terminate
	LDR R12, [SP, #12]
	LDR R0, [R12, #Remainder]
	ADD R0, R0, #0x30		; Convert to ASCII
	BL ST7735_OutChar		; Print R0 ascii value
	LDR LR, [R12, #LinkReg]
	ADD SP, #16				; Clear local variables
      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix	
	PUSH {R4-R5, R11, LR}
; Check for special cases
	LDR R1, =9999			
	CMP R1, R0				; # > 9.999
	BMI Star
	CMP R0, #0				; edge case
	BMI Star
	
; Arrange decimal digits in order on stack	
	MOV R1, #10				; R0 = number to divide, R1 = 10, (start with 5279)
	MOV R4, #0				; Push-count
Push_loop
	UDIV R2, R0, R1			; R2 <- R2 / R1			# / by 10
	MLS R3, R2, R1, R0		; R3 <- R0 - (R1 * R2 )	Remainder
	PUSH {R3, R11}			; Push remainder
	MOV R0, R2
	ADD R4, #1
	CMP R2, #0				; Check terminating case
	BNE Push_loop

; Print/Pop sequence
	MOV R5, #4				; Chars-remaining
Print
	CMP R5, #0				; Check end case
	BEQ Exit
	CMP R5, #3				; Check '.' case
	BNE Next
	MOV R0, #46
	BL ST7735_OutChar
Next
	CMP R4, R5				; If Chars-remaining > Push-count, print a leading '0'
	BMI Leading_zero
Digit						; Print next digit
	POP {R0, R11}
	ADD R0, #0x30
	BL ST7735_OutChar
	SUB R5, #1
	B Print
Leading_zero				; Print leading '0'
	MOV R0, #0x30
	BL ST7735_OutChar
	SUB R5, #1
	B Print
	
; '*' Sequence
Star
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #46
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar

Exit
	POP {R4-R5, R11, LR}
	BX LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
