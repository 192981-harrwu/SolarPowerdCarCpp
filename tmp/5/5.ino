#include <RTClib.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd_1(0x27, 16, 2); // class for the lcd screen
Adafruit_BMP280 bmp; // class for the BMP280 that sens the pressure as well as altitude
RTC_DS3231 rtc; // For the clock module

const float R1 = 4700.0; // Resistance of resistor in the Voltage Dividor circut
const float nominalResistance = 10000.0; // Thermistor resistance at 25°C
const float nominalTemperature = 25.0; // Reference temperature in °C
const float bCoefficient = 3950.0; // Beta coefficient of your thermistor



class Weather {
private:
  int mode_{-1}; // Int value to store the current mode
  bool rtcInit = true,  lcdInit = true, bmpInit = true;
  template<typename... T>   
  const void print1(const T&... msgs) { // A Function for printing on lcd line 1
    lcd_1.clear();
    (lcd_1.print(msgs), ...);
  }
  
  template<typename... T>   
  const void print2(const T&... msgs) { // A Function for printing on lcd line 2
    lcd_1.setCursor(0, 1);
    (lcd_1.print(msgs), ...);
  }
  
  void gameAnimation(int n, int pos) { // the gaming animation. which takes in the collum and the value. It loops from 0 until the number while playing a sound on the screen and stops at the final number
    for (int i = 0; i <= n; i ++) {
      lcd_1.setCursor(pos, 0);
      lcd_1.print(i);
      tone(12, i * 100 + 100);
      delay(300);
    } 
    noTone(12); // Stop Speaker
  }

public:
  Weather(){}

  int getMode() const{return mode_;} // A getter function to return Mode
  void setMode(int n) {mode_ = n;} // A setter function to set mode
  void setBmpFalse() {bmpInit = false;} // Setter false for bmp Init
  void setLcdFalse() {bmpInit = false;} // Setter false for lcd Init
  void setRtcFalse() {bmpInit = false;} // Setter false for rtc Init

  template<typename... T>    // A print function as public member for a direct member access so instead of just writing the pure print function. We call the printing wrapper
  const void printMain(const T&... msgs) {
    lcd_1.clear();
    (lcd_1.print(msgs), ...);
  }


  
  bool init_() { // Initialize the station. By printing greetings and instruction to user..
    lcd_1.init();

    if (!bmp.begin(0x76)) { // Initialize the BMP280 chip that can used to sens temperature, preassure and rough altitude
      Serial.println("Error Init bmp\n"); // Print to serial monitor for debuging
      setBmpFalse(); // Set bmpInit value to false
    } 

    if (!rtc.begin()) { // Initialize the rtc if it dosent start we will just print and set the boolean flag of rtc to false
      Serial.println("Error Init rtc\n");
      setRtcFalse(); // Set rtcInit value to false
    }
    Serial.print("e");
    lcd_1.setBacklight(1);
    print1("Hello User"); delay(1000);
    print1("Press &Set");
    return true;
  }

  void clickSound() { // A sound that is called when pressed pin number 9
    tone(12, 600);
    delay(100);
    tone(12, 800);
    delay(100);
    noTone(12);
  }

  void timeMode() {
    while (!digitalRead(9)) {
      DateTime now = rtc.now(); // Gets time using 
      String minuteStr = (now.minute() < 10) ? "0" + String(now.minute()): String(now.minute()); // This line is for when the minute is a single digit ill add a 0 so that the time looks even
      String hourStr = (now.hour() < 10)? "0" + String(now.hour()): String(now.hour()); // Applys to the same for the hour
      print1(hourStr, ':', minuteStr); // Print hour and minute str on line 1
      print2(now.month(), "/", now.day()); // Print Month and Day on Line 2
      delay(500); //delay 500 so that the screen wont be flucating constantly
    }
  }

  void pressureSens() { // mode 3 for preassure sensing
    print1("Pressure Sens"); delay(1000);
    while (!digitalRead(9)) {
      print1(bmp.readPressure());
      print2("Pa");
      delay(500);
    }
  }

  void altSens() { // mode 4 for altitude sensing 
    print1("Altitude Sens"); delay(1000);
    while (!digitalRead(9)) {
      print1(bmp.readAltitude()); // Find altitude and print
      print2("meters"); // Print the word meters
      delay(500); 
    }
  }
  
  void lightSens() { //  mode 2 to measure light lvl
    print1("Light Sens Mode"); delay(2000); // Print on line 1 the words light sensing mode
    while (!digitalRead(9)) {
      int lightLvl = analogRead(A1) - 400; // We do this because when we block the light it isnt always 0 % but rather at around 20% - 30% we fix this by minus by 400

      float percentage = (lightLvl / 500.0) * 100.0; // Find percentage 
      if (percentage > 100.0) percentage = 100.0; // If percentage > 100 we just set it to 1000
      print1("Lvl: ", percentage , '%'); // Print to lcd1 the percentage
      delay(500); // Delay so the screen dosent lag out
    }
  }

  void tempSens() {  // mode 3 to  measure temp lvl
    print1("Temp Sens Mode"); delay(2000);
    while (!digitalRead(9)) {
      int analogVal = analogRead(A2);
      float voltage = analogVal * (5.0/1023.0);
      float resistance = R1 * (5.0 /voltage - 1.0);
      float temp = 1.0 / (log(resistance / nominalResistance) / bCoefficient + 1.0 / (nominalTemperature + 273.15)); 
      temp -= 273.15;

      print1("Temp: ", temp, "`C");
      delay(500);
    }
  }
  
  void game(){ // mode 1 main loop for the gaming. It stops until pin 9 is pressed
    while (!digitalRead(9)) {
      if (digitalRead(8)) {
        while (digitalRead(9)) delay(0);
        lcd_1.clear(); // Clear screen
        
        int d = random(9); // generate random num from 0 - 9
        int e = random(9); // generate random num from 0 - 9
        int f = random(9); // generate random num from 0 - 9
        
        gameAnimation(d, 0); // call the animation functioin set collum to 0
        gameAnimation(e, 2); // call the animation functioin set collum to 2
        gameAnimation(f, 4); // call the animation functioin set collum to 4

        if (d == e || e == f || d == f) print2("You Win Small"); // if two numberrs are the same win small
        else if (d == e && e == f && d == f) print2("You Win Big"); // if three numbers the  same win big
        else print2("You lose"); // no similar number loose
      }
    } 
  }
  
};

auto main_ {new Weather()}; // initializ a Weather()  pointer using key word new.

static uint8_t mode_ {0}; // A static 8bit variable that remains for the entire program. Holds the mode for the main loop

void setup() { 
  Serial.begin(9600); // start serial
  pinMode(3, INPUT_PULLUP); 
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  randomSeed(analogRead(A4)); // generate seed by reading static siganl from analog port 4
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, // for power mode automaticly sleeps and wakes up to meassure the values
                  Adafruit_BMP280::SAMPLING_X2, // Takes two samples and find avg before returning to the arduino for a better(Temperature values)
                  Adafruit_BMP280::SAMPLING_X16, // Takes 16 samples for pressure to find the avg before returning to user
                  Adafruit_BMP280::FILTER_X16, // Takes 16 samples and avg out for the IRR filter (To find wind etc???)
                  Adafruit_BMP280::STANDBY_MS_500); // Defines the sleep duration to 500ms after this set of time we restart the measuring cycle again
  main_->init_(); // Initialize station by calling init (Iinitialize the Bmp280 lcd screen as well as the RTC module)
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Because I dont have a battery for the RTC it will get the time from compile time 
}


void loop() { // main loop
  if (digitalRead(9)) { // If digital pin 9 is pressed it gets mode from Weather and increases by 1 and sets the mode to the current one. We chec what number is mode  annnd we match it to the mode we want 
    lcd_1.clear(); // Clears the lcd screen so we dont get overlapping text
    while (digitalRead(9)) delay(0); // for an input pullup

    main_->clickSound(); // plays clicksound as button is clicked
    int pass_mode = main_->getMode(); // seting pass_mode using the getter function
    Serial.println(pass_mode); // Print mode before adding for debuging
    pass_mode += (pass_mode >= 5)? -5: 1; // if mode is bigger or equel to 5 it is minused 5 to be the original 0 mode else we just add 1
    Serial.println(pass_mode); // Print mode to serial monitor for debuging
    main_->setMode(pass_mode); // we set the mode using the setter function we have in the weather class as a member
    if (pass_mode == 0) { // Mode 0 
      main_->timeMode(); // start time
    }else if (pass_mode == 1){ // Mode 1
      main_->lightSens(); // start light sens mode stops until we press button digitalPin 9
    }else if (pass_mode == 2){ // Mode 2
      main_->tempSens();  // start temp sens mode stops until we press button  digitalPin 9
    }else if (pass_mode == 3){ // Mode 3
      main_->pressureSens(); // start pressureSens mode stops until we preess button digitalPin 9
    }else if (pass_mode == 4){ // Mode 4
      main_->altSens(); // start altitude Sensing mode stops until we preess butotn digitalPin 9
    }else if (pass_mode == 5){
      lcd_1.print("Game"); // Print game
      main_->game(); // Start game
    }else{
      lcd_1.print(pass_mode + 1); // This acts as a fail safe just in case the  mode is set to -1 or etc this  linie prints the current mode for debuging
      main_->game(); // We just set the mode to game mode
    }
  }
}
