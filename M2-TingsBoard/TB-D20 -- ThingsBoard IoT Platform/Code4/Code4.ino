#include "ThingsBoard.h"
#include <WiFi.h>
#define WIFI_AP "V2036"
#define WIFI_PASSWORD "fnafchica"
#define TOKEN "wcbt10HzC54MQwRO6DeA"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#include <Arduino.h>
#include "ArduinoJson.h"

#define SERIAL_DEBUG_BAUD 115200
WiFiClient espClient;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }
  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: "); Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token "); Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect"); return;
    }
  }
  Serial.print("Sending data...");
  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  float xTempp = random(20, 40);
  float xHdmid = random(60, 80);
  Serial.print(xTempp, 2);
  Serial.print(","); Serial.print(xHdmid, 2); Serial.println();
  tb.sendTelemetryFloat("temperature", xTempp);
  tb.sendTelemetryFloat("humidity", xHdmid);
  tb.loop(); delay(5000);
}

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}
void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
