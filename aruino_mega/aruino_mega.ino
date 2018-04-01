int lights[8][3] = {{2,3,4},{5,6,7},{8,9,10},{11,12,13},{14,15,16},{17,18,19},{20,21,22},{23,24,25}};
int lastOn = 0;
int lightId = 1;
int defaultDelay = 5000;
int stateDelay = 5000;
int currentState = 1;
int previosState = 1;
int x = 1;

int state[5][3] = {{0,0,0}, {1,2,8}, {3,4,2}, {5,6,4}, {7,8,5}};

void setup() {
  Serial.begin(115200);

  for (int id = 2; id < 26; id++) {
    pinMode(id, OUTPUT);
  }

  delay(1000);

  for (int stateId = 1; stateId < 5; stateId++) {
    for (int id = 0; id < 3; id++) {
       int prelightId = state[stateId][id];
       red(prelightId);
    }
  }
}

void loop() {
 currentState = x % 5;
 stateDelay = get_delay_time();

 for (int id = 0; id < 3; id++) {
   int prelightId = state[previosState][id];
   red(prelightId);
 }

 delay(2000);

 for (int id = 0; id < 3; id++) {
   int currlightId = state[currentState][id];
   green(currlightId);
 }

 Serial.write(300);
 x = currentState + 1;
 previosState=currentState;
 delay(stateDelay);
}

int get_delay_time () {
  int time = defaultDelay;
  Serial.write(100);

  while (Serial.available() > 0) {
    time = Serial.read();

    if (time == 0) {
      time = defaultDelay;
    }
  }

  return time;
}

void light_control(int id) {
  if (lastOn == 0) {
    Serial.println("if light control");
    reset(id);
    green(id);
    lastOn=id;
  }
  else{
    Serial.println("else light control");
    reset(id);
    red(lastOn);
    green(id);
    lastOn=id;
  }  
}

void green(int light) {
  int lgt = light-1;
  digitalWrite(lights[lgt][0], 0);
  digitalWrite(lights[lgt][1], 1);
  delay(500);
  digitalWrite(lights[lgt][1], 0);
  digitalWrite(lights[lgt][2], 1);
}

void red(int light) {
  int lgt = light-1;
  digitalWrite(lights[lgt][2], 0);
  digitalWrite(lights[lgt][1], 1);
  delay(500);
  digitalWrite(lights[lgt][1], 0);
  digitalWrite(lights[lgt][0], 1);
}

void reset(int light){
  int lgt = light-1;
  Serial.print(lgt);
  digitalWrite(lights[lgt][3], 0);
  digitalWrite(lights[lgt][2], 0);
  Serial.print(lgt);
  digitalWrite(lights[lgt][1], 0);
  }
