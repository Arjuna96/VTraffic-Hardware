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

void loop() {
  if (Serial.available () > 0 ) {
      integerValue = 0;

      while(1) {
        incomingByte = Serial.read();
        if (incomingByte == '\n') break;
        if (incomingByte == -1) continue;
        integerValue *= 10;
    
        integerValue = ((incomingByte - 48) + integerValue);
      }

      Serial.println(integerValue);

      if (integerValue == 100) {
        get_time_delay();                     // Requesting color light time
      } else if (integerValue == 200) {
        update_current_state();               // Requesting to save current color light state
      } else if (integerValue == 111) {
        test();                               // Test function
      }
  }
}

void get_time_delay () {
  http.begin("http://18.221.95.10:2000/api/requestTime");
  http.addHeader("Content-Type", "application/json; charset=utf-8");
  char *payload = "{\"trafficLightId\":\"\",\"stateId\":\"1\"}";
  
  int httpCode = http.POST((uint8_t *)payload,strlen(payload));

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

void test () {
  Serial.println("getDelay");
  http.begin("http://18.221.95.10:2000/api/test");
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
  
void update_current_state () {
 http.begin("http://18.221.95.10:2000/api/updateState");
  http.addHeader("Content-Type", "application/json; charset=utf-8");
  char *payload = "{\"trafficLightId\":\"\",\"stateId\":\"1\"}";
  int httpCode = http.POST((uint8_t *)payload,strlen(payload));

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

int get_serial_value () {
  
  
}


