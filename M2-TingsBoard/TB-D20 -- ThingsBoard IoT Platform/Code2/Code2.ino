#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#include <WiFi.h>
#include <ThingsBoard.h>
#include <Arduino.h>

#define WIFI_AP_NAME "V2036"
#define WIFI_PASSWORD "fnafchica"
#define TOKEN "wcbt10HzC54MQwRO6DeA"
#define THINGSBOARD_SERVER "demo.thingsboard.io"

#define DHT22_Pin 15
#include "DHTesp.h"
DHTesp dht;
#define pinLEDBlink 2

WiFiClient espClient;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;
uint8_t leds_PinControl[] = {18, 19, 22, 23};

int leds_Status[] = { 0, 0, 0, 0 };
char StringEcho[] = "stsLED_1";
int loopDelay = 20; // Main loop delay(ms)
int sendDataDelay = 2000; // Period of Sending Tempp/Humid.
int BlinkLEDDelay = 500; // Initial period of LED cycling.
int Count_BlinkLEDDelay = 0; // Time Counter Blink peroid
int Count_sendDataDelay = 0; // Time Counter Sending Tempp/Humid
bool Subscribed_Status = false; // Subscribed_Status for the RPC messages.
int ststus_BlinkLED = 0; // LED number that is currenlty ON.
#include "_ThingBoardRPC.h"
#include "_ConnectWifi.h"

//=====================================================
void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  WiFi_Initial();
  dht.setup(DHT22_Pin, DHTesp::DHT22); // Connect DHT sensor to GPIO 15
  // Pinconfig
  pinMode(pinLEDBlink, OUTPUT);
  for (size_t i = 0; i < COUNT_OF(leds_PinControl); ++i) {
    pinMode(leds_PinControl[i], OUTPUT);
  }
}
//=====================================================
void loop() {
  // Step0/6 - Loop Delay
  delay(loopDelay);
  Count_BlinkLEDDelay += loopDelay;
  Count_sendDataDelay += loopDelay;
  // Step1/6 - Check if next LED Blink
  if (Count_BlinkLEDDelay > BlinkLEDDelay) {
    digitalWrite(pinLEDBlink, ststus_BlinkLED);
    ststus_BlinkLED = 1 - ststus_BlinkLED;
  Count_BlinkLEDDelay = 0;
}
// Step 2/6 - Reconnect to WiFi, if needed
if (WiFi.status() != WL_CONNECTED) {
    reconnect();
    return;
  }
  // Step 3/6 - Reconnect to ThingsBoard, if needed
  if (!tb.connected()) {
    Subscribed_Status = false;
    // Connect to the ThingsBoard
    Serial.print("Connecting to: "); Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token "); Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  // Step 4/6 - Subscribe for RPC, if needed
  if (!Subscribed_Status) {
    Serial.println("Subscribing for RPC...");
    // Perform a subscription. All consequent data processing will happen in
    // callbacks as denoted by callbacks[] array. Page 14 of 23
    if (!tb.RPC_Subscribe(callbacks, COUNT_OF(callbacks))) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }
    Serial.println("Subscribe done");
    Subscribed_Status = true;
  }
  // Step 5/6 - Check if it is a time to send Tempp/Humid
  if (Count_sendDataDelay > sendDataDelay) {
    Serial.print("Sending data...");
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    tb.sendTelemetryFloat("temperature", temperature);
    tb.sendTelemetryFloat("humidity", humidity);
    Serial.print("T=" + String(temperature, 2) + ", ");
    Serial.print("H=" + String(humidity, 2) + ", ");
    Serial.print("LED=");
    for (size_t i = 0; i < COUNT_OF(leds_PinControl); ++i) {
      StringEcho[7] = 0x30 + i; // Set 0 to "0"
      tb.sendTelemetryInt(StringEcho, leds_Status[i]);
      Serial.print(leds_Status[i]);
    }
    Serial.println();
    Count_sendDataDelay = 0;
  }
  // Step 6/6 - Process messages
  tb.loop();
}
