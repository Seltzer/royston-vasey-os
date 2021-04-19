#include "keyboard.h"
#include "../core/ports.h"
#include "screen.h"

#define KBD_DATA_PORT 0x60


static uchar read_scan_code();


void handle_kb_interrupt()
{
	print("scan code: ");

	const uchar character = read_scan_code();

	//const char* ptr2 = itoa((int)character, 10, numeric_string);
	//print(ptr2)	
}


static uchar read_scan_code()
{
	return port_byte_in(KBD_DATA_PORT);
}