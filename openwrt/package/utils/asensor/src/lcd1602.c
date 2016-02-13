#include <stdio.h>  
#include <stdlib.h>   /* atoi */
//#include "LiquidCrystal/LiquidCrystal_I2C.h"
//#include "LiquidTWI/LiquidTWI.h"               
#include "LiquidCrystal_I2C/LiquidCrystal_I2C.h"

      
#define I2C_ADDR    0x20
//Definitions moved to I2CIO.h

    
 


int main(int argc, char * * argv){
       
     
uint8_t pins[7] = {2,3,0}; 
//3 7 nadejne, nebo 3 6 nadejne 
//2 5 4
//2 0 7    
     
//permute(pins, 0,2);     	   

LiquidCrystal_I2C lcd (I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin,BACKLIGHT_PIN,POSITIVE); //POSITIVE


            
   
  /* 
  Commands (argv[1]): 
   1 "one Line"
   2 "Two " "Lines"
   3 "light OFF"  
   4 "display OFF"
  */      
  int cmd=atoi(argv[1]);
  
  if(cmd==0){
   lcd.begin(16, 2);
  }
  
  if(cmd==1 || cmd==2){       
   //lcd.begin (16, 2);
   lcd.internalBegin (16, 2);
    //lcd.setBacklight(HIGH);   
    lcd.display();   
    //lcd.blink();          
    //lcd.cursor();
    lcd.home ();             
    //lcd.cursor();     
    lcd.setCursor(0,0);  
    lcd.print(argv[2]);    
    if(cmd==2){
      lcd.setCursor(0,1);  
      lcd.print(argv[3]);
    }   
  }
  if(cmd==3){    
   lcd.internalBegin (16, 2);
    lcd.setBacklight(LOW,1);
  }
  if(cmd==4){      
   lcd.internalBegin (16, 2);
    lcd.noDisplay();
  }    
  if(cmd==5){     
   lcd.internalBegin (16, 2);
    lcd.display();
  }
   
   
  
}