#include "ESP8266WiFi.h";
#include "ESP8266HTTPClient.h";

//const char* ssid = "IIT Student WIFI";
//const char* password = "#1234@abcd";

const char* ssid = "SLT-LTE-WiFi";
const char* password = "th8ne88y8qf";

HTTPClient http;

void setup(void) {
  Serial.begin(115200); // Starting serial communication
  
  // Printng available wifi networks
   int numSsid = WiFi.scanNetworks();
   for (int wifiId = 0; wifiId < numSsid; wifiId++) {
    Serial.print(wifiId);
    Serial.print("----");
    Serial.print(WiFi.SSID(wifiId));
    Serial.println("");
  }

  WiFi.begin(ssid, password); // Connecting to wifi

  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print('.');
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println(WiFi.localIP());
}

unsigned int integerValue = 0;
char incomingByte;
int trafficLightId;
int stateId;

void loop() {
  if (Serial.available () > 2) {
      integerValue = 0;
      String str = "";

      for (int x = 0; x < 3; x++) {
        incomingByte = Serial.read();
        if (incomingByte == '\n') break;
        if (incomingByte == -1) continue;
        str += incomingByte;
        integerValue = str.toInt();
//        Serial.println(incomingByte);
      }
//Serial.println(str);
      while(Serial.available()) {
        Serial.read();
      }
      Serial.println("afterflush " + Serial.read());
      int reqCode;
      
      if (integerValue > 100) {
        reqCode = get_req_code ();
        set_req_params(reqCode);
      }
      
      if (reqCode == 100) {
        get_time_delay();                     // Requesting color light time
      } else if (reqCode == 200) {
        update_current_state();               // Requesting to save current color light state
      } else if (reqCode == 111) {
        test();                               // Test function
      }
  }
}



// return request code to identyfy the relevent request
int get_req_code () {
  if (integerValue < 200) {
    return 100;
  } else if (integerValue < 300) {
    return 200;
  }
}




// set current traffic light state and id globly 
void set_req_params (int reqCode) {
  int remain = integerValue % reqCode;
  String temp = String(remain);
  trafficLightId = String(temp.charAt(0)).toInt();
  stateId = String(temp.charAt(1)).toInt();
}




// get the delay time for next traffic state from server 
void get_time_delay () {
  http.begin("http://18.191.39.15:2000/api/requestTime");
  http.addHeader("Content-Type", "application/json; charset=utf-8");

  String stringOne = "{\"trafficLightId\":\"";
  String stringTwo = "\",\"stateId\":\"";
  String stringThree = "\"}";
  String paramString = stringOne + trafficLightId + stringTwo + stateId + stringThree;
  
  int httpCode = http.POST(paramString);

  if(httpCode == HTTP_CODE_OK) {
//      Serial.print("HTTP response code ");
//      Serial.println(httpCode);
      String response = http.getString();
//      Serial.write(response.toInt());
      serialWrite(response.toInt());
  } else {
    Serial.println("Error in HTTP request");
  }

  http.end();
}

char* string2char(String phrase){
    if(phrase.length()!=0){
        char *chrArray = const_cast<char*>(phrase.c_str());
        return chrArray;
    }
}

// check connection
void test () {
  Serial.println("getDelay");
  http.begin("http://18.191.39.15:2000/api/test");
  int httpCode = http.GET();

  if(httpCode == HTTP_CODE_OK) {
     Serial.print("HTTP response code ");
     Serial.println(httpCode);
     String response = http.getString();
     Serial.println(response);
  } else {
     Serial.println("Error in HTTP request");
  }
 
  http.end();
}




// update the traffic light state in database
void update_current_state () {
 http.begin("http://18.191.39.15:2000/api/updateState");
 http.addHeader("Content-Type", "application/json; charset=utf-8");

  String stringOne = "{\"trafficLightId\":\"";
  String stringTwo = "\",\"stateId\":\"";
  String stringThree = "\"}";
  String paramString = stringOne + trafficLightId + stringTwo + stateId + stringThree;
  
  int httpCode = http.POST(paramString);

  if(httpCode == HTTP_CODE_OK) {
//      Serial.print("HTTP response code ");
//      Serial.println(httpCode);
      String response = http.getString();
//      Serial.println(response);
  } else {
//    Serial.println("Error in HTTP request");
  }

  http.end();
}

void serialWrite (int value) {
  String str = String(value);
//  char arry[sizeof(str)];
//  str.toCharArray(arry, sizeof(arry));
  Serial.println(str);
}

