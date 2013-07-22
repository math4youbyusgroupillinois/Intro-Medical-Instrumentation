void updateLCD(long time) //input is in milliseconds
{
  lcd.setCursor(0, 0); //move cursor to initial position
  if (time / (60000) < 10) //if less than 10 minutes
  {
    lcd.print(" "); //print a space 
  }
  lcd.print(time / 60000);
  lcd.print(":");
  //print seconds
  if (time % (60000) >= 10000) //60000 milliseconds in a minute
  {
    lcd.print((time % 60000) / 1000);
  }
  else
  {
    lcd.print("0"); //print an extra 0 if number is less than 10 seonds
    lcd.print((time % 60000) / 1000);
  }
  lcd.print(":");
  //print the milliseconds (only first 2 digits)
  if (time % 1000 >= 100) 
  {
    lcd.print((time % 1000) / 10); 
  }
  else
  {
    lcd.print("0"); //print an extra 0 if less than 100 milliseconds
    lcd.print((time % 1000) / 10);
  }
  
}
