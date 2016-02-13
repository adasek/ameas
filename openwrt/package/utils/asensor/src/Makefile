all: compile 

compile: beep_gpio measure_bmp180 measure_dht11 measure_dht22 measure_analog lcd1602 measure_pcf8591 measure_bh1750fvi milisleep     # measure_bh1750fvi

beep_gpio:
	$(CC)   $(CFLAGS) $(OBJECTS) -o beep_gpio $(LDFLAGS) beep_gpio.c
	
measure_dht11:
	$(CC)  $(CFLAGS) $(OBJECTS) -D DHT11 -o measure_dht11 $(LDFLAGS) measure_dht11.c  
  
measure_dht22:
	$(CC)  $(CFLAGS) $(OBJECTS) -D DHT22 -o measure_dht22 $(LDFLAGS) measure_dht11.c    
                                                                         
measure_analog:
	$(CC)  $(CFLAGS) $(OBJECTS) -o measure_analog $(LDFLAGS) measure_analog.c   
   
measure_bh1750fvi:
	$(CC)  $(CFLAGS) $(OBJECTS) -o measure_bh1750fvi $(LDFLAGS)  measure_bh1750fvi.c  #-li2c

measure_bmp180:     
	$(CC)  $(CFLAGS) -o measure_bmp180  $(LDFLAGS) measure_bmp180.c # -li2c
                      
milisleep:     
	$(CC)  $(CFLAGS) -o milisleep  $(LDFLAGS) milisleep.c
                                            
measure_pcf8591:     
	$(CC)  $(CFLAGS) -std=c99 -o measure_pcf8591  $(LDFLAGS) measure_pcf8591.c   #-li2c 
                     
       
lcd1602_c: LiquidCrystal_I2C 
	$(CXX) $(CXXFLAGS) -o lcd1602.o -c lcd1602.c   # -li2c 
                  
arduinoPrint:   
	$(CXX) -g -O $(CXXFLAGS) -o print.o  -c LiquidCrystal_I2C/Print.cpp 
                                                                 
I2CIO:   
	$(CXX) -g -O $(CXXFLAGS) -o I2CIO.o  -c LiquidCrystal_I2C/I2CIO.cpp  
                                               
LCD:   
	$(CXX) -g -O $(CXXFLAGS) -o LCD.o  -c LiquidCrystal_I2C/LCD.cpp  
	
LiquidCrystal_I2C: I2CIO LCD arduinoPrint   
	$(CXX) -g -O $(CXXFLAGS) -o LiquidCrystal_I2C.o  -c LiquidCrystal_I2C/LiquidCrystal_I2C.cpp  
     
            
lcd1602: lcd1602_c
	$(CXX) -g $(LDFLAGS) -o lcd1602 print.o I2CIO.o LCD.o LiquidCrystal_I2C.o lcd1602.o
               
   

clean: 
	@\rm -Rf beep_gpio 	*.o milisleep measure_bmp180 measure_bh1750fvi lcd1602
	@echo "Clean complete."
	
run:
	./$(BINARY)
