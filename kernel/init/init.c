#include "../drivers/screen.h"
#include "../util/util.h"
#include "../apps/terminal.h"
#include "../core/interrupts.h"
#include "init.h"


// Forward decls
static void print_stuff();
static void print_more_stuff(); 
static void print_even_more_stuff();


void init() {
    initialise_interrupts_system();

    clear_screen();

    startTerminal();

    //print_stuff();
    //print_more_stuff();
    //print_even_more_stuff();


    //const int result = 1 / 0;
}


static void print_stuff()
{
    char* msg = "This is a local town for local people.";
    print(msg);
}


static void print_more_stuff()
{
    for (int i = 0; i < 300; i++) {
        print_char('X', -1, -1, (char)0);
        print_char('O', -1, -1, (char)0);
        print_char('R', -1, -1, (char)0);

        delay();
    }
}


static void print_even_more_stuff()
{
    for (int i = 0; i < 700; i++) {
        print_char('X', -1, -1, (char)0);
        print_char('O', -1, -1, (char)0);
        print_char('R', -1, -1, (char)0);

        delay();
    }
    
    for (int i = 0; i < 700; i++) {
        print_char('A', -1, -1, (char)0);
        print_char('B', -1, -1, (char)0);
        print_char('C', -1, -1, (char)0);

        delay();
    }
}