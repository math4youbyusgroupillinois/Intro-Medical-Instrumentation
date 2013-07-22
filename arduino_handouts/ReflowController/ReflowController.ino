//Declare which libraries to include
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <PID_v1.h>

//Initialize LCD Pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Define pins
int soundPin = 3; //this pin drives piezo buzzer
int relayPin = 2; //this pin drives the relay
int buttonPin = A0;
int thermocouplePin = A5;

//Define ADC buttons
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//Define scalar values
#define tempScale 2
#define timeScale 5

//Initialize variables
int btnPrs = 0;
int UIcursor = 0;
int ovenMode = 0; //0 = start in set mode, 1 = confirm mode, 2 = run mode
boolean confirm = 0;
int maxTemp = 997; //calibrate according to thermocouple and ADC
int delayTime = 250; //default delay time
int delayCounter = 0;

/*
boolean relay_State = 0; //0 = relay OFF, 1 = relay ON
 int currentTemp = 0;     // Current Temperature (0-1023)
 int setPoint = 0;        // Setpoint which PID controller is trying to reach
 boolean userSet = 0;
 */

//Declare number of set points
const int numPoints = 3; //This is the number of setpoints (No more than 9)
int temps[numPoints];  //array of temperature set points
int times[numPoints];
int k1Set[numPoints]; //PID coefficient1
int k2Set[numPoints]; // PID coefficient2
int k3Set[numPoints]; // PID coefficient3

//Define PID variables
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double kp = 2; //(proportional gain - difference between the current temperature and the set point)
double ki = 0.1; //(integral gain - proportional to the magnitude and duration of error)
double kd = 5; //(derivative gain - slope of the error over time)
PID myPID(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT); 

//WindowSize is essentially the period of the PWM
int WindowSize = 500;
unsigned long windowStartTime;

/****************************************************************************************************************
 * setup
 * 
 * Initializes the LCD.
 * Initializes the Serial port.
 * Initializes the EEPROM values if necessary.
 ****************************************************************************************************************/
void setup()
{
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  Serial.begin(9600);
  for (int index = 0; index < numPoints; index++)
  {
    //locations 0 through numPoints-1 are reserved for temperature set points
    int readVal = EEPROM.read(index)*tempScale;
    if(readVal > 350) //if not yet initialized, set to default 100 degreees C
    {
      EEPROM.write(index, 100 / tempScale); //stored in multiples of *scale*
    }
    //locations numPoints through 2*numPoints-1 are reserved for time setpoints
    int readVal2 = EEPROM.read(index + numPoints)*timeScale;
    if(readVal2 > 1000) //if not yet initialized set to default 100 seconds
    {
      EEPROM.write(index + numPoints, 100 / timeScale); //stored in multiples of *scale*
    }
    EEPROM.write(numPoints*2, 0); //write 0 to the last EEPROM slot
  }
}

/****************************************************************************************************************
 * loop
 * 
 * Loops through different oven modes.
 ****************************************************************************************************************/
void loop()
{
  while(ovenMode == 0) //set mode
  {
    printMsg(UIcursor);
    btnPrs = getButtonPress();
    delay(delayTime);
    selectAction(btnPrs);
  }
  while(ovenMode == 1) //confirm mode
  {
    btnPrs = getButtonPress();
    delay(delayTime);
    selectAction(btnPrs);
    //Serial.println(ovenMode);
    //Serial.println(confirm);
  }
  while(ovenMode == 2) //run mode
  {
    lcd.setCursor(0, 0);
    lcd.print("Running...");
    checkPID();
    for (int i = 0; i < numPoints; i++)
    {
      unsigned long timeRef = millis(); //record start time
      PIDinit(EEPROM.read(i)*tempScale*(1024/maxTemp)); //set the set point to the first temperature set point
      Serial.print("New Target ");
      Serial.println(EEPROM.read(i)*tempScale*(1024/maxTemp));
      while ((millis() - timeRef)/1000 < EEPROM.read(i + numPoints)*timeScale) //while still within the first setpoint time
      {
        int timeLeft = (EEPROM.read(i + numPoints)*timeScale - (millis()-timeRef)/1000);
        //Serial.println((millis() - timeRef)/1000);
        //Serial.println(EEPROM.read(i + numPoints)*timeScale);
        lcd.setCursor(11,0);
        printTime(timeLeft);
        int currTemp = runPID();
        printTemp(currTemp, i);
        delay(100);
      }
    }
    //Set the last set point to 0
    PIDinit(0);
    int currTemp = runPID();
    lcd.setCursor(0,0);
    lcd.print("Cooling...      "); //clear display
    lcd.setCursor(12,1);
    lcd.print("    "); //clear display
    while (currTemp > 50) //While temperature is greater than 50
    {
      //currTemp = runPID();
      printTemp(currTemp, 2*numPoints);
      delay(100);
    }

    //Switch to wait mode
    lcd.clear();
    ovenMode = 3;
  }
  while(ovenMode == 3) //wait mode
  {
    printDone();
    btnPrs = getButtonPress();
    delay(delayTime);
    selectAction(btnPrs);
  }
}

/****************************************************************************************************************
 * getButtonPress
 * 
 * Takes no input. Reads the analog pin A0 (tied to the push buttons on the LCD). Then returns which button was 
 * pressed based on the read in ADC value.
 ****************************************************************************************************************/
int getButtonPress()
{
  int adcIn;
  while(analogRead(buttonPin) > 1000) //while no button press is detected
  {
    delayCounter = 0; //reset button press counter
    delayTime = 250; //reset to default delay time
  }
  delay(5);
  adcIn = analogRead(buttonPin);
  Serial.println(adcIn);
  if (adcIn < 50) return btnRIGHT;
  else if (adcIn < 195) return btnUP;
  else if (adcIn < 380) return btnDOWN;
  else if (adcIn < 555) return btnLEFT;
  else if (adcIn < 790) return btnSELECT;
  else 
  {
    delayCounter = 0; //reset button press counter
    delayTime = 250; //reset to default delay time
    return btnNONE; 
  }
}

/****************************************************************************************************************
 * selectAction
 * 
 * Takes an integer as an input. The integer ranges from 0 to 5, representing one of 6 possible buttons, RIGHT, 
 * LEFT, UP, DOWN, SELECT, and NONE. It then performs an action according to which state the program is on.
 * 
 * In Set Mode (ovenMode = 0):
 * Left and Right switch between the different set points.
 * Up and Down increment or decrement the value of the set point by 5.
 * Select asks user if they would like to confirm, and switched to Confirm Mode (ovenMode = 1).
 * 
 * In Confirm Mode (ovenMode = 1):
 * Left and Right have no function.
 * Up and Down switch between Y/N for confirming.
 * Select will switch back to Set Mode on "N" or to Oven Run Mode (ovenMode = 2) on "Y".
 * 
 * In Oven Run Mode (ovenMode = 2):
 * Left and Right have no function.
 * Up and Down have no function. (potentially allow user to view set points)
 * Select has no function. (potentially override oven run)
 * LCD will display "Oven Running..." as well as current state (Ramping to 90C,  etc.)
 * 
 * In Wait Mode (ovenMode = 3);
 * Left and Right have no function.
 * Up and Down have no function.
 * Select will return user to Set Mode (ovenMode = 0);
 ****************************************************************************************************************/
void selectAction(int btn)
{
  switch (btnPrs)
  {
  case btnRIGHT:
    {
      switch (ovenMode)
      {
      case 0:
        {
          if (UIcursor < 2*numPoints-1) //switch set point mode unless at farthest right
          {
            UIcursor = UIcursor + 1;
          }
          printMsg(UIcursor); //print message
          break;
        }
      }
      delay(10);
      while(analogRead(0) < 1000)
      {
        //wait until button is released
      }
      break;
    }
  case btnLEFT:
    {
      switch (ovenMode)
      {
      case 0:
        {
          if (UIcursor > 0) //switch set point mode unless at farthest left
          {
            UIcursor = UIcursor - 1;
          }
          printMsg(UIcursor); //print message
          break;
        }
      }
      delay(10);
      while(analogRead(0) < 1000)
      {
        //wait until button is released
      }
      break;
    }
  case btnUP:
    {
      switch (ovenMode)
      {
      case 0:
        {
          int limit = 350; // temperature limit is 300 C
          int scale = tempScale; // changing temperature -> use tempScale
          if (UIcursor >= numPoints)
          {
            limit = 1000; //time limit is 1000 seconds
            scale = timeScale; // changing time -> use timeScale
          }
          int readVal = EEPROM.read(UIcursor)*scale;
          if (readVal < limit)
          {
            EEPROM.write(UIcursor, (readVal+scale)/scale); //write value to EEPROM
          }
          printMsg(UIcursor);
          delayCounter++; //count how many times button presses is detected
          if (delayCounter > 5) //if button is being held down, decrease delay time for rapid increase
          {
            delayTime = 75;
          }
          break;
        }
      case 1:
        {
          confirm = 1-confirm; //toggle
          lcd.setCursor(9, 0);
          if (confirm == 0) lcd.print("N");
          else if (confirm == 1) lcd.print("Y");
          break;
        }
      }
      break;
    }
  case btnDOWN:
    {
      switch (ovenMode)
      {
      case 0:
        {
          int scale = tempScale; // changing temperature -> use tempScale
          if (UIcursor >= numPoints)
          {
            scale = timeScale; // changing time -> use timeScale
          }
          int readVal = EEPROM.read(UIcursor)*scale;
          if (readVal > 0)
          {
            EEPROM.write(UIcursor, (readVal-scale)/scale); //write value to EEPROM
          }
          printMsg(UIcursor);
          delayCounter++; //count how many times button presses is detected
          if (delayCounter > 5) //if button is being held down, decrease delay time for rapid increase
          {
            delayTime = 75;
          }
          break;
        }
      case 1:
        {
          confirm = 1-confirm; //toggle
          lcd.setCursor(9, 0);
          if (confirm == 0) lcd.print("N");
          else if (confirm == 1) lcd.print("Y");
          break;
        }
      }
      break;
    }
  case btnSELECT:
    {
      switch (ovenMode)
      {
      case 0:
        {
          ovenMode = 1;
          lcd.clear();
          lcd.print("Confirm?");
          confirm = 1;
          lcd.setCursor(9, 0);
          lcd.print("Y");
          break;
        }
      case 1:
        {
          if (confirm == 1) ovenMode = 2; //enter oven run mode
          else if (confirm == 0) ovenMode = 0; //return to set mode
          break;
        }
      case 3:
        {
          ovenMode = 0;
          break;
        }
      }
      delay(10);
      while(analogRead(0) < 1000)
      {
        //wait until button is released
      }
      break;
    }
  case btnNONE:
    {
      break;
    }
  }    
}


/****************************************************************************************************************
 * checkPID
 * 
 * checkPID will make sure the temperature and time setpoints are attainable using the current PID parameters. It 
 * will report to the user whether or not all tests have passed.
 ****************************************************************************************************************/
void checkPID()
{

}

/****************************************************************************************************************
 * PIDinit
 * 
 * Initializes PID parameters, setting the setpoint to x.
 ****************************************************************************************************************/
void PIDinit(int x)
{
  pinMode(relayPin, OUTPUT);
  windowStartTime = millis();
  PIDsetup(x, WindowSize);
}

/****************************************************************************************************************
 * PIDinit
 * 
 * Sets PID Setpoint and output limits.
 ****************************************************************************************************************/
void PIDsetup(int x, int WindowSize)
{
  //initialize the variables we're linked to
  Setpoint = x;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC); 
}


/****************************************************************************************************************
 * runPID
 * 
 * Calculates necessary PWM based on set point and PID parameters and controls relay.
 ****************************************************************************************************************/
int runPID()
{
  Input = analogRead(thermocouplePin);
  Serial.println(Input);
  myPID.Compute();

  //turn the output pin on/off based on pid output
  unsigned long now = millis();

  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
    //Serial.print("Temperature: ");
    //Serial.println(analogRead(thermocouplePin));

    //Serial.print("Output: ");
    //Serial.println(Output);
  }

  if(Output > now - windowStartTime) 
  {
    digitalWrite(relayPin, HIGH);
  }
  else 
  {
    digitalWrite(relayPin, LOW);
  }
  //Serial.println(Input);
  return 2*Input/((1024/maxTemp)*tempScale); //return temperature reading
}

/****************************************************************************************************************
 * printMsg
 * 
 * Takes an integer ranging from 0 to 2*numPoints-1 as an input. It prints the corresponding message on the LCD to
 * display to the user which Set Point Mode the user is currently adjusting.
 ****************************************************************************************************************/
void printMsg(int state)
{
  lcd.clear();
  if (state < numPoints) //printing temperature
  {
    lcd.print("Temp Set Pt ");
    lcd.print(state+1);
    lcd.setCursor(0, 1);
    lcd.print(EEPROM.read(state)*tempScale);
    lcd.print(" C");
  }
  else
  {
    lcd.print("Time Set Pt ");
    lcd.print(state-numPoints+1);
    lcd.setCursor(0, 1);
    printTime(EEPROM.read(state)*timeScale);
  }
}

/****************************************************************************************************************
 * printTime
 * 
 * Prints time in minutes and seconds
 ****************************************************************************************************************/
void printTime(int time)
{
  //Serial.println(timeLeft);

  if (time / 60 < 10)
  {
    lcd.print(" "); //space if less than 10 minutes
  }
  lcd.print(time / 60);
  lcd.print(":");
  if (time % 60 >= 10)
  {
    lcd.print(time % 60);
  }
  else
  {
    lcd.print("0");
    lcd.print(time % 60);
  }
}

/****************************************************************************************************************
 * printTemp
 * 
 * While oven is running, prints the current temperature and the target temperature
 ****************************************************************************************************************/
void printTemp(int currTemp, int index)
{
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(currTemp);
  lcd.print("C "); //space is to clear last character

  lcd.setCursor(7,1);
  lcd.print("Tgt");
  lcd.print(index+1);
  lcd.print(":");
  lcd.print(EEPROM.read(index)*tempScale);
  lcd.print("C "); //space is to clear last character
}

/****************************************************************************************************************
 * printDone
 * 
 * Prints output when oven is done running.
 ****************************************************************************************************************/
void printDone()
{
  lcd.setCursor(0,0);
  lcd.print("Done");
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(analogRead(thermocouplePin)/((1024/maxTemp)*tempScale));
  lcd.print("C "); //space is to clear last character

  lcd.setCursor(8,0);
  lcd.print("SELECT");
  lcd.setCursor(8,1);
  lcd.print("to Reset");
}


