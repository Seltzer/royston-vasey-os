/****
 * The beginnings of a VGA screen driver
 ****/

#pragma once


#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f


/*
 * Convenience function
 */
void print(char* message);


/*
 * Print string at specified location
 */
void print_string_at(char* message, int row, int col);


/*
 * Print char at specified location
 * 
 * Params: 
 *		attr_byte - Foreground (4 bits) + background color (4 bits)
 */
void print_char(char character, int row, int col, char attr_byte);


void clear_screen();