
// {Appendix 1 - Tutorial Code}
#include <LiquidCrystal.h>

// BME354L - Palmeri - Spring 2013 Arduino Project
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
byte smiley[8] = {
B00000,
B10001,
B00000,
B00000,
B10001,
B01110,
B00000,
};
void setup() {
lcd.createChar(0, smiley);
lcd.begin(16, 2);
lcd.write(byte(0));
}
void loop() {}
