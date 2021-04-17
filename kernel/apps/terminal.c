#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../util/util.h"
#include "terminal.h"



void startTerminal()
{
	clear_screen();
	print("******* TERMINAL *******");

	move_cursor(2, 0);

	const uchar result = read_scan_code();

	char numeric_string[32] = { 0 };
	const char* ptr = itoa((int)result, 10, numeric_string);
	print(ptr);
	
	//const uchar result2 = read_scan_code();
	//print_char(result2, 2, 3, 0);
}