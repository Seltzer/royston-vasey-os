#include "interrupts.h"
#include "../libc/typedefs.h"
#include "ports.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../drivers/pic.h"


#define TIMER_INTERRUPT PRIMARY_PIC_INTERRUPT_RANGE_START
#define KEYBOARD_INTERRUPT PRIMARY_PIC_INTERRUPT_RANGE_START + 1


// Defined in interrupts.asm
extern void load_idt(struct InterruptDescriptorTable* idt_ptr);
extern void noop_handler();
extern void interrupt_handler_0x20();
extern void interrupt_handler_0x21();


// Interrupt vector from 0-0x100. Reference:
//		0x00-0x1F = reserved for CPU
//		0x20-0x27 = handled by primary PIC
//		0x28-0x2F = handled by secondary PIC
struct IdtEntry interrupt_vector[0x100];

// Interrupt descriptor table
struct InterruptDescriptorTable interrupt_descriptor_table;


// Forward decls
static void set_handler(struct IdtEntry* idt_entry, void* handler);
static void populate_interrupt_vector();



void initialise_interrupts_system()
{
	// Populate interrupt vector
	populate_interrupt_vector();

	// Initialise interrupt descriptor table
	interrupt_descriptor_table.size = sizeof(interrupt_vector) - 1;
	interrupt_descriptor_table.idt_head = interrupt_vector;

	// Load it
	load_idt(&interrupt_descriptor_table);

	// Configure master + secondary PICs
	init_pics();
}


static void populate_interrupt_vector()
{
	// Start off by wiring up all interrupts to a NOOP handler
	for (size_t i = 0; i < sizeof(interrupt_vector); i++) {
		set_handler(&interrupt_vector[0], noop_handler);
	}

	// This needs to be kept consistent with the IMR in pic.c
	set_handler(&interrupt_vector[TIMER_INTERRUPT], interrupt_handler_0x20);
	set_handler(&interrupt_vector[KEYBOARD_INTERRUPT], interrupt_handler_0x21);
}


void handle_interrupt(struct InterruptCpuState cpu_state, uint interrupt_number, struct InterruptStackState stack)
{
	switch (interrupt_number) {
		case KEYBOARD_INTERRUPT:
			handle_kb_interrupt();
			break;
		default:
			break;
	}

	// FIXME: Double checking
	if (interrupt_number >= PRIMARY_PIC_INTERRUPT_RANGE_START && interrupt_number <= SECONDARY_PIC_INTERRUPT_RANGE_END)
		acknowledge_interrupt(interrupt_number);
}


static void set_handler(struct IdtEntry* idt_entry, void* handler)
{
	idt_entry->offset_high_word = (((uint)handler) & 0xffff0000) >> 16;
	idt_entry->offset_low_word = ((uint)handler) & 0xffff;

	// 1 Segment present | 00 Ring 0 | 01110 32 bit interrupt gate
	idt_entry->type_attr = 0b10001110;

	// Segment selector
	idt_entry->selector = 0x0008;

	idt_entry->zero = 0;
}

