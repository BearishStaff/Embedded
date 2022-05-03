// File 2 of 3
// _HandOnMQTT.h
//========================================================
//========================================================
String get_gpio_status() {
  // Prepare gpios JSON payload string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject & data = jsonBuffer.createObject();
  data[String(GPIO1_ESP32Pin)] = gpioState[0];
  data[String(GPIO2_ESP32Pin)] = gpioState[1];
  data[String(GPIO3_ESP32Pin)] = gpioState[2];
  data[String(GPIO4_ESP32Pin)] = gpioState[3];
  char payload[256];
  data.printTo(payload, sizeof(payload));
  String strPayload = String(payload);
  Serial.print("Get GPIO Status: ");
  Serial.println(strPayload);
  return strPayload;
}
//========================================================
//========================================================
void set_gpio_status(int pin, boolean enabled) {
  if (pin == GPIO1_ESP32Pin) {
    gpioState[0] = 1 - gpioState[0];
    digitalWrite(GPIO1_ESP32Pin, gpioState[0]);
  }
  if (pin == GPIO2_ESP32Pin) {
    gpioState[1] = 1 - gpioState[1];
    digitalWrite(GPIO2_ESP32Pin, gpioState[1]);
  }
  if (pin == GPIO3_ESP32Pin) {
    gpioState[2] = 1 - gpioState[2];
    digitalWrite(GPIO3_ESP32Pin, gpioState[2]);
  }
  if (pin == GPIO4_ESP32Pin) {
    gpioState[3] = 1 - gpioState[3];
    digitalWrite(GPIO4_ESP32Pin, gpioState[3]);
  }
}
//========================================================
//========================================================
// The callback for when a PUBLISH message is received from the server.
void on_message(const char* topic, byte* payload, unsigned int length) {
  Serial.println("\nOn message");
  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';
  Serial.print("Topic: "); Serial.println(topic);
  Serial.print("Message: "); Serial.println(json);
  // Decode JSON request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject((char*)json);
  if (!data.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  // Check request method
  String methodName = String((const char*)data["method"]);
  // If Reply with GPIO status
  if (methodName.equals("getGpioStatus")) {
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
  }
  // If Update GPIO status and reply
  if (methodName.equals("setGpioStatus")) {
    set_gpio_status(data["params"]["pin"], data["params"]["enabled"]);
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
  }
}
