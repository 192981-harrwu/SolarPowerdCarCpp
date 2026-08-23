#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd_1(0x27, 16, 2);


class Weather {
private:
  int mode_{-1};
  template<typename... T>   
  const void print1(const T&... msgs) {
    lcd_1.clear();
    (lcd_1.print(msgs), ...);
  }
  
  template<typename... T>   
  const void print2(const T&... msgs) {
    lcd_1.setCursor(0, 1);
    (lcd_1.print(msgs), ...);
  }
  
  void gambelAnimation(int n, int pos) {
    for (int i = 0; i <= n; i ++) {
      lcd_1.setCursor(pos, 0);
      lcd_1.print(i);
      tone(12, i * 100 + 100);
      delay(100);
    }
    noTone(12);
  }

  void printEnemyPos(int pos) {
    if (pos == 15) {lcd_1.setCursor(0, 1); lcd_1.print(" ");}
    else {lcd_1.setCursor(pos + 1, 1); lcd_1.print(" ");}
    lcd_1.setCursor(pos, 1);
    lcd_1.print("F");
  }

  void jumpSound() {
    tone(12, 800);
    delay(100);
    tone(12, 600);
    delay(100);
    noTone(12);
  }

public:
  Weather(){}

  int getMode() const{return mode_;}
  void setMode(int n) {mode_ = n;}

  template<typename... T>   
  const void printMain(const T&... msgs) {
    lcd_1.clear();
    (lcd_1.print(msgs), ...);
  }

  void game() {
    bool isJump = false;
    int enemyPos = 15;
    print1("Dino Game");
    delay(1000);
    lcd_1.clear();
    lcd_1.setCursor(1, 1);
    lcd_1.print('e');

    while (!digitalRead(9)) {
      delay(50);
      printEnemyPos(enemyPos);
      enemyPos += (enemyPos <= 0)? 15: -1;
      lcd_1.setCursor(1, 1);
      lcd_1.print('e');
      if (digitalRead(8) && !isJump){ 
        jumpSound();
        isJump = true; 
        while(digitalRead(8)) delay(0);
        if (isJump){
          lcd_1.setCursor(1, 1);
          lcd_1.print(" ");

          lcd_1.setCursor(1, 0);
          lcd_1.print('e');

          delay(300); isJump = false;
          lcd_1.print(" ");
        }
      }
    }
  }
  
  bool init_() {
    lcd_1.init();
    lcd_1.setBacklight(1);
    Serial.println("ee");
    print1("Hello User"); delay(1000);
    print1("Press &Set");
    return true;
  }

  void clickSound() {
    tone(12, 600);
    delay(100);
    tone(12, 800);
    delay(100);
    noTone(12);
  }
  
  void lightSens() {
    print1("Light Sens Mode"); delay(3000);
    while (!digitalRead(9)) {
      int lightLvl = analogRead(A1);
      print1(lightLvl);
      delay(100);
    }
  }

  void tempSens() {
    print1("Temp Sens Mode"); delay(3000);
    while (!digitalRead(9)) {
      int lightLvl = analogRead(A2);
      print1(lightLvl);
      delay(100);
    }
  }
  
  void gamble(){
    while (!digitalRead(9)) {
      if (digitalRead(8)) {
        while (digitalRead(9)) delay(0);
        lcd_1.clear();
        
        int d = random(9),
        	e = random(9),
        	f = random(9);
        
        gambelAnimation(d, 0);
        gambelAnimation(e, 2);
        gambelAnimation(f, 4);

        if (d == e || e == f || d == f) print2("You Win Small");
        else if (d == e && e == f && d == f) print2("You Win Big");
        else print2("You lose");
      }
    } 
  }
  
};

auto main_ {new Weather()};

static uint8_t mode_ {0};

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP); 
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  randomSeed(analogRead(A4));
  
  main_->init_();
}


void loop() {
  if (digitalRead(9)) {
    lcd_1.clear();
    while (digitalRead(9)) delay(0);

    main_->clickSound();
    int pass_mode = main_->getMode();
    Serial.println(pass_mode);
    pass_mode += (pass_mode >= 4)? -4: 1;
    Serial.println(pass_mode);
    main_->setMode(pass_mode);
    if (pass_mode == 0) {
      lcd_1.print("Gamble");
      main_->gamble();
    }else if (pass_mode == 1){
      main_->game();
    }else if (pass_mode == 2){
      main_->lightSens();
    }else if (pass_mode == 3){
      main_->tempSens();
    }else{
      lcd_1.print(pass_mode + 1);
      main_->gamble();
    }
  }
}
