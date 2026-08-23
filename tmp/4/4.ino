#include <LiquidCrystal_I2C.h>
#include <Wire.h>
static int count_{0};

LiquidCrystal_I2C lcd_1(0x27, 16, 2);

byte singlePixel[8] = {
  0b00000, // Row 0
  0b00100, // Row 1
  0b00100, // Row 2
  0b00100,
  0b00100,
  0b11011,
  0b11011,
  0b00000  
};

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);

  lcd_1.init();
  lcd_1.backlight();

  lcd_1.createChar(0, singlePixel);
}

void inline dick(const int val) {
  for (int i = 0; i < val; i ++) {lcd_1.setCursor(i, 0); lcd_1.write(byte(0)); delay(10);}
  if (val > 16 && val <= 32) {
    for (int i = 0; i < val - 16; i ++) {lcd_1.setCursor(i, 1); lcd_1.write(byte(0)); delay(10);}
  }
}

template <typename T>
void inline printS(const T& msg) {
  lcd_1.clear();
  lcd_1.print(msg);
}

bool isReverse{false};

void loop() {
  if (digitalRead(3)) {
    while (digitalRead(3)) delay(10);
    Serial.println(count_);
    if (count_ <= 32 && !isReverse) {count_++; if (count_ >= 32) isReverse = true;}
    else {count_ --; if (count_ <= 0) isReverse = false;}
    count_ += (count_ < 33)? 1: -1;
    Serial.println(count_);
    dick(count_);
  }
}