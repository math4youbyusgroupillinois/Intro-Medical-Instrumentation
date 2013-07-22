#define PhotoResistor A0
#define LED 10


void setup()
{
  pinMode(PhotoResistor, INPUT); //not necessary - just declaring the photoresistor pin as an input
  pinMode(LED, OUTPUT); //define LED as an output
  Serial.begin(9600); //9600 baud - the speed at which we communicate with the computer, pretty standard speed
}

void loop()
{
  int PhotoIn = analogRead(PhotoResistor); //read in the input from the photoresistor, remember that the higher the value, the darker the environment is
  Serial.println(PhotoIn); //we can view the values it reads from the ADC on the computer
  //let's say that after a certain brightness, we want to turn the LED off - less than 750
  //let's also say that after a certain darkness, the LED will be on full brightness - greater than 900
  //in between let's have it vary based on the darkness
  int PhotoIn2 = constrain(PhotoIn, 750, 900); //this will constrain the values such that anything below 750 becomes 750, and anything above 900 becomes 900
  int PhotoIn3 = map(PhotoIn2, 750, 900, 0, 255); //this maps it in reverse - so that the darkest environment will produce the greatest brightness
  analogWrite(LED, PhotoIn3); //outputs a PWM wave on the LED pin, which has a default frequency of 500Hz. 
  //The second value simply changes the duty cycle and is an 8-bit number. This means the highest value is 255, which represents 100% on.
  delay(100); //delay for 100 ms
}
