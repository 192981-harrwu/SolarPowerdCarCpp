#include <LiquidCrystal.h>
#include <Arduino.h>

int count_ {0};
bool newData {false};

LiquidCrystal lcd_1(12, 11, 7, 6, 5, 4);

class Weather {
private:
  int mode_{0};
public:
  bool initWeather() {
    lcd_1.begin(16, 2);
    lcd_1.setCursor(0, 0);
    lcd_1.print("Hello User");
    delay(1000);
    lcd_1.clear();
    
    lcd_1.print("Enter Name Mx8Ch");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Thru Serial:");
    
    char buff[8]{};
    recvBuff(buff, 8);
    Serial.println("Enter user: ");
    lcd_1.clear();
    lcd_1.print(buff);
    
    return true;
  }
  
  void recvBuff(char* buffer, size_t size) {
    if (size == 0) return;
    size_t i {0};
    
    while (i < size - 1) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') break;
        buffer[i] = c;
        i++;
      }
    }
    buffer[i] = '\0';
  }
};

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT); 
  pinMode(13, OUTPUT);
  Weather main{};
  main.initWeather();
}
/*
void loop () {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
}
*/
void loop() {
  if (digitalRead(3)) {
    while (digitalRead(3)) delay(1);
    count_ += (count_ == 2) ? -2 : 1;
    lcd_1.clear();
    
    if (count_ == 0) { 
      lcd_1.print("mode 1");
    } else if (count_ == 1) { 
      lcd_1.print("mode 2");
    } else { 
      lcd_1.print("mode 3"); 
    }
  }
}
