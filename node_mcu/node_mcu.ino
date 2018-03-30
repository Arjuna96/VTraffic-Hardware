#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>

const char* ssid = "SLT-LTE-WiFi";
const char* password = "th8ne88y8qf";

//const char* ssid = "IIT Student WIFI";
//const char* password = "#1234@abcd";

ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
//
//    int numSsid = WiFi.scanNetworks();
//   for (int thisNet = 0; thisNet < numSsid; thisNet++) {
//    Serial.print(thisNet);
//    Serial.print(") ");
//    Serial.print(WiFi.SSID(thisNet));
//    Serial.print("\tSignal: ");
//    Serial.print(WiFi.RSSI(thisNet));
//    Serial.println("");
//  }

  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print('.');
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println(WiFi.localIP());
  server.on("/", handleBody);
  Serial.println("server started");
  server.begin();
}

void loop() {
  server.handleClient();
  delay(1000);
}

void handleBody() {
  Serial.println("handle body");
  if (server.hasArg("plain")== false){ //Check if body received
        Serial.println("received");
        server.send(200, "text/plain", "Body not received");
        return;

  }
  Serial.println("handle body");
  String message = "Body received:\n";
         message += server.arg("plain");
         message += "\n";

  server.send(200, "text/plain", message);
  char data = server.arg("plain").charAt(0);
  Serial.println(data);
  Serial.write(data);
}

