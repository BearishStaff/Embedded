#include <WiFi.h>
const char* ssid = "V2036";
const char* password = "fnafchica";

int pinTest1 = 18;
int pinTest2 = 19;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(pinTest1, OUTPUT); // set the LED pin mode
  pinMode(pinTest2, OUTPUT); // set the LED pin mode
  delay(10);
  Serial.print("\n\nConnecting to "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected."); Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); server.begin();
}

int value = 0;
bool LED_Status1 = LOW;
bool LED_Status2 = LOW;

void loop() {
  digitalWrite(pinTest1, LED_Status1);
  digitalWrite(pinTest2, LED_Status2);
  WiFiClient client = server.available(); // listen for incoming clients
  if (client) { // if you get a client,
    Serial.println("New Client."); // print a message out the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        if (c == '\n') { // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html>");
            client.println("<body>");
            client.println("<h1>LED Status</h1>");
            client.println("<p>");
            if (LED_Status1 == HIGH)
              client.println("LED1-On");
            else
              client.println("LED1-Off");
            
            if (LED_Status2 == HIGH)
              client.println(",LED2-On");
            else
              client.println(",LED2-Off");
             
            client.println("<p>");
            client.println("<a href=\"/led1on\"><button style = \"background-color: #f44336;\">LED1 On</button></a>");
            client.println("<a href=\"/led2on\"><button style = \"background-color: #f44336;\">LED2 On</button></a>");
            client.println("</p>");
            client.println("<a href=\"/led1off\"><button style = \"background-color: #008CBA;\">LED1 Off</button></a>");
            client.println("<a href=\"/led2off\"><button style = \"background-color: #008CBA;\">LED2 Off</button></a>");
            client.println("<body>");
            client.println("<html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /led1on")) LED_Status1 = HIGH;
        if (currentLine.endsWith("GET /led1off")) LED_Status1 = LOW;
        if (currentLine.endsWith("GET /led2on")) LED_Status2 = HIGH;
        if (currentLine.endsWith("GET /led2off")) LED_Status2 = LOW;
        
      }
    }
    client.stop(); // close the connection:
    Serial.println("Client Disconnected.");
  }
}
