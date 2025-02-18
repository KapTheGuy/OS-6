/*++
; Module name: OS.C
;
; Author: Kap Petrov (kappa_)
;
; Description:
;    OS/6 Operating System Demonstration in The C Programming Language
;
; Revision History:
; 
--*/

int x;

void Loop();
void KeSchedule();
void Reset();
void Task1();
void Task2();

void Init()
{
    x = 0;
    Loop();
}

void Loop()
{
    while (1)
    {
		KeSchedule();
    }
}

void KeSchedule()
{
    if (x == 2)
		Reset();

    if (x == 0)
		Task1();

    if (x == 1)
		Task2();
}

void Task1()
{
	asm volatile("LDA #$05\nSTA $0200");
	x++;
}

void Task2()
{
	asm volatile("LDA #$01\nSTA $0201");
	x++;
}

void Reset()
{
    x = 0;
}
