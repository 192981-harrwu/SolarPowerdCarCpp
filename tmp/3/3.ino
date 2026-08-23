int maxFreq = 1000; 
int currentFreq = 1000;
bool isOn = true;  

void playSiren() {  
  for (int i = 200; i < maxFreq; i++) {     
    tone(5, i);     
    delay(1);
  }    
  for (int i = maxFreq; i > 200; i--) {     
    tone(5, i);     
    delay(1);   
  } 
}  

void task4(int del_) {
  digitalWrite(5, HIGH);
  delay(del_);
  digitalWrite(5, LOW);
  delay(del_);
}

void setup() {   
  pinMode(5, OUTPUT);   
  pinMode(2, INPUT);
  pinMode(3, INPUT);   
  pinMode(4, INPUT);   
  Serial.begin(9600); 
}  

void task5() { 
  if (digitalRead(2)) {     
    while (digitalRead(2)) delay(1);     
    currentFreq += 100;     
    Serial.println("HIGH");   
  }    

  if (digitalRead(3)) {     
    while (digitalRead(3)) delay(1);     
    currentFreq -= 100; 
  }
  if (digitalRead(4)) {     
    while (digitalRead(4)) delay(1);     
    isOn = !isOn;
  }    

  if (isOn) {     
    tone(5, currentFreq, 0);
    delay(0);
  } else {     
    playSiren();
    if (digitalRead(4)) {     
      while (digitalRead(4)) delay(1);     
      isOn = !isOn;
    }
  } 
}

void loop () {
  for (int i = 10; i > 0; i --) {
    task4(i);
  }

  for (int i = 1; i < 10; i ++) {
    task4(i);
  }
}
