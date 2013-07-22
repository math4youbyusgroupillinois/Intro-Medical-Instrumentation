#include <Bounce.h> //we have to import a library to use the built in debouncer function. the library contains pre-written functions and objects that we can use

#define LEDg 10
#define LEDr 11
#define LEDy 12
#define pushButton 9

int counter = 0; //declare variables
boolean lastButton = 0;
boolean currentButton = 0;

Bounce buttonDebouncer = Bounce(pushButton, 20); 
//we create a new object called buttonDebouncer, and it is of type "Bounce"
//when we create it, it takes two variables, the pin that it will be reading from, as well as the amount of delay of the debouncing

void setup()
{
  pinMode(pushButton, INPUT); 
  pinMode(LEDg, OUTPUT); //declare LEDs as outputs
  pinMode(LEDr, OUTPUT);
  pinMode(LEDy, OUTPUT);
}

void loop()
{
  buttonDebouncer.update(); //this function will look to see if the pin has changed from LOW to HIGH or HIGH to LOW
  currentButton = buttonDebouncer.read(); //this function gets the actual value of the pin
  if (lastButton == 0 && currentButton == 1) //we will remember the previous value, and only detect a "button press" if the value changes from low to high (positive edge triggered); question: how would you detect negative edge?
  {
    counter+=1; //if a button press was detected, add one to the counter
    if (counter >= 8) //since we are only implementing a 3-bit counter, we can only count from 0 to 7
    {
      counter = 0; //reset to 0 if counter went up to 8
    }
    output(counter); //call the output function, with the input being the currnt value of the counter
  }
  lastButton = currentButton;
}

void output(int counter) //declaring a new function
{
  if (counter >= 4) digitalWrite(LEDg, 1); //if counter is 4, 5, 6, or 7, we should turn on the first LED
  else digitalWrite(LEDg, 0);

  if ( (counter % 4) >= 2) digitalWrite(LEDr, 1); //if counter is 2, 3, 6, or 7, we should turn on the second LED
  else digitalWrite(LEDr, 0);

  if ( (counter % 2) >= 1) digitalWrite(LEDy, 1); //if counter is 1, 3, 5, or 7, we should turn on the third LED
  else digitalWrite(LEDy, 0);
}

