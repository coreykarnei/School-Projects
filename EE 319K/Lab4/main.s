;****************** main.s ***************
; Program written by: Brent Atchison & Michael Darden
; Date Created: 8/25/2013 
; Last Modified: 3/4/2015 
; Section 1-2pm     TA: Jenny Chen
; Lab number: 4
; Brief description of the program
;   If the switch is presses, the LED toggles at 8 Hz
; Hardware connections
;  PE0 is switch input  (1 means pressed, 0 means not pressed)
;  PE1 is LED output (1 activates external LED on protoboard) 
;Overall functionality of this system is the similar to Lab 3, with four changes:
;1-  activate the PLL to run at 80 MHz (12.5ns bus cycle time) 
;2-  initialize SysTick with RELOAD 0x00FFFFFF 
;3-  add a heartbeat to PF2 that toggles every time through loop 
;4-  add debugging dump of input, output, and time
; Operation
;	1) Make PE1 an output and make PE0 an input. 
;	2) The system starts with the LED on (make PE1 =1). 
;   3) Wait about 62 ms
;   4) If the switch is pressed (PE0 is 1), then toggle the LED once, else turn the LED on. 
;   5) Steps 3 and 4 are repeated over and over

SWITCH                  EQU 0x40024004  ;PE0
LED                     EQU 0x40024008  ;PE1
SYSCTL_RCGCGPIO_R       EQU 0x400FE608
SYSCTL_RCGC2_GPIOE      EQU 0x00000010   ; port E Clock Gating Control
SYSCTL_RCGC2_GPIOF      EQU 0x00000020   ; port F Clock Gating Control
GPIO_PORTE_DATA_R       EQU 0x400243FC
GPIO_PORTE_DIR_R        EQU 0x40024400
GPIO_PORTE_AFSEL_R      EQU 0x40024420
GPIO_PORTE_PUR_R        EQU 0x40024510
GPIO_PORTE_DEN_R        EQU 0x4002451C
NVIC_ST_CURRENT_R       EQU 0xE000E018
GPIO_PORTF_DATA_R       EQU 0x400253FC
GPIO_PORTF_DIR_R        EQU 0x40025400
GPIO_PORTF_AFSEL_R      EQU 0x40025420
GPIO_PORTF_DEN_R        EQU 0x4002551C


           AREA    DATA, ALIGN=2
SIZE       EQU    50
;You MUST use these two buffers and two variables
;You MUST not change their names
DataBuffer SPACE  SIZE*4
TimeBuffer SPACE  SIZE*4
DataPt     SPACE  4
TimePt     SPACE  4
;These names MUST be exported
           EXPORT DataBuffer
           EXPORT TimeBuffer
           EXPORT DataPt
           EXPORT TimePt
		   IMPORT SysTick_Init

      ALIGN          
      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      EXPORT  Start
      IMPORT  TExaS_Init


Start BL   TExaS_Init  ; running at 80 MHz, scope voltmeter on PD3
; initialize Port E
	LDR R1, =SYSCTL_RCGCGPIO_R
	LDR R0, [R1]
	ORR R0, R0, #0x00000030	;port E and F
	STR R0, [R1]
	NOP
	NOP
	LDR R1, =GPIO_PORTE_DIR_R
	LDR R0, [R1]
	ORR R0, R0, #0x00000002
	AND R0, R0, #0xFFFFFFFE
	STR R0, [R1]
	LDR R1, =GPIO_PORTE_AFSEL_R
	LDR R0, [R1]
	AND R0, R0, #0xFFFFFFFC
	STR R0, [R1]
	LDR R1, =GPIO_PORTE_DEN_R
	LDR R0, [R1]
	ORR R0, R0, #0x00000003
	STR R0, [R1]
	
	LDR R1, =GPIO_PORTE_DATA_R	;port E1 = output
	LDR R0, [R1]
	ORR R0, R0, #0x00000002
	STR R0, [R1]
; initialize Port F
	LDR R1, =GPIO_PORTF_DIR_R
	LDR R0, [R1]
	ORR R0, R0, #0x00000004
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_AFSEL_R
	LDR R0, [R1]
	AND R0, R0, #0xFFFFFFFB
	STR R0, [R1]
	LDR R1, =GPIO_PORTF_DEN_R
	LDR R0, [R1]
	ORR R0, R0, #0x00000004
	STR R0, [R1]
	
; initialize debugging dump
	BL Debug_Init

      CPSIE  I    ; TExaS voltmeter, scope runs on interrupts
loop  BL   Debug_Capture
;heartbeat
	LDR R1, =GPIO_PORTF_DATA_R;check PF2
	LDR R0, [R1]
	AND R0, R0, #0x00000004	;mask for bit 2
    CMP R0, #0
		BEQ ToggleOnF	;If off, toggle on
	LDR R0, [R1]
	AND R0, R0, #0xFFFFFFFB
	STR R0, [R1]		;toggle off
		B	next
ToggleOnF
	LDR R0, [R1]
	ORR R0, R0, #0x00000004
	STR R0, [R1]		;Toggle on
next
; Delay
;input PE0 test output PE1
	BL delay	;Jump to delay function
	
	LDR R1, =GPIO_PORTE_DATA_R;check PE0
	LDR R0, [R1]
	AND R0, R0, #0x00000001	;mask for bit 0
    CMP R0, #0 
		BNE pressed
	LDR R0, [R1]		;Turn on if not pressed
	ORR R0, R0, #0x00000002
	STR R0, [R1]
		B    loop
pressed
	LDR R1, =GPIO_PORTE_DATA_R;check PE1
	LDR R0, [R1]
	AND R0, R0, #0x00000002	;mask for bit 1
    CMP R0, #0
		BEQ ToggleOnE	;If off, toggle on
	LDR R0, [R1]
	AND R0, R0, #0xFFFFFFFD
	STR R0, [R1]		;toggle off
		B	loop
ToggleOnE
	LDR R0, [R1]
	ORR R0, R0, #0x00000002
	STR R0, [R1]		;Toggle on
		B	loop
;---------subroutines---------	
delay
	LDR R0, =1240000; your input output delay 
wait
	SUBS R0, R0, #0X01
	BNE wait
	BX LR


;------------Debug_Init------------
; Initializes the debugging instrument
; Input: none
; Output: none
; Modifies: none
; Note: push/pop an even number of registers so C compiler is happy
Debug_Init
	  PUSH {R0-R4,LR}
	  LDR R0, =DataBuffer
	  LDR R1, =DataPt	;Initialize data pointer
	  STR R0, [R1]
	  LDR R0, =TimeBuffer
	  LDR R2, =TimePt	;Initialize time pointer
	  STR R0, [R2]
	  
	  LDR R0, =0xFFFFFFFF
	  LDR R3, =50	;Fill Array
	  LDR R1, [R1]	;take pointer values
	  LDR R2, [R2]
FillBuffers
	  STR R0, [R1]	
	  ADD R1, R1, #4;step forward (doesn't change pointer)
	  STR R0, [R2]
	  ADD R2, R2, #4
	  SUBS R3, #1
	  BNE FillBuffers 
; init SysTick
	  BL SysTick_Init
	  POP {R0-R4,LR}
      BX LR

;------------Debug_Capture------------
; Dump Port E and time into buffers
; Input: none
; Output: none
; Modifies: none
; Note: push/pop an even number of registers so C compiler is happy
Debug_Capture
	  ;Check if buffers are full
	  PUSH {R0-R3}
	  LDR R0, =DataBuffer
	  ADD R0, #200
	  LDR R1, =DataPt
	  LDR R1, [R1]
	  CMP R0, R1
	  BEQ	SkipCapture	;If DataPt is at end of array, return
	  ;Sub
	  ;Combine PE0 and PE1 into R0
	  LDR R1, =GPIO_PORTE_DATA_R;
	  LDR R2, [R1]	;Deal with PE0
	  AND R2, #0x00000001
	  LSL R0, R2, #4;Store PE0 in bit 4 of R0
	  LDR R2, [R1]	;Deal with PE1
	  AND R2, #0x00000002
	  LSR R2, #1
	  ORR R0, R2	;Store PE1 in bit 0 of R0
	  
	  LDR R2, =DataPt
	  LDR R1, [R2]	;Find Array element
	  STR R0, [R1]	;Store data in array
	  ADD R1, #4	
	  STR R1, [R2]	;Increment pointer
	  
	  LDR R0, =NVIC_ST_CURRENT_R
	  LDR R0, [R0]	;Get counter value
	  
	  LDR R2, =TimePt
	  LDR R1, [R2]	;Find Array element
	  STR R0, [R1]	;Store time in array
	  ADD R1, #4	
	  STR R1, [R2]	;Increment pointer
SkipCapture	  
	  POP {R0-R3}
      BX LR


    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
        
		
		;Overhead Calculations:
		;Execution time:	29 instructions		* 2 bus cycles * 12.5 ns	=	7.25e-7		seconds
		;Between time:		1240012 instructions* 2 bus cycles * 12.5 ns	=	0.0310003	seconds
		;((29*2*12.5n)/(1240012*2*12.5n))*100	=	0.00935474817%