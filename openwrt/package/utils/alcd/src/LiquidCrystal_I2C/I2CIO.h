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
// @version API 1.0.0
//
// @author F. Malpartida - fmalpartida@gmail.com
//
//
//
// Modified 2015-03-30 by Adam Benda: Changed to not use Arduino API.
// ---------------------------------------------------------------------------

#ifndef _I2CIO_H_
#define _I2CIO_H_


#define BACKLIGHT_PIN  1  //overeno!
#define En_pin  6
#define Rw_pin  0  //not used?
#define Rs_pin  7
#define D4_pin  5
#define D5_pin  4
#define D6_pin  3
#define D7_pin  2   

#include <inttypes.h>
                          
#include <unistd.h> //usleep                                                     
//i2c relevant functions  
#include <stdio.h>        
#include <fcntl.h> //file open  
#include <linux/i2c.h>   
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>  //ioctl

#define _I2CIO_VERSION "1.0.0"        
#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0


// registers
#define MCP23008_IODIR 0x00
#define MCP23008_IPOL 0x01
#define MCP23008_GPINTEN 0x02
#define MCP23008_DEFVAL 0x03
#define MCP23008_INTCON 0x04
#define MCP23008_IOCON 0x05
#define MCP23008_GPPU 0x06
#define MCP23008_INTF 0x07
#define MCP23008_INTCAP 0x08
#define MCP23008_GPIO 0x09
#define MCP23008_OLAT 0x0A


/*!
 @class
 @abstract    I2CIO
 @discussion  Library driver to control PCF8574 based ASICs. Implementing
 library calls to set/get port through I2C bus.
 */

class I2CIO  
{
public:
   /*!
    @method     
    @abstract   Constructor method
    @discussion Class constructor constructor. 
    */
   I2CIO ( );
   
   /*!
    @method
    @abstract   Initializes the device.
    @discussion This method initializes the device allocating an I2C address.
    This method is the first method that should be call prior to calling any
    other method form this class. On initialization all pins are configured
    as INPUT on the device.
    
    @param      i2cAddr: I2C Address where the device is located. 
    @param      I2Cfilename: Filename of i2c bus
    @result     1 if the device was initialized correctly, 0 otherwise
    */   
   int begin ( uint8_t i2cAddr, char * I2Cfilename="/dev/i2c-0" );
   
   /*!
    @method
    @abstract   Sets the mode of a particular pin.
    @discussion Sets the mode of a particular pin to INPUT, OUTPUT. digitalWrite
    has no effect on pins which are not declared as output.
    
    @param      pin[in] Pin from the I2C IO expander to be configured. Range 0..7
    @param      dir[in] Pin direction (INPUT, OUTPUT).
    */   
   void pinMode ( uint8_t pin, uint8_t dir );
   
   /*!
    @method
    @abstract   Sets all the pins of the device in a particular direction.
    @discussion This method sets all the pins of the device in a particular
    direction. This method is useful to set all the pins of the device to be
    either inputs or outputs.
    @param      dir[in] Direction of all the pins of the device (INPUT, OUTPUT).
    */
   void portMode ( uint8_t dir );
   
   
   
   /*!
    @method
    @abstract   Write a value to the device.
    @discussion Writes to a set of pins in the device. The value is the binary
    representation of all the pins in device. The value written is masked with 
    the configuration of the direction of the pins; to change the state of
    a particular pin with this method, such pin has to be configured as OUTPUT 
    using the portMode or pinMode methods. If no pins have been configured as
    OUTPUTs this method will have no effect.
    
    @param      value[in] value to be written to the device.
    @result     1 on success, 0 otherwise
    */   
   int write ( uint8_t value );
   
   /*!
    @method
    @abstract   Writes a digital level to a particular pin.
    @discussion Write a level to the indicated pin of the device. For this 
    method to have effect, the pin has to be configured as OUTPUT using the
    pinMode or portMode methods.
    
    @param      pin[in] device pin to change level. Range (0..7).
    @para       level[in] logic level to set the pin at (HIGH, LOW).
    @result     1 on success, 0 otherwise.
    */   
   int digitalWrite ( uint8_t pin, uint8_t level );
   
   
   
private:
   uint8_t _shadow;      // Shadow output
   uint8_t _dirMask;     // Direction mask
   uint8_t _i2cAddr;     // I2C address
   bool    _initialised; // Initialised object
        
    //Added by Adam Benda 
	size_t buffSize; 
	int I2Cfile; 
	uint8_t buffer[100];
public:
  bool beginTransmission(uint8_t addr);
  bool endTransmission();
  void delayMicroseconds(int us);
  void delay(int s);                           
	void sendI2C (uint8_t value); 
	
};

#endif