/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

uint8_t weldTime = 0;
uint16_t weldCounter = 0;
const uint8_t eeAddress = 0;
char lcdString[16+1];

const int buttonPin = 2;    // the number of the pushbutton pin
const int weldPin = 3;    // the number of the solidstate relay enable pin

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 /*if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  */

 // For V1.0 comment the other threshold and use the one below:

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}

void weldTimeUpdate(void) {
  lcd.setCursor(0, 0);
  snprintf (lcdString, 16, "pulse: %i ms%s", weldTime, "                 ");
  lcd.print(lcdString);
  EEPROM.update(eeAddress, weldTime);
}

void weldCntUpdate(void) {
  lcd.setCursor(0, 1);
  snprintf (lcdString, 16, "Pulse cnt: %i%s", weldCounter, "                 ");
  lcd.print(lcdString);
}

void setup() {
  EEPROM.get(eeAddress, weldTime);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  sprintf (lcdString, "pulse: %i ms", weldTime);
  lcd.print(lcdString);

  weldCntUpdate ();
}

int lcd_key_prev = btnNONE;

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);

  lcd_key = read_LCD_buttons();  // read the buttons
  if (lcd_key != lcd_key_prev) {
    lcd_key_prev = lcd_key;
    switch (lcd_key)               // depending on which button was pushed, we perform an action
      {
       case btnRIGHT:
         {
         //lcd.print("RIGHT ");
         break;
         }
       case btnLEFT:
         {
         //lcd.print("LEFT   ");
         break;
         }
       case btnUP:
         {
         //lcd.print("UP    ");
         weldTime++;
         weldTimeUpdate();
         break;
         }
       case btnDOWN:
         {
         //lcd.print("DOWN  ");
         weldTime--;
         weldTimeUpdate();
         break;
         }
       case btnSELECT:
         {
         //lcd.print("SELECT");
         weldCounter++;
         digitalWrite(weldPin, HIGH);
         delay(weldTime);
         digitalWrite(weldPin, LOW);
         weldCntUpdate ();
         
         break;
         }
         case btnNONE:
         {
         //lcd.print("NONE  ");
         break;
         }
     }
  }
  
}

