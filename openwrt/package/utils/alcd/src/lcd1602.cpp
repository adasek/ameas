/*
 * LCD1602 program - interfacing two-line LCD connected via I2C
 * 
 * Using modified "New LiquidCrystal" library by Francisco Malpartida
 * 
 * Author: Adam Benda, 2016
 * License: creative commons license 3.0 Attribution-ShareAlike CC BY-SA
 * 
 * Usage:
 *  /sbin/lcd1602 [command] [line1] [line2]
 * where [command] is a number:
 *   1 turn on the backlight and display one line (string argument [line1])
 *   2 turn on the backlight and display two lines (string arguments [line1] and [line2])
 *   3 turn off the backlight
 *   4 hide displayed content
 *   5 show displayed content
 * 
 */


#include <stdio.h>  
#include <stdlib.h>   // atoi 
#include <unistd.h>  //usleep
#include <string.h> //strcpy,memcpy
#include "LiquidCrystal_I2C/LiquidCrystal_I2C.h"


#define I2C_ADDR    0x20
//Definitions moved to I2CIO.h

void print_usage() {
    printf("USAGE:\n");

    printf("lcd1602 [command] [line1] [line2]\n");
    printf("where [command] is a number:\n");
    printf("1 turn on the backlight and display one line (string argument [line1])\n");
    printf("2 turn on the backlight and display two lines (string arguments [line1] and [line2])\n");
    printf("3 turn off the backlight\n");
    printf("4 hide displayed content\n");
    printf("5 show displayed content\n");
}

int main(int argc, char * * argv) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

int timeLimit=10; //in seconds
int timeElapsed=0; //in useconds

    uint8_t pins[7] = {2, 3, 0};

    //This order was fun to determine.
    //I had to permute pins...

    try {
        LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin, BACKLIGHT_PIN, POSITIVE);

        int cmd = atoi(argv[1]);

        if (cmd == 0) {
            lcd.begin(16, 2);
        } else if (cmd == 1 || cmd == 2) {
            if (argc < 3) {
                printf("Not enough arguments!\n");
                print_usage();
                return 0;
            }

            lcd.internalBegin(16, 2);

		lcd.clear();
                lcd.display();
                lcd.home();
                lcd.setCursor(0, 0);
		 
		int lenMax=16; 
		if(strlen(argv[2])>lenMax){
			lenMax=strlen(argv[2]);
			}
		if(strlen(argv[3])>lenMax){
			lenMax=strlen(argv[3]);
			}

               lcd.print(argv[2]);

		int line1Size=strlen(argv[2]);


            if (cmd == 2) {

                if (argc < 4) {
                    printf("Not enough arguments!\n");
                    print_usage();
                    return 0;
                }

                lcd.setCursor(0, 1);
	
                lcd.print(argv[3]);

            }

                usleep(2000000);
		timeElapsed+=2000000;
	//Now scroll
            while (true) {
		if(lenMax>16){
		 lcd.scrollDisplayLeft();
		}
                 usleep(timeLimit*1000*1000/lenMax);
		timeElapsed+=timeLimit*1000*1000/lenMax;
		if(timeElapsed>timeLimit*1000*1000){
			//fun is over
			break;
		}
            }

        } else if (cmd == 3) {
            lcd.internalBegin(16, 2);
            lcd.setBacklight(LOW, 1);
        } else if (cmd == 4) {
            lcd.internalBegin(16, 2);
            lcd.noDisplay();
        } else if (cmd == 5) {
            lcd.internalBegin(16, 2);
            lcd.display();
        } else {
            print_usage();
            return 0;
        }
    } catch (int errCode) {
        fprintf(stderr, "Error communicating with display (errCode=%d)\n", errCode);
        return errCode;
    }

    return 0;
}
