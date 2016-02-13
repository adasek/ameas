/*
* Copyright (C) 2015 Adam Benda
*
* measure_bh1750fvi.c
*
* Sensor program for digital light sensor BH1750FVI
* Usage:
*   
* 
*/

  
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <math.h>   
#include <linux/i2c.h>   
#include <linux/i2c-dev.h> 


static int i2c_wr1B(int file,unsigned char addr, unsigned char data) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    outbuf = data;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 1;
    }

    return 0;
}

static int i2c_rd2B(int file,unsigned char addr, unsigned char * data) {
    unsigned char buf[4];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
	

    messages[0].addr  = addr;
    messages[0].flags = I2C_M_RD;
    messages[0].len   = 4;
    messages[0].buf   = buf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        return 1;
    }
    data[0] = buf[0];
    data[1] = buf[1];
  
    return 0;
}


int main(int argc, char * * argv)
{       
  int file;
  int bus_id = 0; 
  char filename[20]; 
  int i;  


  snprintf(filename, 19, "/dev/i2c-%d", bus_id);
  file = open(filename, O_RDWR);
  if (file < 0) {                 
    exit(1);
  }
   
  int addr = 0x23; /* The I2C address */

if (ioctl(file, I2C_SLAVE, addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    exit(1);
}      
   
  int res;
  char buf[10];
	

		
     if(0 != i2c_wr1B (file,addr,0X00)){exit(5);} //power off   
     usleep(2000);   
     if(0 != i2c_wr1B  (file,addr, 0X01)){exit(6);} //power on  
     usleep(2000);   
     if(0 != i2c_wr1B  (file,addr, 0X07)){exit(7);} //reset on  
     usleep(2000);   
     if(0 !=  i2c_wr1B (file,addr, 0X10)){exit(8);} //set One Time  H-Resolution Mode  0x10
     usleep(200000); //200ms     

	         
    
   unsigned char resWord[2];  
	res=i2c_rd2B(file,addr, resWord);
   
    
   //res has different ENDIANESS
   //int firstByte=res/256;
   //int secondByte=res%256;
                                                     
     if(0 != i2c_wr1B (file,addr,0X00) ){exit(12);} //power off 
    
   
   //swap bytes
   
   unsigned int firstByte=(unsigned int)(resWord[0] & 0xFF);
   unsigned int secondByte=(unsigned int)(resWord[1] & 0xFF);
   
       
    
   float flRes=(float)((firstByte<<8)+secondByte);  
   flRes=flRes/1.2;
   printf("%4.1f\n",flRes);  
  
    
    return 0;
}
