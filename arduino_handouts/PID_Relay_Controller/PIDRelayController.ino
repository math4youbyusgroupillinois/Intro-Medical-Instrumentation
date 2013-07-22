
/********************************************************
 Modified PID Relay Controller based on online example
 Authors: Oliver Fang, Will Scheideler
 Date last updated: 1/19/2013
 ********************************************************/

#include <PID_v1.h>
#define RelayPin 6
#define ThermocouplePin A0

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//kp = 2 (proportional gain - difference between the current temperature and the set point)
//ki = 5 (integral gain - proportional to the magnitude and duration of error)
//kd = 1 (derivative gain - slope of the error over time)
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1, DIRECT); 

//WindowSize is essentially the period of the PWM
int WindowSize = 500;
unsigned long windowStartTime;

void setup()
{
  pinMode(RelayPin, OUTPUT);
  windowStartTime = millis(); //set to milliseconds

  PIDsetup(50, WindowSize);

  Serial.begin(9600);
}

void loop()
{
  Input = analogRead(ThermocouplePin);
  myPID.Compute();

  //turn the output pin on/off based on pid output
  unsigned long now = millis();

  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
    Serial.print("Temperature: ");
    Serial.println(analogRead(ThermocouplePin));

    Serial.print("Output: ");
    Serial.println(Output);
  }
  
  if(Output > now - windowStartTime) 
  {
    digitalWrite(RelayPin, HIGH);
  }
  else 
  {
    digitalWrite(RelayPin, LOW);
  }
}

void PIDsetup(int x, int WindowSize)
{
  //initialize the variables we're linked to
  Setpoint = x;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC); 
}

