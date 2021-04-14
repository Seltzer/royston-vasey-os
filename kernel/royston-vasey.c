int _start() {
    char* video_memory = (char*) 0xb8000;   
    *video_memory = 'N';
}


/**
 * To placate the IDE compiler
 */
int main() { 
    return 0;
}


