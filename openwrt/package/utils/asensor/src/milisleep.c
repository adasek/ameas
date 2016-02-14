/*
 * Copyright (C) 2015 Adam Benda
 *
 * Milisleep
 *
 * Sleep for a given number of miliseconds
 *   
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

int main(int argc, char * * argv) {
    int ms = 0;
    if (argc < 1) {
        printf("Usage: %s [number] - sleep for a given number of miliseconds\n", argv[0]);
        return 1;
        ms = atoi(argv[1]);
    }
    if (ms <= 0) {
        return 2;
    }

    usleep(ms * 1000);
    return 0;
}
