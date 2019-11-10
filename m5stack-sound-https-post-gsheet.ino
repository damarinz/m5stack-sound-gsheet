#include <WiFi.h>
#include <M5Stack.h>
#include <HTTPClient.h>



#define M5STACKFIRE_MICROPHONE_PIN 34

const char* ssid     = "SSID-11g";   // your network SSID (name of wifi network)
const char* password = "xxxxxxxxx";    // your network password

//setup start
void setup() {
  M5.begin();
  dacWrite(25, 0); // stop speaker for decrease noize
  Wire.begin();
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);


  Serial.print("Attempting to connect to SSID: ");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

}


void loop() {
  // put your main code here, to run repeatedly:
  int micValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);

  Serial.println(micValue);

//generate JSON
  char json[100];
  sprintf(json, "{\"sound\": %d }", micValue);

//HTTPClient code start
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  http.begin("https://script.google.com/macros/s/AKfycbz6TFqd3plX_vrxqY4fXxCADSPKJuSpx3Fn1DBRKoijMvEtYxM/exec"); //HTTP

  Serial.print("[HTTP] POST...\n");
  // start connection and send HTTP header
  int httpCode = http.POST(json);

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();


  delay(10000);

}