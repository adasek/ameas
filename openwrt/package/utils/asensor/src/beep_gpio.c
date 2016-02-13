/**
 * Beep_gpio.c: piezo bieper over GPIO control
 * Arguments:
 *  1) number of GPIO pin 
 *  2) how long to stay at 1 value (in microseconds)
 *  3) how long to stay at 0 value (in microseconds)     
 *            
 * Author: Adam Benda 
 */   


#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>


int main(int argc, char * * argv)
{
if(argc<3){
printf("Not enough arguments... first argument should be no. of pin, second time of 1 in useconds, third time of 0 in useconds");
return 1;
}

   printf("Let's beep!");
            
       int fd;
        char buf[500]; 
        int gpio = atoi(argv[1]);
        int waitTime1 = atoi(argv[2]);  
        int waitTime0 = atoi(argv[3]);
        if(gpio<0 || gpio>30){
           printf("I don't know about this pin %d. Exitting.",gpio);
           return 3;
           }
        
        fd = open("/sys/class/gpio/export", O_WRONLY);
        sprintf(buf, "%d", gpio); 
        write(fd, buf, strlen(buf));
        close(fd);
        
        
        sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
        fd = open(buf, O_WRONLY);
        
        // Set out direction
        write(fd, "out", 3); 
        close(fd);
        
        sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
        
        fd = open(buf, O_WRONLY);
                while(1){
                      //high
                      write(fd, "1", 1); 
                      usleep(waitTime1);  
                      //low
                      write(fd, "0", 1);     
                      usleep(waitTime0);
                }
        
        close(fd);
    
    return 0;
}
