/* 
 * Low level I/O routines
 */

#pragma once


unsigned char port_byte_in(unsigned short port);

void port_byte_out(unsigned short port, unsigned char data);

unsigned short port_word_in(unsigned short port);

void port_word_out(unsigned short port, unsigned short data);

/*
 * Wait for I/O op to complete in cases where there's no interrupt / status register to act as an ACK
 */
void io_wait();