/*
* Copyright (C) 2015 Adam Benda
*
* Simple GPIO talking script to retrieve data from DHT11 sensor.
* http://www.uugear.com/portfolio/dht11-humidity-temperature-sensor-module/
* 
* Arguments:
*  1) number of GPIO pin 
*  2] type of sensor: 11 for DHT11, 22 for DHT22
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <sys/time.h> 
#include <math.h>
#include <inttypes.h>

         

            

int roundF(float a){
 int ceil=(int)a;
 float decimalPart=a-(float)ceil;

 if(decimalPart>=.5){
  return ceil+1;
 }else if(decimalPart<=-0.5){
  return ceil-1;
 }else{
  return ceil;
 }
 
}
            
int setDirection(int gpio, int dir){
  int directionFileDescriptor;
  char buf[100];    
  
  // Direction descriptor
          sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
          directionFileDescriptor = open(buf, O_WRONLY);   
if(directionFileDescriptor<0)
{
fprintf(stderr,"Could not open %s\n",buf);
return 1;
}          

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
     
void startSignal(int gpio){
//bit 0 = 27us of high voltage
//bit 1 = 70us of high voltage
//start signal = 18ms of low voltage
  unsigned long t;
  int gpioFileDescriptor;
  char buf[100];            
  const uint8_t low=0;
  const uint8_t high=0xff;
               
                 
  //open GPIO port
  sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
  gpioFileDescriptor = open(buf, O_WRONLY);
if(gpioFileDescriptor<0)
{
fprintf(stderr,"Could not open %s\n",buf);
}          
        
  setDirection(gpio, 1);
                                     
  
    write(gpioFileDescriptor, low, 1);   //Start signal start    
    usleep(18000); //18ms       
    
    write(gpioFileDescriptor, high, 1);   //end of start signal   
    close(gpioFileDescriptor);
    
    setDirection(gpio, 0);     
}
   
int meas(int gpio,uint8_t * out){ 
unsigned long times[85]; //85            
char c=(char)0;   
int allCounter=0; 
int counter=0;  
int changes=0;
int signalLength;
unsigned long t;
int gpioFileDescriptor;
int i;
char buf[80];
char stat;                   
struct timeval t0;
int r_ret;


             
/* Intialize an array */
for(i=0;i<5;i++){
  out[i]=0;
}

//open GPIO port READ     
sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
gpioFileDescriptor = open(buf, O_RDONLY);
if(gpioFileDescriptor<0)
{
fprintf(stderr,"Could not open %s\n",buf);
}          
    
                
        while(allCounter++<50000){
         lseek(gpioFileDescriptor, 0, SEEK_SET);    
         r_ret=read(gpioFileDescriptor,&c,1);    
         if(1!=r_ret){  
           close(gpioFileDescriptor);
           fprintf(stderr," E:%d\n",r_ret);
           return 2; //could not read -> measurement unsuccessful.
         }
         
         if(stat!=c){
            gettimeofday(&t0, NULL); 
            t=((unsigned long long)t0.tv_sec * 1000000) + t0.tv_usec;
           times[counter++]=t;
          //printf("%c \n",c);
          stat=c; 
         }else{
          //no usleep.
         }
         
         if(counter>=84){
         break; //there should be exactly 2(start signal from DHT)  + 2*40bits changes
         }       
         
        }

        close(gpioFileDescriptor);
        
        //aggregate times array
        out[4]=out[3]=out[2]=out[1]=out[0]=0;
        changes=counter;
        counter=0;
        for(i=0;i<changes-1;i++){
          if(i%2==0 && i!=0){
           //these are the data pulses  
           signalLength=(times[i+1]-times[i]);
           if(signalLength > 10 && signalLength < 50){
           // printf("0 = %u\n",signalLength);
           out[counter/8]=out[counter/8]<<1;
           }                     
           else if((signalLength > 55 && signalLength < 110)){
           // printf("1 = %lu\n",signalLength);
           out[counter/8]=out[counter/8]<<1 | 1;
           }else{
          //  printf("X(%d) = %lu\n",i,signalLength);
           break;
           }
           counter++;
          }
        }

        
       //  printf("%02x %02x %02x %02x %02x\n",out[0],out[1],out[2],out[3],out[4]);
          //verify checksum
        	if ( (out[4] == ( (out[0] + out[1] + out[2] + out[3]) & 0xFF) ) && (out[0]!=0 || out[1]!=0 || out[2]!=0 || out[3]!=0 )  )
        	//
        	{       
        		return 1; //successful
        	}else  {
        		return 0;
        	}
                
      
}
 
 
int convertData(uint8_t * data,float * temperature, float * humidity){  
#ifdef DHT11
  *temperature=(float)(int)data[2];
  *humidity=(float)(int)data[0];   
     //printf("convertData DHT11\n");
   return 1;
#elif defined(DHT22)
   *humidity = (data[0] * 256 + data[1]) / 10.0f;
   *temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
    if (data[2] & 0x80) {
      *temperature *= -1.0f;
     }
     //printf("convertData DHT22\n");
     return 1;
#else
 return 0; 
#endif
}  
   

int main(int argc, char * * argv)
{     
  int gpio = atoi(argv[1]); 
  int i; 
  int fd; //sys class gpio export descriptor   
  char buf[100];
  float temp; 
  float hum;   
  uint8_t data[5];  
  int retVal=0;

  
  if(argc<1){
  printf("Not enough arguments... first argument should be no. of pin");
  return 1;
  }    

#ifdef DHT11

#elif defined(DHT22)

#else
fprintf(stderr,"Wrong device type defined in build\n");
return 13;
#endif

  if(gpio<0 || gpio>30){
     printf("I don't know about this pin %d. Exitting.",gpio);
     return 3;
     }
     

                      
   //Export the gpio pin
  fd = open("/sys/class/gpio/export", O_WRONLY);
  sprintf(buf, "%d", gpio); 
if(fd<0)
{
fprintf(stderr,"Could not open %s\n",buf);
return 5;
}          
  write(fd, buf, strlen(buf));
  close(fd); 
           
            
 startSignal(gpio);



 if(meas(gpio,data) && convertData(data,&temp,&hum)){
    printf("%f %f\n",hum,temp);
    retVal=0;
   }else{
   //error       
    retVal=2;          
   }
       

  //Unexport the gpio pin
  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  sprintf(buf, "%d", gpio); 
  write(fd, buf, strlen(buf));
  close(fd);    
 

return retVal;
}
