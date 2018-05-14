int lights[3][3] = {{2, 3, 4}, {5, 6, 7}, {8, 9, 10}};
int lastOn = 0;
int lightId = 1;
int defaultDelay = 5000;
long stateDelay = 10000;
int currentState = 1;
int previosState = 1;
int x = 0;
int timeRequestCOde = 100;
int saveStateRqstCode = 200;;

int state[4][1] = {{1}, {2}, {3}};

void setup() {
  Serial.begin(115200);

  for (int id = 2; id < 11; id++) {
    pinMode(id, OUTPUT);
  }

  delay(1000);

  for (int stateId = 0; stateId < 3; stateId++) {
    for (int id = 0; id < 1; id++) {
      int prelightId = state[stateId][id];
      red(prelightId);
    }
  }
}

void loop() {
  currentState = x % 3;
  serialWrite(timeRequestCOde + lightId*10 + currentState);
  Serial.println("rqst");
  delay(5000);
  get_delay_time();

// Previos lights to red 
  for (int id = 0; id < 1; id++) {
    int prelightId = state[previosState][id];
    red(prelightId);
  }

  delay(1000);

// Current lights to green 
  for (int id = 0; id < 1; id++) {
    int currlightId = state[currentState][id];
    green(currlightId);
  }

  serialWrite(saveStateRqstCode + lightId*10 + currentState);
  x = currentState + 1;
  previosState = currentState;
  delay(stateDelay);
  Serial.println("after");
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
        time = str.toInt() * 1000;
      }
  }

  if (time == 0 || time < 0) {
    time = defaultDelay;
  }

  stateDelay = time;
  Serial.println(stateDelay);
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
//  char arry[sizeof(str)];
//  str.toCharArray(arry, sizeof(arry));
  Serial.println(value);
}
