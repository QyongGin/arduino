#define TRIG 13 // TRIG 핀
#define ECHO 12



void setup()
{
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  
 long duration,distance;
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  duration = pulseIn(ECHO, HIGH);
  
  distance = duration / 58.2;
  Serial.println(duration);
  Serial.print("\nDIstance : ");
  Serial.print(distance);
  Serial.println(" Cm");
  
  delay(1000);
            
  if(distance >= 100){
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
  }
   else
   {
     digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
   }
    
  
}