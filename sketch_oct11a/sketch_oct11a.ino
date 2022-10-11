#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define PRESS 850
#define OUT 0

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pressSensor[4] = {A0, A1, A2, A3};
const int reedSW[4] = {7, 8, 9, 10};

int readValue_curr[4] = {0,};
int readValue_prev[4] = {0,};
int pressSensorRead[4];
int currentState[4] = {0,};
char dispString[16] = {0x00, };

void setup() {
for (int i=0;i<4;i++)
{
pinMode(pressSensor[i], INPUT);
pinMode(reedSW[i], INPUT_PULLUP);
}
Serial.begin(9600);
lcd.init();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("Let start");
delay(2000);
lcd.clear();
for (int i=0;i<4;i++)
{
readValue_prev[i] = digitalRead(reedSW[i]);
}
}

void loop()
{
for (int i=0;i<4;i++)
{
pressSensorRead[i] = analogRead(pressSensor[i]);
readValue_curr[i] = digitalRead(reedSW[i]);
if (pressSensorRead[i] > PRESS)
{
currentState[i] = 0;
}
else
{
if (readValue_curr[i]==0 && readValue_prev[i]==1)
currentState[i] = 2;
else if (readValue_curr[i]==0 && readValue_prev[i]==0)
{
if (currentState[i] != 2)
currentState[i] = 3;
}
else if (readValue_curr[i]==1)
currentState[i] = 1;
}
}

sprintf(dispString, "1:%d 2:%d 3:%d 4:%d", currentState[0], currentState[1], currentState[2], currentState[3]);
lcd.setCursor(0,0);
lcd.print(dispString);

for (int i=0;i<4;i++)
{
readValue_prev[i] = readValue_curr[i];
}

delay(500);
}