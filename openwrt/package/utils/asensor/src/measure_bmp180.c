/*
* Copyright (C) 2015 Adam Benda
*
* measure_bmp180.c
*
* Sensor program for digital pressure sensor BMP180
* Usage:
*   
* 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <linux/i2c.h>   
#include <linux/i2c-dev.h>   
#include <sys/ioctl.h>
#include "i2c-func.h"
     
#define OSS 3 //high resolution   
#define WAIT_TIME 26000 //high resolution wait


int main(int argc, char * * argv)
{

int amsl=0; //height above sea level
if(argc>1){amsl=atoi(argv[1]);}      


int file;
char *filename = "/dev/i2c-0";
if ((file = open(filename, O_RDWR)) < 0) {
    perror("Failed to open the i2c bus");
    exit(1);
}   

unsigned char addr = 0x77;          // The I2C address of BMP180
if (ioctl(file, I2C_SLAVE, addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    exit(1);
}

float data;
char channel;
int i;
unsigned char res; 
short sh;
long UT, UP;
long T, P;
int calib_data[11];

/* Read calibration data */
  for(i=0XAA;i<=0XBF;i++){    
  get_i2c_register(file,addr,i,&res);
    if(i%2==0){
     //first byte
      sh=0;
      sh=res;
      sh=sh<<8;
    }else{
      sh=sh|res;
      if(i==0xB1 || i==0xB3 || i==0xB5){  
       calib_data[(i-0XAA)/2]=(int)(unsigned short)sh;
      }else{ 
       calib_data[(i-0XAA)/2]=sh;
      }
    }
  }
   
/* Check calibration data */
//should not be 0 or FF 
  for(i=0;i<11;i++){
   // printf("Calib %d :  %05d\n",i,calib_data[i]);
   if(calib_data[i]==0 || calib_data[i]==0xFFFF){printf("Error! Unable to obtain calibration data.\n");return 1;}
  }
  
  /* Read uncompensated temperature */
  if(set_i2c_register(file,addr,0xF4,0x2E)){return 2;}
  usleep(5000);   
  if(get_i2c_register(file,addr,0xF6,&res)){return 2;}
  UT=res;
  UT=UT<<8;
  if(get_i2c_register(file,addr,0xF7,&res)){return 2;}
  UT=UT|res;  
  
  /* Read uncompensated pressure */
  if(set_i2c_register(file,addr,0xF4,0x34+(OSS<<6))){return 2;}
  usleep(WAIT_TIME);   
  if(get_i2c_register(file,addr,0xF6,&res)){return 2;}
  UP=res;
  UP=UP<<8;
  if(get_i2c_register(file,addr,0xF7,&res)){return 2;}
  UP=(UP|res)<<8;
  if(get_i2c_register(file,addr,0xF8,&res)){return 2;} 
  UP=(UP|res) >> (8-OSS);
  
  /* Calculate true temperature */
  long X1=(UT - calib_data[5])*calib_data[4] / (1<<15);   
  long X2=calib_data[9]*(1<<11) / (X1+calib_data[10]);
  long B5=X1+X2;
  T=(B5+8)/(1<<4);
                
  /* Calculate true pressure */
  long B6=B5-4000;
  X1 = (calib_data[7] * (B6*B6/(1<<12)))/(1<<11);
  X2 = calib_data[1] * B6/(1<<11);
  long X3= X1+X2;
  long B3=(((calib_data[0]*4+X3)<<OSS)+2)/4;
  X1=calib_data[2]*B6/(1<<13);
  X2=(calib_data[6]*(B6*B6/(1<<12)))/(1<<16);
  X3=((X1+X2)+2)/(1<<2);
  unsigned long B4= calib_data[3] * (unsigned long)(X3+32768) / (1<<15);
  unsigned long B7=((unsigned long)UP-B3)*(50000>>OSS);
  
  if(B7<0x80000000){P=(B7*2)/B4;}else{P=(B7/B4)*2;}
  X1=(P/(1<<8)) * (P/(1<<8));
  X1=(X1 * 3038)/(1<<16);
  X2=(-7357 * P)/(1<<16);
  P=P+(X1+X2+3791)/(1<<4);
         //330 dle google                
         //http://pocasi.ok5aw.cz/teorie.php?doc=page1
         long Prel=P+(int)((float)(100*amsl)/8.3);
  printf("%3.1f %d\n",(float)T*0.1,Prel);   
  return 0;
}
