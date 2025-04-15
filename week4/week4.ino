#include <Wire.h> 		// I2C 통신을 위한 기본 라이브러리	
#include <LiquidCrystal_I2C.h> 	// I2C LCD 라이브러리

#define TRIG 13 // TRIG 핀 (초음파 보내는 핀)
#define ECHO 12 // ECHO 핀 (초음파 받는 핀)
								 
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  lcd.init();				// I2C LCD 초기화
  lcd.backlight();			// 백라이트 켜기
  lcd.print("LDC init");
  delay(2000);
  lcd.clear(); 				// 지운다

  Serial.begin(9600); // 시리얼 통신 속도 맞추기. 초당 9600비트 전송.

  pinMode(TRIG, OUTPUT); // 초음파 센서가 아두이노에 보낸다. OUT
  pinMode(ECHO, INPUT);  // 아두이노가 신호를 받는다. IN
}

void loop()
{
  long duration, distance; 

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn (ECHO, HIGH);
  distance = duration * 17 / 1000;

  
  lcd.setCursor(16,0);			// LCD 16,0 좌표부터 작성
  lcd.print(duration);
  lcd.print(" Distance : ");
  lcd.print(distance);
  lcd.print(" Cm");
  
  for (int position = 0; position < 32; position++) { // 16번 반복
    lcd.scrollDisplayLeft(); 	// 좌측으로 한칸씩 이동 애니메이션 효과
    delay(150);
  }
  lcd.clear();
}
