#include "pic.h"
#include "../core/ports.h"
#include "../libc/typedefs.h"


#define PRIMARY_PIC_COMMAND_PORT 0x20	    
#define PRIMARY_PIC_DATA_PORT 0x21
#define SECONDARY_PIC_COMMAND_PORT 0xA0
#define SECONDARY_PIC_DATA_PORT 0xA1

// End of interrupt command
#define PIC_EOI 0x20


// Forward decls
static void reconfigure_pics();


void init_pics()
{
	reconfigure_pics();

	// Set IMR to disable all interrupts on the primary pic except for timer and keyboard
	port_byte_out(PRIMARY_PIC_DATA_PORT, 0b11111100);
	
	// Similarly, disable all interrupts on secondary PIC
	port_byte_out(SECONDARY_PIC_DATA_PORT, 0xff);
}


static void reconfigure_pics()
{
	// Save existing data (interrupt mask from IMR)
	uchar primary_pic_data = port_byte_in(PRIMARY_PIC_DATA_PORT);
	uchar secondary_pic_data = port_byte_in(SECONDARY_PIC_DATA_PORT);

	// Commence initialisation sequence for both PICs 
	// // ICW1 = 0x11 = 00010001 => Initialisation in cascaded (primary/secondary mode) with ICW4
	port_byte_out(PRIMARY_PIC_COMMAND_PORT, (uchar)0x11);
	io_wait();
	port_byte_out(SECONDARY_PIC_COMMAND_PORT, (uchar)0x11);
	io_wait();

	// ICW2 => The CPU reserves interrupts 0-0x1F for exceptions so we'll configure the primary PIC to use 0x20-0x27
	port_byte_out(PRIMARY_PIC_DATA_PORT, PRIMARY_PIC_INTERRUPT_RANGE_START);
	io_wait();

	// ... and the secondary one to use 0x28-0x2F
	port_byte_out(SECONDARY_PIC_DATA_PORT, SECONDARY_PIC_INTERRUPT_RANGE_START);
	io_wait();

	// ICW3 => Daisy chain primary to secondary via IRQ2
	port_byte_out(PRIMARY_PIC_DATA_PORT, 0x04);
	io_wait();
	port_byte_out(SECONDARY_PIC_DATA_PORT, 0x02);
	io_wait();

	// Set both PICs to x86 mode 
	port_byte_out(PRIMARY_PIC_DATA_PORT, 0x01);
	io_wait();
	port_byte_out(SECONDARY_PIC_DATA_PORT, 0x01);
	io_wait();

	// Restore data
	port_byte_out(PRIMARY_PIC_DATA_PORT, primary_pic_data);
	port_byte_out(SECONDARY_PIC_DATA_PORT, secondary_pic_data);
}


void acknowledge_interrupt(unsigned int interrupt_number)
{
	// Ignore interrupts outside our defined range
	if (interrupt_number < PRIMARY_PIC_INTERRUPT_RANGE_START || interrupt_number > SECONDARY_PIC_INTERRUPT_RANGE_END) 
		return;
	
	// Send ACK to primary PIC
	port_byte_out(PRIMARY_PIC_COMMAND_PORT, PIC_EOI);

	// If the interrupt was serviced by the secondary PIC, send an ACK to it as well
	if (interrupt_number >= SECONDARY_PIC_INTERRUPT_RANGE_START) 
		port_byte_out(SECONDARY_PIC_COMMAND_PORT, PIC_EOI);	
}
