/*
Stopwatch.ino - actually acts like a stopwatch, not a time like the handout
mistakenly described!

Oliver Fang provided the base code for this project, and I modified to
demonstrate some different functionality.

Mark Palmeri
mlp6@duke.edu
2013-04-21
*/

#include <LiquidCrystal.h>

// Initialize LCD Pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Define pins
int buttonPin = A0;
int buttonLast = 0;
int buttonVal = 0;

// Define ADC buttons
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

unsigned long now = 0;
unsigned long last = 0;
unsigned long currTime = 0;

int timerMode = 1; // 1 = run mode

void setup()
{
  lcd.begin(16, 2);
  //Serial.begin(9600);
}

void loop()
{
  if(timerMode == 1) //timer is in run mode
  {
    lcd.setCursor(0, 1);
    lcd.print(" Mode: Run  "); //print timer mode to LCD screen
    now = millis(); //get the total time elapsed now
    currTime += (now-last); //add the elapsed time since last time we checked currTime
    last = now; //update/check the last time
    lcd.setCursor(0,0);
    updateLCD(currTime); //update the LCD
    //Serial.println(getButtonPress());
    buttonVal = getButtonPress(); //check if a button was pressed
    if (buttonVal == btnSELECT && buttonLast != btnSELECT) //if the SELECT button was pressed
    {
      timerMode = 0; //switch timer mode to stop
      buttonLast = btnSELECT; //change the last button to SELECT (used for debouncing)
    }
    else if (buttonVal != btnSELECT)
    {
      buttonLast = 0; //otherwise set to default
    }
  }
  if(timerMode == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(" Mode: Pause"); //print timer mode to LCD screen
    buttonVal = getButtonPress(); //check if a button was pressed
    if (buttonVal == btnSELECT && buttonLast != btnSELECT) //if the SELECT button was pressed
    {
      timerMode = 1; //switch timer mode to run
      buttonLast = btnSELECT; //change the last button to SELECT
    }
    else if (buttonVal != btnSELECT)
    {
      buttonLast = 0; //otherwise set to default
    }
    last = millis(); //important: update the last time so we only add time elapsed while in timer mode 1
  }
}
