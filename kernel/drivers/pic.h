#pragma once

// Interrupt range handled by primary PIC
#define PRIMARY_PIC_INTERRUPT_RANGE_START 0x20
#define PRIMARY_PIC_INTERRUPT_RANGE_END PRIMARY_PIC_INTERRUPT_RANGE_START + 7

// Interrupt range handled by secondary PIC
#define SECONDARY_PIC_INTERRUPT_RANGE_START 0x28
#define SECONDARY_PIC_INTERRUPT_RANGE_END SECONDARY_PIC_INTERRUPT_RANGE_START + 7


void init_pics();

void acknowledge_interrupt(unsigned int interrupt_number);
