//++
// Module name: OS.C
//
// Author: Kap Petrov (kappa_)
//
// Description:
//	OS/6 Operating System
//
// Revision History:
// 
//--

//
// Routine Description:
// 		Initialize OS/6
//
void Init()
{
	asm volatile("ldx #$0");
	Loop();
}

//
// Routine Description:
//		Loop that should initialize drivers & scheduler
//
void Loop()
{
	while(1)
	{
		KeSchedule();
	}
}

//
// Routine Description:
//		Round-Robin Scheduler
//
void KeSchedule()
{
	asm volatile("cpx #$2");
	asm volatile("beq Reset");	
	asm volatile("cpx #$0");
	asm volatile("beq Task1");
	asm volatile("cpx #$1");
	asm volatile("beq Task2");
	return;
}

//
// Routine Description:
//		Put a pixel on screen, expects Y to hold position & Accumulator to hold Color.
//
void KePutPix()
{
	asm volatile("sta $0200, Y");
	return;
}

//
// Routine Description:
//		Brings the system down in a controlled manner
//
void KeBugCheck()
{
	asm volatile("lda #$02");
	KePutPix();
	asm volatile("iny");
	asm volatile("jmp KeBugCheck");
}

// Tasks
void Task1()
{
	asm volatile("lda #$05");
	asm volatile("sta $0200");
	asm volatile("inx");
	return;
}

void Task2()
{
	asm volatile("lda #$01");
	asm volatile("sta $0201");
	asm volatile("inx");
	return;
}

//
// Routine Description:
//		Reset Task PID to run
//
void Reset()
{
	asm volatile("ldx #$0");
	return;
}
