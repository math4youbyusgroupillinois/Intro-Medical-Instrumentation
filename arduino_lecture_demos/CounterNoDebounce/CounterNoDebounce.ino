#define LEDg 10
#define LEDr 11
#define LEDy 12
#define pushButton 9

int counter = 0; //declare variables
boolean lastButton = 0;
boolean currentButton = 0;

void setup()
{
  pinMode(pushButton, INPUT); //declare pushbutton as input
  pinMode(LEDg, OUTPUT); //declare LEDs as outputs
  pinMode(LEDr, OUTPUT);
  pinMode(LEDy, OUTPUT);
}

void loop()
{
  currentButton = digitalRead(pushButton); //read the value from the pushButton pin
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
  if (counter >= 4) digitalWrite(LEDg, HIGH); //if counter is 4, 5, 6, or 7, we should turn on the first LED
  else digitalWrite(LEDg, LOW);

  if ( (counter % 4) >= 2) digitalWrite(LEDr, HIGH); //if counter is 2, 3, 6, or 7, we should turn on the second LED
  else digitalWrite(LEDr, LOW);

  if ( (counter % 2) >= 1) digitalWrite(LEDy, HIGH); //if counter is 1, 3, 5, or 7, we should turn on the third LED
  else digitalWrite(LEDy, LOW);
}

