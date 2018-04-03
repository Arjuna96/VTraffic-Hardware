#include "ESP8266WiFi.h"

const char* ssid = "IIT Student WIFI";
const char* password = "#1234@abcd";

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

void loop() {
  if (Serial.available () > 0 ) {  // checking any request code from colorlight controller
    int reqCode = Serial.read();
    
    if (reqCode == 100) {
      get_time_delay ();  // Requesting color light time
    } else if (reqCode == 200) {
      update_current_state ();   // Requesting to save current color light state
    } else if (reqCode == 300) {
      request_pedestrian_crossing (); // Requesting to pedestrian crossing
    }
}

void get_time_delay () {
  http.post('https://vtraffic.herokuapp.com/getTime/',
        'Content-Type: application/json\r\n',
        '{"locationId":"1",
        "stateId":"5"}', 
        function(reqStat, data) {
          if (reqStat < 0) then
            Serial.println("failed")
            Serial.write (0);
          else
            Serial.println("Success")
            Serial.write (data.time);
          end 
        }       
  end
}
  
void update_current_state () {
 http.post('https://vtraffic.herokuapp.com/state/',
        'Content-Type: application/json\r\n',
        '{"locationId":"1",
        "currentState":"2"}', 
        function(reqStat, data) {
          if (reqStat < 0) then
            Serial.println("failed - saveState")
          else
            Serial.println("Success - saveState")
          end 
        }       
  end
}

void request_pedestrian_crossing () {
  http.post('https://vtraffic.herokuapp.com/pedestrian/',
          'Content-Type: application/json\r\n',
          '{"locationId":"1",
          "crossintId":"1"}', 
          function(reqStat, data) {
            if (reqStat < 0) then
              Serial.println("failed - pedestrian")
            else
              Serial.println("Success - pedestrian")
            end 
          }       
    end
}


