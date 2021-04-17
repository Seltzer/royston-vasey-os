#include "keyboard.h"
#include "ports.h"


#define KBD_DATA_PORT 0x60

uchar read_scan_code() 
{
	return port_byte_in(KBD_DATA_PORT);
}