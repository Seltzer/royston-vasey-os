#include "screen.h"
#include "../core/ports.h"
#include "../libc/libc.h"


// I/O ports
# define REG_SCREEN_CTRL 0x3D4
# define REG_SCREEN_DATA 0x3D5


// Forward decls
static uint compute_position(uint row, uint col);
static uint handle_scrolling(uint cursor_pos);
static uint get_cursor_position();
static void set_cursor_position(uint cursor_pos);


void clear_screen()
{
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLS; col++) {
			print_char(' ', row, col, WHITE_ON_BLACK);
		}
	}

	// Reset cursor position to top left
	set_cursor_position(0);
}


void print(const char* message)
{
	print_string_at(message, -1, -1);
}


/*
 * Terribly inefficient 
 */
void print_string_at(const char* message, int row, int col)
{
	// Move cursor if user has specified a position
	if (row >= 0 && col >= 0) 
		set_cursor_position(compute_position(row, col));
			
	// Print characters one by one and very inefficiently
	uint i = -1;
	while (message[++i] != 0) {
		print_char(message[i], i == 0 ? row : -1, i == 0 ? col : -1, WHITE_ON_BLACK);
	}
}


void print_char(char character, int row, int col, char attr_byte)
{
	if (!attr_byte)
		attr_byte = WHITE_ON_BLACK;	

	// Compute position either as requested row/col or current cursor position
	uint pos = row >= 0 && col >= 0
		? compute_position(row, col)
		: get_cursor_position();

	if (character == '\n') {
		// Move cursor to end of current row, so that the next char will be printed on a new line
		uint current_row = pos / MAX_COLS;
		pos = compute_position(current_row, 79);
	}
	else {
		// Write character
		uchar* mem_ptr = (uchar*)VIDEO_ADDRESS;
		const int mem_offset = pos * 2;
		mem_ptr[mem_offset] = character;
		mem_ptr[mem_offset + 1] = attr_byte;
	}

	// Increment cursor position
	pos += 1;
	
	// Scroll if needed and move cursor
	pos = handle_scrolling(pos);
	set_cursor_position(pos);
}


void move_cursor(uint row, uint col) {
	const uint pos = compute_position(row, col);
	set_cursor_position(pos);
}


/*
 * Computes a position, not a memory offset
 */
static uint compute_position(uint row, uint col)
{
	return (row * MAX_COLS) + col;
}


/*
 * Returns cursor position
 */
static uint get_cursor_position()
{
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;

	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);

	return offset;
}


/**
 * Set cursor to position
 */
static void set_cursor_position(uint cursor_pos)
{
	// Write high byte
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (uchar)((cursor_pos & 0xff00) >> 8));
		
	// Write low byte
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (uchar)(cursor_pos & 0xff));
	
	int new_cursor_position = get_cursor_position();
}


/**
 * Scrolls screen if necessary. Returns potentially updated cursor position.
 */
static uint handle_scrolling(uint cursor_pos)
{	
	if (cursor_pos < (MAX_ROWS * MAX_COLS)) 
		return cursor_pos;

	// Move everything one row up
	const char* src = (const char*)(VIDEO_ADDRESS + compute_position(1, 0) * 2);
	memcpy((char*)VIDEO_ADDRESS, src, (MAX_ROWS - 1) * MAX_COLS * 2);
	
	// Zero out bottom row
	char* bottom_row = (char*)(VIDEO_ADDRESS + compute_position(MAX_ROWS - 1, 0) * 2);
	for (int i = 0; i < MAX_COLS * 2; i++) 
		bottom_row[i] = 0;	
	
	// Return updated cursor position (one row backwards)
	return cursor_pos - MAX_COLS;
}

