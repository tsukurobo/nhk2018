const int PULSE_PIN = 2;
const int CW_PIN = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(CW_PIN, OUTPUT);
  delay(3000);
}

long delay_span = 1500;
long count = 0;
long M_COUNT = (300*100/175); //100mm
void loop() {
  // put your main code here, to run repeatedly:
  for (count=0; count<M_COUNT; count++){
      delayMicroseconds(delay_span);
      digitalWrite(PULSE_PIN, HIGH);
      delayMicroseconds(delay_span);
      digitalWrite(PULSE_PIN, LOW);
    }
    delay(1000);
    digitalWrite(CW_PIN, HIGH);
  for (count=0; count<M_COUNT; count++){
      delayMicroseconds(delay_span);
      digitalWrite(PULSE_PIN, HIGH);
      delayMicroseconds(delay_span);
      digitalWrite(PULSE_PIN, LOW);
    }
    digitalWrite(CW_PIN, LOW);    
    delay(1000);
}

