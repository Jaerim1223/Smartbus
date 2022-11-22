#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;

float calibration_factor = -372.0;
float Weight = 0;

// 로드센서의 임계값. 이 값을 넘으면 사람이 있음을 의미함
const float PRESS = 10.0;
// 리드 스위치: 7번에 연결
// 다른 포트에 연결되어 있을경우 값을 바꾸세요
const int reedSW = 7;

int readValue_curr = 0;
int readValue_prev = 0;
int currentState = 0;
char dispString[16] = {0x00, };


void setup() {
pinMode(reedSW, INPUT_PULLUP);
Serial.begin(9600);
// LCD
lcd.init ();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("CJU");
lcd.setCursor(6, 1);
lcd.print("VUASRL");
delay(2000);
lcd.clear();
// 로드셀
scale.begin(2, 3);
scale.set_scale();
scale.tare();
readValue_prev = digitalRead(reedSW);
}

void loop()
{
scale.set_scale(calibration_factor);
Weight = (scale.get_units()*0.01);

readValue_curr = digitalRead(reedSW);
Serial.print("W: ");
Serial.println(Weight);
Serial.print("SW: ");
Serial.println(readValue_curr);

if (Weight < PRESS) // 사람이 없음
{
currentState = 0;
}
else // 사람이 있음
{
// 현재 안전벨트를 함. 이전 현재 관계없이 현재 안전벨트를 한 상태임
if (readValue_curr==0 && readValue_prev==1)
{
currentState = 2;
}
// 현재 안전벨트를 하지 않음
else if (readValue_curr==0 && readValue_prev==0)
{
// 현재 안전벨트를 하지 않고 이전에도 벨트를 하지 않았음
if (currentState != 2)
currentState = 3;
}
else if(readValue_curr==1)// 현재 안전벨트를 하지 않았음. 그러나 이전에 벨트를 한 기록이 있음
currentState = 1;
}


sprintf(dispString, "State: %d", currentState);
lcd.setCursor(0,0);
lcd.print(dispString);

readValue_prev = readValue_curr;

delay(1000);

} 
