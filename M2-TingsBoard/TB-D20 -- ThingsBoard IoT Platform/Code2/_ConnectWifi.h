//_ConnectWifi.h
//=====================================================
void WiFi_Initial() {
  Serial.println("Connecting to AP ..."); // attempt to connect to WiFi network
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to AP");
  Serial.print("Local IP = ");
  Serial.println(WiFi.localIP());
}
//=====================================================
void reconnect() {
  status = WiFi.status(); // Loop until we're reconnected
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConnected to AP");
    Serial.print("Local IP = ");
    Serial.println(WiFi.localIP());
  }
}
