#include "ports.h"

#ifndef _MSC_VER 



unsigned char port_byte_in(unsigned short port) {
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

	return result;
}


void port_byte_out(unsigned short port, unsigned char data) {
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));	
}


unsigned short port_word_in(unsigned short port) {
	unsigned short result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));

	return result;
}


void port_word_out(unsigned short port, unsigned short data) {
	__asm__("out %%ax , %%dx " : : "a" (data), "d" (port));
}


void io_wait()
{
	// Linux does this so it's probably okay
	asm volatile ("outb %%al, $0x80" : : "a"(0));
}


#else



unsigned char port_byte_in(unsigned short port) {
	return 42;
}


void port_byte_out(unsigned short port, unsigned char data) {
}


unsigned short port_word_in(unsigned short port) { 
	return 42;
}


void port_word_out(unsigned short port, unsigned short data) { 
}



#endif
