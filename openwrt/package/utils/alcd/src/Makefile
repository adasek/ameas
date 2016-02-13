all: compile 

compile: lcd1602    

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
	@\rm -Rf *.o lcd1602
	@echo "Clean complete."
	
run:
	./$(BINARY)
