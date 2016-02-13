/*
* Copyright (C) 2015 Adam Benda
*
* measure_pcf8591.c
*
* Sensor program for Analog/Digital I2C GPIO Analog expander chip PCF8591
*   
* 
*/ 
#define _BSD_SOURCE //for usleep

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <linux/i2c.h>   
#include <linux/i2c-dev.h>   
#include <sys/ioctl.h>
#include "i2c-func.h"    
#include <inttypes.h> //uint8_t  
#include <unistd.h>  //usleep
     
 uint8_t readI2C(int I2Cfile,uint8_t pin){
    uint8_t command[2];  
    uint8_t value[4];
    int ret;
    uint8_t val[100];
    
    //Command to read value of pin i+1
    command[0]=(0x40 | (pin & 0x03));   //or with 0x40 | for analog output enable
    command[1]=(0x40 | (pin & 0x03)); //analog out ... but why not repeat ourselves? .)
    ret = write(I2Cfile, command, 2);
    
    usleep(3*1000); 
                   
    //read value of pin
    read(I2Cfile, val,1); //first byte is dummy  
    ret = read(I2Cfile, val,1);    
    usleep(20*1000); //20ms   
     if(ret != 1)
     {
        perror("reading i2c device\n");
        return 0x00;
     }         
    return val[0];
 }       
    
int main(int argc, char * * argv)
{

if(argc<2){
 printf("USAGE: %s [pin]\nPins are numbered A0=0, A1=1, A2=2, A3=3",argv[0]);
}

int I2Cfile;
char *filename = "/dev/i2c-0";
if ((I2Cfile = open(filename, O_RDWR)) < 0) {
    perror("Failed to open the i2c bus");
    exit(1);
}   

unsigned char _i2cAddr = 0x48;          // The I2C address of device
if (ioctl(I2Cfile, I2C_SLAVE, _i2cAddr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    exit(1);
}

struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

           
uint8_t val=0x00; 
int pin=atoi(argv[1]);
 for(int i=0;i<5;i++){
  readI2C(I2Cfile,pin ); //first byte is "dummy"    
  usleep(200*1000); //200ms   
 }
 
val=readI2C(I2Cfile,pin ); //second byte is our information
printf("%d\n",val);
         
          
  return 0;
}
