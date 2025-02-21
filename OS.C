//++
// Module name: OS.C
//
// Author: Kap Petrov (kappa_)
//
// Description:
//	OS/6 Operating System for SC6
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
	// This is kind of pointless,
	// but i'll keep it for the funni.
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
	// If task counter is 2, then reset the task counter.
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
//		Brings the system down in a controlled manner
//
void KeBugCheck()
{
	// Warning: Hacky bullshit ahead!
	asm volatile("cpy #255");
	asm volatile("beq InitP2");
	asm volatile("lda #$02");
	asm volatile("sta $0200, Y");
	asm volatile("iny");
	asm volatile("jmp KeBugCheck");
	asm volatile("InitP2:");
	asm volatile("	ldy #$00");
	asm volatile("	jmp Page2");
	asm volatile("Page2:");
	asm volatile("	sta $0300, Y");
	asm volatile("	iny");
	asm volatile("	jmp Page2");
}

void Task1()
{
	// Load in last key press, if it's Q, render a white pixel at 0 0.
	asm volatile("lda $ff");
	asm volatile("cmp #$71");
	asm volatile("bne Task1");
	#WRITE 01 0200
	asm volatile("inx");
	return;
}

void Task2()
{
	// Address 0x0101 is where the text buffer is stored.
	asm volatile("lda $ff");
	asm volatile("lda $0101");
	asm volatile("sta $0100");
	asm volatile("lda $ff");
	asm volatile("sta $0101");
	asm volatile("lda $0100");
	asm volatile("cmp #$71");
	asm volatile("bne Task2");
	asm volatile("lda $0101");
	asm volatile("cmp #$0d");
	asm volatile("bne Task2");
	asm volatile("jmp KeBugCheck");

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
