int lights[8][3] = {{2,3,4},{5,6,7},{8,9,10},{11,12,13},{14,15,16},{17,18,19},{20,21,22},{23,24,25}};
int lastOn=0;
int lightId = 1;

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  delay(2000);
}

void loop() {
  if (Serial.available() > 0) {
//    char temp[3];
//    String arrayString = Serial.readString();
//    arrayString.toCharArray(temp, 3);
//    Serial.print(temp);
    char c = Serial.read();
    Serial.println("serial");
    if (c == '1') {
      lightId = 1;
    } else if (c == '2') {
      lightId = 2;
    } else if (c == '3') {
      lightId = 3;  
    }

    light_control(lightId);
 }
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
