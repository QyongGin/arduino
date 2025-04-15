int Vo = A0;    // 센서 출력 핀 (아날로그)
int V_led = 12; // 

float Vo_value = 0;  // 아날로그 센서값 저장
float Voltage = 0;   // 전압값 저장
float dustDensity = 0; // 계산된 먼지 농도 저장

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(V_led, OUTPUT);
  pinMode(Vo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(V_led, LOW);
  delayMicroseconds(280);
  Vo_value = analogRead(Vo);    // 센서 출력값 읽기
  delayMicroseconds(40);
  digitalWrite(V_led,HIGH);
  delayMicroseconds(9680);

  Voltage = Vo_value*5.0 / 1023.0;    // 아날로그 값 -> 전압 변환
  dustDensity = (Voltage - 0.5)/0.005; // 전압 -> 먼지 농도 변환

  Serial.print("dust=");
  Serial.println(dustDensity);

  delay(1000);
}
