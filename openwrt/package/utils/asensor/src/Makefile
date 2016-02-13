all: compile 

compile: beep_gpio measure_bmp180 measure_dht11 measure_dht22 measure_analog measure_pcf8591 measure_bh1750fvi milisleep   

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
     


clean: 
	@\rm -Rf *.o  beep_gpio measure_bmp180 measure_dht11 measure_dht22 measure_analog measure_pcf8591 measure_bh1750fvi milisleep
	@echo "Clean complete."
	
run:
	./$(BINARY)
