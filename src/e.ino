constexpr int motorPin1 = 3; 
constexpr int motorPin2 = 9; 
constexpr int motorPin1R = 5; 
constexpr int motorPin2R = 6;  

constexpr static int motorSpeed = 200;
constexpr static int turnSpeed = 180;

void setup() {   
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT);   
  pinMode(motorPin2, OUTPUT);   
  pinMode(motorPin1R, OUTPUT);   
  pinMode(motorPin2R, OUTPUT); 
}  

void left() {   
  analogWrite(motorPin1R, turnSpeed);   
  analogWrite(motorPin2, turnSpeed); 
}  

void right() {   
  analogWrite(motorPin1, turnSpeed);   
  analogWrite(motorPin2R, turnSpeed); 
}  

void stop() {   
  analogWrite(motorPin1, 0);   
  analogWrite(motorPin2, 0);   
  analogWrite(motorPin1R, 0);   
  analogWrite(motorPin2R, 0); 
}  

void reverse() {   
  analogWrite(motorPin1, motorSpeed);   
  analogWrite(motorPin2, motorSpeed); 
}  

void forward() {   
  analogWrite(motorPin1R, motorSpeed);   
  analogWrite(motorPin2R, motorSpeed); 
}  

void eloop() {
  left();
  delay(360);
  stop();
  delay(3000);
}

void loop() {
  Serial.print("Forward");
  forward();
  delay(3500);
  stop();
  delay(2000);  
  Serial.print("Reverse");
  left();
  delay(400);
  stop();
  delay(3000);
}

