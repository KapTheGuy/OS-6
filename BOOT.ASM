;++
; Module name: BOOT.ASM
;
; Author: Kap Petrov (kappa_)
;
; Description:
;	OS/6 Operating System
;
; Revision History:
; 
;--

INIT:
    LDX #$0
    JMP LOOP

LOOP:
    JSR KE_SCHEDULE
    JMP LOOP

KE_SCHEDULE:
    CPX #$2
    BEQ RESET
    CPX #$0
    BEQ TASK1
    CPX #$1
    BEQ TASK2
    RTS

TASK1:
    LDA #$05
    STA $0200
    INX
    RTS

TASK2:
    LDA #$01
    STA $0201
    INX
    RTS

RESET:
    LDX #$0
    RTS
