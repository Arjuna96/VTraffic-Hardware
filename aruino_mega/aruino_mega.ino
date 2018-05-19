#include <TimerOne.h>
#include <TM1637Display.h>

#define CLK 50
#define DIO 51

TM1637Display display(CLK, DIO);

int lights[8][3] = {{2, 3, 4}, {5, 6, 7}, {8, 9, 10}, {11, 12, 13}, {14, 15, 16}, {17, 18, 19}, {20, 21, 22}, {23, 24, 25}};
int lastOn = 0;
int lightId = 1;
long defaultDelay = 10000;
long stateDelay = 10000;
int currentState = 0;
int previosState = 0;
int x = 0;
int timeRequestCOde = 100;
int saveStateRqstCode = 200;;
int count = 5;
int state[4][3] = {{1,2,8}, {3,4,2}, {5,6,4}, {7,8,6}};

void setup() {
  Timer1.initialize(1000000);
  display.setBrightness(0x0f);
  display.showNumberDec(50, true, 4, 0);
  Serial.begin(115200);

  for (int id = 2; id < 26; id++) {
    pinMode(id, OUTPUT);
  }

  delay(1000);

  for (int stateId = 0; stateId < 4; stateId++) {
    for (int id = 0; id < 3; id++) {
      int prelightId = state[stateId][id];
      red(prelightId);
    }
  }
}

void loop() {
  currentState = x % 4;
  serialWrite(timeRequestCOde + lightId*10 + currentState);
  Serial.println("rqst");
  delay(2000);

  Timer1.detachInterrupt();
  
  get_delay_time();

// Previos lights to red 
  for (int id = 0; id < 3; id++) {
    int prelightId = state[previosState][id];
    red(prelightId);
  }

// Current lights to green 
  for (int id = 0; id < 3; id++) {
    int currlightId = state[currentState][id];
    green(currlightId);
  }

  serialWrite(saveStateRqstCode + lightId*10 + currentState);
  x = currentState + 1;
  previosState = currentState;
  delay(1000);
  Timer1.attachInterrupt(timer);

  delay(stateDelay - 2000);
}

int get_delay_time () {
  long time = 0;
  String str = "";
  char incomingByte;

  while (Serial.available() > 1) {
      while(true) {
        incomingByte = Serial.read();
        if (incomingByte == '\n') break;
        if (incomingByte == -1) continue;
        str += incomingByte;
        
        count = str.toInt();
        time = str.toInt() * 1000;
      }
  }
  
  if (time == 0 || time < 0) {
    time = defaultDelay;
    count = 10;
  }

  stateDelay = time;
}

void light_control(int id) {
  if (lastOn == 0) {
    Serial.println("if light control");
    reset(id);
    green(id);
    lastOn = id;
  }
  else {
    Serial.println("else light control");
    reset(id);
    red(lastOn);
    green(id);
    lastOn = id;
  }
}

void green(int light) {
  int lgt = light - 1;
  digitalWrite(lights[lgt][0], 0);
  digitalWrite(lights[lgt][1], 1);
  delay(500);
  digitalWrite(lights[lgt][1], 0);
  digitalWrite(lights[lgt][2], 1);
}

void red(int light) {
  int lgt = light - 1;
  digitalWrite(lights[lgt][2], 0);
  digitalWrite(lights[lgt][1], 1);
  delay(500);
  digitalWrite(lights[lgt][1], 0);
  digitalWrite(lights[lgt][0], 1);
}

void reset(int light) {
  int lgt = light - 1;
  Serial.print(lgt);
  digitalWrite(lights[lgt][3], 0);
  digitalWrite(lights[lgt][2], 0);
  Serial.print(lgt);
  digitalWrite(lights[lgt][1], 0);
}


void serialWrite (int value) {
  String str = String(value);
  Serial.println(value);
}

void timer () {
  display.showNumberDec(count, true, 4, 0);
  if (count != 0) {
    count = count - 1;
  }
  
}
