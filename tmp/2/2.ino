#define MAX_FREQ 100

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(1, INPUT);
  pinMode(12, OUTPUT);
  pinMode(3, INPUT);
}


bool a = false;
bool b = false;

void task1(int port, int out){
  if (digitalRead(port)){
    while(digitalRead(port)){
      delay(0);
    }
    a = !a;
  }

  if (!a){
  	digitalWrite(out, LOW);
  }
  else{
  	digitalWrite(out, HIGH);
  }
}

void task3(int port, int out){
  if (digitalRead(port)){
    while(digitalRead(port)){
      delay(0);
    }
    b = !b;
  }

  if (!b){
  	digitalWrite(out, LOW);
  }
  else{
  	digitalWrite(out, HIGH);
  }
}


void task2() {
  const bool statesA[] = {false, true, false, true};
  const bool statesB[] = {false, false, true, true};

  for (int i = 0; i < 4; i++) {
    a = statesA[i];
    b = statesB[i];

    digitalWrite(13, a);
    digitalWrite(12, b);
    digitalWrite(11, a & b); // Pin 11 tracks the OR logic

    delay(1000);
  }
}


void loop(){
  task1(2, 13);
  task3(3, 12);
  digitalWrite(11, !(a & b));
  //task2();
}