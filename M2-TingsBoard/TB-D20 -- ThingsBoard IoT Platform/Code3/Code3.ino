#include <WiFi.h>
#include <ArduinoJson.h> // by Benoit Blanchon >> Ver 5.8.0
#include <PubSubClient.h> // by Nick O’Leary. >> Ver 2.6 and Update PubSubClient.h

#define WIFI_AP_NAME "V2036"
#define WIFI_PASSWORD "fnafchica"
#define Device_Name "Bearish"
#define Device_Token "wcbt10HzC54MQwRO6DeA"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define GPIO1_ESP32Pin 18
#define GPIO2_ESP32Pin 19
#define GPIO3_ESP32Pin 22
#define GPIO4_ESP32Pin 23

#define Rand "random"
boolean gpioState[] = {false, false, false, false};
int status = WL_IDLE_STATUS;

int Stepupdate;
int Random;
WiFiClient wifiClient;
PubSubClient client(wifiClient);
#include "_HandOnMQTT.h"
#include "_WifiConnect.h"
void setup() {
  Serial.begin(115200);
  // Set output mode for all GPIO pins
  pinMode(GPIO1_ESP32Pin, OUTPUT);
  pinMode(GPIO2_ESP32Pin, OUTPUT);
  pinMode(GPIO3_ESP32Pin , OUTPUT);
  pinMode(GPIO4_ESP32Pin , OUTPUT);
  delay(10);
  InitialWiFi();
  client.setServer( THINGSBOARD_SERVER, 1883 );
  client.setCallback(on_message);
}
void loop() {
  delay(20);
  Stepupdate += 20;
  if (Stepupdate > 5000) {
    Random = random(00 , 50);
    client.publish("v1/devices/me/telemetry", get_gpio_status().c_str());
    Stepupdate = 0;
  }
  if ( !client.connected() ) {
    reconnect();
  }
  client.loop();
}
