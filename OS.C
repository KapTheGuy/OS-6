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
	asm volatile("ldx #$0");
	// User or Kernel Mode?
	// 00 is Kernel
	// 01 is User
	#WRITE 00 0100
	
	Loop();
}

//
// Routine Description:
//		Loop that should initialize drivers & scheduler
//
void Loop()
{
	// Initialize User Mode
	#WRITE 01 0100

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
	asm volatile("lda $0100");
	asm volatile("cmp #$00");
	asm volatile("beq KeBugCheck");
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

void Right()
{
	asm volatile("iny");
	asm volatile("lda #$00");
	asm volatile("sta $ff");
	return;
}

void Left()
{
	asm volatile("dey");
	asm volatile("lda #$00");
	asm volatile("sta $ff");
	return;
}

void Down()
{
	asm volatile("txa");
	asm volatile("ldx #32");
	asm volatile("IncLoop:");
	asm volatile("	iny");
	asm volatile("	dex");
	asm volatile("	cpx #00");
	asm volatile("	bne IncLoop");
	asm volatile("tax");
	asm volatile("lda #$00");
	asm volatile("sta $ff");
	return;
}

void Up()
{
	asm volatile("txa");
	asm volatile("ldx #32");
	asm volatile("IncLoop2:");
	asm volatile("	dey");
	asm volatile("	dex");
	asm volatile("	cpx #00");
	asm volatile("	bne IncLoop2");
	asm volatile("tax");
	asm volatile("lda #$00");
	asm volatile("sta $ff");
	return;
}

void Task1()
{
	#WRITE 05 0200
	asm volatile("lda $ff");
	asm volatile("cmp #$64");
	asm volatile("beq Right");
	asm volatile("cmp #$61");
	asm volatile("beq Left");
	asm volatile("cmp #$73");
	asm volatile("beq Down");
	asm volatile("cmp #$77");
	asm volatile("beq Up");
	asm volatile("inx");
	return;
}

void Task2()
{
	asm volatile("lda #$06");
	asm volatile("sta $0201, Y");
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
