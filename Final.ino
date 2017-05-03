#include <LiquidCrystal.h>

#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9

const int NBSTEPS = 4096;
const int STEPTIME = 900;
int Step = 0;
boolean Clockwise = true;

int arrayDefault[4] = {LOW, LOW, LOW, LOW};

int stepsMatrix[8][4] =
{
  {LOW, LOW, LOW, HIGH},
  {LOW, LOW, HIGH, HIGH},
  {LOW, LOW, HIGH, LOW},
  {LOW, HIGH, HIGH, LOW},
  {LOW, HIGH, LOW, LOW},
  {HIGH, HIGH, LOW, LOW},
  {HIGH, LOW, LOW, LOW},
  {HIGH, LOW, LOW, HIGH},
};

unsigned long lastTime;
unsigned long time;

char pw[9];
char pw1[9];
char pw2[9];
char pcattempt[9];

boolean pwset = false;
boolean pwset1 = false;
boolean pwset2 = false;
boolean pwattemptset = false;
int i = 0;
int pwlength;
int j = 0;
int k = 0;
int l = 0;
int a;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buzzer = 10; //buzzer to arduino pin 10

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Enter desired");
  lcd.setCursor(0, 1);
  lcd.print("passcode.");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{
  if (Serial.available() > 0)
  {
    lcd.clear();
    delay(500);
    a = Serial.available();
    if (pwset == true)
    {
      pcattemptset();
      if (pwattemptset == true)
      {
        verify();
      }
    }
    else
    {
      setPW();
    }
  }
}

void verify()
{
  boolean matchpass = true;
  int b;
  for (b = 0; b < 3; b++)
  {
    if (pcattempt[b] != pw[b])
    {
      matchpass = false;
    }
  }
  if (matchpass == true)
  {
    lcd.clear();
    lcd.print("Password is");
    lcd.setCursor(0, 1);
    lcd.print("correct.");
    motorize();
    delay(5000);
    motorize();
    pwattemptset = false;
    pcattempt[0] = char(0);
    l = 0;
    lcd.clear();
    lcd.print("Enter your");
    lcd.setCursor(0, 1);
    lcd.print("passcode.");
  }
  else
  {
    lcd.clear();
    lcd.print("Password is");
    lcd.setCursor(0, 1);
    lcd.print("incorrect.");
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(100);
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    delay(2000);
    pwattemptset = false;
    pcattempt[0] = char(0);
    l = 0;
    lcd.clear();
    lcd.print("Enter your");
    lcd.setCursor(0, 1);
    lcd.print("passcode.");
  }
}

void setPW()
{
  if (pwset1 == false)
  {
    setPW1();
    lcd.clear();
    lcd.print("Confirm your");
    lcd.setCursor(0, 1);
    lcd.print("passcode.");
  }
  else if (pwset2 == false)
  {
    setPW2();
    if (pwset2 == true)
    {
      check();
    }
  }
}

void setPW1()
{
  pw1[j] = Serial.read();
  j++;
  if (a == 1)
  {
    pwset1 = true;
  }
}

void setPW2()
{
  pw2[k] = Serial.read();
  k++;
  if (a == 1)
  {
    pwset2 = true;
  }
}

void pcattemptset()
{
  pcattempt[l] = Serial.read();
  l++;
  if (a == 1)
  {
    pwattemptset = true;
  }
}

void check()
{
  boolean match = true;
  int b;
  for (b = 0; b < 3; b++)
  {
    if (pw1[b] != pw2[b])
    {
      match = false;
    }
  }
  if (match == true)
  {
    pwset = true;
    pw[0] = pw1[0];
    pw[1] = pw1[1];
    pw[2] = pw1[2];
    pw[3] = pw1[3];
    pw[4] = pw1[4];
    pw[5] = pw1[5];
    pw[6] = pw1[6];
    pw[7] = pw1[7];
    pw[8] = pw1[8];
    pw[9] = pw1[9];
    lcd.clear();
    lcd.print("Password has");
    lcd.setCursor(0, 1);
    lcd.print("been confirmed.");
    delay (2000);
    lcd.clear();
    lcd.print("Enter your");
    lcd.setCursor(0, 1);
    lcd.print("passcode.");
  }
  else
  {
    pw1[0] = (char)0;
    pw2[0] = (char)0;
    pwset = false;
    pwset1 = false;
    pwset2 = false;
    j = 0;
    k = 0;
    lcd.clear();
    lcd.print("Passcodes did");
    lcd.setCursor(0, 1);
    lcd.print("not match.");
    delay(2000);
    lcd.clear();
    lcd.print("Enter desired");
    lcd.setCursor(0, 1);
    lcd.print("passcode.");
  }
}

void motorize()
{
  unsigned long currentMicros;
  int stepsLeft = NBSTEPS;
  time = 0;
  while (stepsLeft > 0)
  {
    currentMicros = micros();
    if (currentMicros - lastTime >= STEPTIME) {
      stepper();
      time += micros() - lastTime;
      lastTime = micros();
      stepsLeft--;
    }
  }
  delay(2000);
  Clockwise = !Clockwise;
  stepsLeft = NBSTEPS;
}

void writeStep(int outArray[4])
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(IN1 + i, outArray[i]);
  }
}

void stepper()
{
  if ((Step >= 0) && (Step < 8))
  {
    writeStep(stepsMatrix[Step]);
  } else {
    writeStep(arrayDefault);
  }
  setDirection();
}

void setDirection()
{
  (Clockwise) ? (Step++) : (Step--);

  if (Step > 7) {
    Step = 0;
  } else if (Step < 0) {
    Step = 7;
  }
}

