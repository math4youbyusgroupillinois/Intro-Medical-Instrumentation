int getButtonPress()
{
  int adcIn;
  adcIn = analogRead(buttonPin);
  delay(5); 
  if (adcIn < 50) return btnRIGHT;
  else if (adcIn < 195) return btnUP;
  else if (adcIn < 380) return btnDOWN;
  else if (adcIn < 555) return btnLEFT;
  else if (adcIn < 790) return btnSELECT;
  else return btnNONE; 
}
