// 초음파 거리에 따른 LED 제어
#define TRIG 13 // TRIG 핀
#define ECHO 12



void setup()
{
  Serial.begin(9600);     // 시리얼 통신을 초당 9600 비트 속도로 시작.
  pinMode(TRIG, OUTPUT);  // 초음파 보내는 핀
  pinMode(ECHO, INPUT);   // 초음파 받는 핀
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  
 long duration,distance;
  
  digitalWrite(TRIG, LOW);  // 디지털 핀 TRIG에 LOW 값 출력 
  delayMicroseconds(2);     // 대기
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  duration = pulseIn(ECHO, HIGH);   // ECHO핀에 HIGH 값이 몇 마이크로초 동안 들어왔는지 반환.
  
  distance = duration / 58.2;
  Serial.println(duration);
  Serial.print("\nDIstance : ");
  Serial.print(distance);
  Serial.println(" Cm");
  
  delay(1000);   // 1초 쉬기
            
  if(distance >= 100){      // 거리가 100cm이상이면
    digitalWrite(7, HIGH);  // 7번 LED on
    digitalWrite(8, LOW);
  }
   else
   {
     digitalWrite(8, HIGH); // 8번 LED on
    digitalWrite(7, LOW);
   }
    
  
}
