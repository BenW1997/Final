 #include <LiquidCrystal.h>
char pw;     
char confirm;  
char match = 0;  
char pc;     
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() 
{
  while(match == 0)
  {
   Serial.begin(9600);         
   pinMode(13, OUTPUT);        
   lcd.begin(16, 2);
   lcd.clear();
   lcd.print("Enter a passcode.");
   if (Serial.available() > 0)
   {
      pw = Serial.read();     
      lcd.clear();
      lcd.print("Confirm your passcode.");
      if (Serial.available() > 0)
      {
        confirm = Serial.read();     
        lcd.clear();
        if (pw == confirm)
        {
          match = 1;
          lcd.print("Passcode confirmed.");
        }
        else
        {
          lcd.print("Passcodes did not match.");
          delay(2000);
        }
      } 
    } 
  }
}

void loop() 
{
  lcd.clear();
  lcd.print("Enter the passcode.");
  if (Serial.available() > 0)
  {
    pc = Serial.read(); 
    if (pc == pw)
    {
      lcd.clear();
      lcd.print("Password is correct.");
    }
    else
    {
      lcd.clear();
      lcd.print("Password is Incorrect");
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(1000);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      delay(100);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
      delay(2000);
    }
  }

}
