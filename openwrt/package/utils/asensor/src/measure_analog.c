/*
* Copyright (C) 2015 Adam Benda
*
* Simple GPIO talking script to retrieve data from analog outpu AO
* http://www.raspberrypi-spy.co.uk/2012/08/reading-analogue-sensors-with-one-gpio-pin/
* 
* Arguments:
*  1) number of GPIO pin    
*  2) debug? 1/0
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <sys/time.h> 
#include <math.h>

#define OUTPUT_LOW 0
#define OUTPUT_HIGH 1
#define TRIES_START 20
#define TRIES_NUM 70 
#define TIMEOUT 100 //in ms
#define TIMEOUT_TIMES 3 //how many tries that timeout before ending
   
   
void bSort(long long unsigned * array){ //array is from TRIES_START to TRIES_NUM
long long unsigned tmp;
int changed=0;
int a=0;int b=0;

 for(b=TRIES_START;b<TRIES_NUM;b++){
   changed=0;
   for(a=TRIES_START;a<TRIES_NUM-1;a++){
     if(array[a]>array[a+1]){
      tmp=array[a+1];
      array[a+1]=array[a];
      array[a]=tmp;
      changed++;
     }
   }
   if(changed==0){break;}
 }
}   
   
   
int setDirection(int gpio, int dir){
  int directionFileDescriptor;
  char buf[100];   
  
  // Direction descriptor
          sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
          directionFileDescriptor = open(buf, O_WRONLY);   
          
  if(dir==1){
         write(directionFileDescriptor, "out", 3);
  }else if(dir==0){
         write(directionFileDescriptor, "in", 2);
  }else{
      close(directionFileDescriptor); 
      return 0;
  }     
         close(directionFileDescriptor); 
      return 1;
} 
   
 

int main(int argc, char * * argv)
{     
  
        
  int gpio = atoi(argv[1]);  
  int i; 
  int fd; //sys class gpio export descriptor   
  char buf[100]; 
  char filename[100];
  struct timeval tval;
  unsigned long long t0,t1,t2;
  int r_ret;
  char c;     
  int gpioFileDescriptor; 
  int measNum;
  unsigned long long measRes[TRIES_NUM];
  int debug=0;
  int bad_times=0;
  
  if(argc<1){
  printf("Not enough arguments... first argument should be no. of pin");
  return 1;
  }
  if(argc>2){
   if(argv[2][0]=='1'){
    debug=1;
   }
  }
      
  if(gpio<0 || gpio>30){
     printf("I don't know about this pin %d. Exitting.",gpio);
     return 3;
     }
     

                      
   //Export the gpio pin
  fd = open("/sys/class/gpio/export", O_WRONLY);
  sprintf(buf, "%d", gpio); 
  write(fd, buf, strlen(buf));
  close(fd); 

for(measNum=0;measNum<TRIES_NUM;measNum++){

measurement_begin:          
    //set GPIO pin as an output
    setDirection(gpio, 1);
    //set it low
    write(gpioFileDescriptor, OUTPUT_LOW, 1);   
    //discharges any charge
    usleep(300000); //300ms (was 30ms)  
    
      gettimeofday(&tval, NULL); 
      t0=((unsigned long long)tval.tv_sec * 1000000) + tval.tv_usec;
      t1=t0;
            
    //set GPIO pin as an input
    setDirection(gpio, 0);   

    //open GPIO port READ  
    unsigned int cnt=0; 
    int tMin=30;  
    unsigned long long deltaT=0;
    sprintf(filename, "/sys/class/gpio/gpio%d/value", gpio);
    gpioFileDescriptor = open(filename, O_RDONLY);     
    while(1){
      lseek(gpioFileDescriptor, 0, SEEK_SET);   
      r_ret=read(gpioFileDescriptor,&c,1);
      if(r_ret!=1){
          fprintf(stderr,"Read from gpio failed!\n");
          return 5;
          }
      
       /* Time counter*/
        gettimeofday(&tval, NULL); 
        t2=((unsigned long long)tval.tv_sec * 1000000) + tval.tv_usec;
        deltaT=t2-t1;
          //printf("  %lld\n",deltaT);
        if(deltaT<tMin){
            tMin=deltaT;
            }
        t1=t2;
        
       if(c=='1'){
          //check if time in the last tick was ~tMin
            if(tMin+10<deltaT){  
             //printf("--------------\n");
             goto measurement_begin;
            }
          //else
          break;
          }else if(t2-t0 > TIMEOUT*1000){
            if(++bad_times > TIMEOUT_TIMES){
             //timeout happened in too many times
             printf("0\n");
             return 3;
            }
            goto measurement_begin;
          } 
      }
      
   measRes[measNum]=t2-t0; 
   usleep(10000); //10ms   
}

//Sort
bSort(measRes); 

unsigned long long total=0;
//find average of array minus 20% of extremes
for(measNum=TRIES_START+(TRIES_NUM-TRIES_START)/10;measNum<TRIES_NUM-(TRIES_NUM-TRIES_START)/10;measNum++){
//for(measNum=TRIES_START;measNum<TRIES_NUM;measNum++){
    total+=measRes[measNum]; 
     if(debug){
       printf("%lld\n",measRes[measNum]);
     }
   }
  if(debug){
   printf("----------\n");
  }
printf("%d\n",total/(TRIES_NUM-TRIES_START));
    
        
  //Unexport the gpio pin
  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  sprintf(buf, "%d", gpio); 
  write(fd, buf, strlen(buf));
  close(fd);    
 
 
return 0;
}
