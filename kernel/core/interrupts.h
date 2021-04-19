#pragma once


#include "../libc/typedefs.h"


struct IdtEntry {
	ushort offset_low_word;
	ushort selector;
	uchar zero;
	uchar type_attr;
	unsigned short int offset_high_word;
} __attribute__((__packed__));


struct InterruptDescriptorTable {
	ushort size;
	struct IdtEntry* idt_head;
} __attribute__((packed));


struct InterruptCpuState {
	uint eax;
	uint ebx;
	uint ecx;
	uint edx;
	uint edi;
	uint esi;
	uint ebp;
	uint esp;
} __attribute__((packed));


struct InterruptStackState {
	// 31-16 = reserved | 15-3 = segment selector index 
	// | 2 = GDT (0) or LDT (1) | 1 = description location (0 = GDT/LDT, 1 = IDT) | 0 = External event (0 = internal, 1 = external)
	uint error_code;
	
	uint return_eip;
	uint return_cs;
	uint eflags;
} __attribute__((packed));


void initialise_interrupts_system();

void handle_interrupt(struct InterruptCpuState cpu_state, uint interrupt_number, struct InterruptStackState stack);