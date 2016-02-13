// ---------------------------------------------------------------------------
// Created by Francisco Malpartida on 20/08/11.
// Copyright 2011 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// Thread Safe: No
// Extendable: Yes
//
// @file I2CIO.h
// This file implements a basic IO library using the PCF8574 I2C IO Expander
// chip.
// 
// @brief 
// Implement a basic IO library to drive the PCF8574* I2C IO Expander ASIC.
// The library implements basic IO general methods to configure IO pin direction
// read and write uint8_t operations and basic pin level routines to set or read
// a particular IO port.
//
//
// @version API 1.0.0
//
// @author F. Malpartida - fmalpartida@gmail.com   
//
//
//
// Modified 2015-03-30 by Adam Benda: Changed to not use Arduino API.
// ---------------------------------------------------------------------------

#include <inttypes.h>
//includes are in .h file

#include "I2CIO.h"

// CLASS VARIABLES
// ---------------------------------------------------------------------------


// CONSTRUCTOR
// ---------------------------------------------------------------------------
I2CIO::I2CIO ( )
{
   _i2cAddr     = 0x0; 
   _dirMask     = 0xFF;    // mark all as INPUTs
   _shadow      = 0x0;     // no values set
   _initialised = false;
}

 
//
// begin
int I2CIO::begin (  uint8_t i2cAddr, char * I2Cfilename )
{
   _i2cAddr = i2cAddr;
   int result;
   
   //Added by Adam Benda                                 
  I2Cfile = open(I2Cfilename, O_RDWR); //TODO: 0 is I2C bus_id
  if (I2Cfile < 0) {                                             
    printf("Failed to open the i2c bus.\n");
   throw 3; 
  };
  
  if (ioctl(I2Cfile, I2C_SLAVE, _i2cAddr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
   throw 4; 
  }
  
    // first thing we do is get the GPIO expander's head working straight, with a boatload of junk data.
    beginTransmission(MCP23008_IODIR); 
    sendI2C(0xFF); //was 0xFF
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);
    sendI2C(0x00);	
    result = endTransmission();
#ifdef DETECT_DEVICE
    if (result) {
        if (_deviceDetected == 2) {
          _deviceDetected = 0;
          return;
        }
    }
#endif 
	  
    // now we set the GPIO expander's I/O direction to output since it's soldered to an LCD output.
    beginTransmission(MCP23008_IODIR); 
    sendI2C(0x00); // all output: 00000000 for pins 1...8    
    _dirMask=0x00;
    result = endTransmission();
  
  
   _initialised=true;
   return !result; //returns 1 for success
}

//
// pinMode
void I2CIO::pinMode ( uint8_t pin, uint8_t dir )
{
   if ( _initialised )
   {
      if ( OUTPUT == dir )
      {
         _dirMask &= ~( 1 << pin );
      }
      else 
      {
         _dirMask |= ( 1 << pin );
      }
   }
}

//
// portMode
void I2CIO::portMode ( uint8_t dir )
{
   
   if ( _initialised )
   {
      if ( dir == INPUT )
      {
         _dirMask = 0xFF;
      }
      else
      {
         _dirMask = 0x00;
      }
   }
}


//
// write
int I2CIO::write ( uint8_t value )
{
   int status = 0;
   
   if ( _initialised )
   {
      // Only write HIGH the values of the ports that have been initialised as
      // outputs updating the output shadow of the device
      _shadow = ( value & ~(_dirMask) );
   
      beginTransmission ( MCP23008_GPIO );
      sendI2C ( _shadow );     
      status = endTransmission ();
   }
   return ( (status == 0) );
}


//
// digitalWrite
int I2CIO::digitalWrite ( uint8_t pin, uint8_t level )
{
   uint8_t writeVal;
   int status = 0;
   
   // Check if initialised and that the pin is within range of the device
   // -------------------------------------------------------------------
   if ( ( _initialised ) && ( pin <= 7 ) )
   {
      // Only write to HIGH the port if the port has been configured as
      // an OUTPUT pin. Add the new state of the pin to the shadow
      writeVal = ( 1 << pin ) & ~_dirMask;
      if ( level == HIGH )
      {
         _shadow |= writeVal;
                                                      
      }
      else 
      {
         _shadow &= ~writeVal;
      }
      status = this->write ( _shadow );
   }
   return ( status );
}

//
// PRIVATE METHODS
// ---------------------------------------------------------------------------
  
bool I2CIO::beginTransmission(uint8_t reg){
  //buffer has 100B, it should be enough
  buffer[0]=reg; //address of register : MCP23008_GPIO  
  this->buffSize=1;  
  return true;
}

//Modified function set_i2c_register from i2c-func.h
bool I2CIO::endTransmission(){
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  =  _i2cAddr; //I2C defice address
    messages[0].flags = 0;
    messages[0].len   = this->buffSize; 
    messages[0].buf   = buffer;



    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;        
    if(ioctl(I2Cfile, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        delay(30); //patience
        return 1;
    }
    delayMicroseconds(2000);
        return 0;
}


//sendI2C replacement    
void I2CIO::sendI2C (uint8_t value){


this->buffer[buffSize++]=(value);

  //output binary
  /*
while (value) {
    if (value & 1)
        printf("1");
    else
        printf("0");
    value >>= 1;
}
printf("\n");
*/
/*
  //output binary as RS  0R/W D7 D6 D5 D4 - - - - eN
uint8_t myByte[8];
for (int i=0;i<8;i++) {
    if (value & 1)
        myByte[i]=1;
    else
        myByte[i]=0;
    value >>= 1;
}
printf("%d %d %d %d %d %d - - - - %d\n",myByte[Rs_pin],myByte[Rw_pin],myByte[D7_pin],myByte[D6_pin],myByte[D5_pin],myByte[D4_pin],myByte[En_pin]);
  */

 if(buffSize>=100){throw 5;}
}   

                    
void I2CIO::delayMicroseconds(int us){
 usleep(us);
     }
     
void I2CIO::delay(int s){
 delayMicroseconds(1000*s);
}

