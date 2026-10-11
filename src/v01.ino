void setup(){
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
}
void stop() {
  Serial.write("stop\n");
  digitalWrite(5, LOW); digitalWrite(6, LOW);
}

void left() {
  Serial.write("left\n");
  digitalWrite(5, LOW); digitalWrite(6, HIGH);
}

void front() {
  Serial.write("front\n");
  digitalWrite(5, HIGH); digitalWrite(6, HIGH);
}

void right() {
  Serial.write("right\n");
  digitalWrite(5, HIGH); digitalWrite(6, LOW);
}

bool isOn = true;

void loop() {
  front();
  delay(1000);
  left();
  delay(250);
}
