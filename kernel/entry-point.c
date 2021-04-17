#include "drivers/screen.h"
#include "init/init.h"
#include "util/util.h"

int _start() {
    init();
}


/**
 * To placate the IDE compiler which still thinks we're in hosted mode
 */
int main() { 
    return 0;
}


