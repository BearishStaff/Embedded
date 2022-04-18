#include <TM1638plus.h>
#include "DHTesp.h"
DHTesp dht;

#define DHT22_Pin 15
#define Brd_STB 18 // strobe = GPIO connected to strobe line of module
#define Brd_CLK 19 // clock = GPIO connected to clock line of module
#define Brd_DIO 21 // data = GPIO connected to data line of module
bool high_freq = true; //default false,, If using a high freq CPU > ~100 MHZ set to true.
TM1638plus tm(Brd_STB, Brd_CLK , Brd_DIO, high_freq);

void setup() {
  Serial.begin(115200);
  tm.displayBegin();
  dht.setup(DHT22_Pin, DHTesp::DHT22); // Connect DHT sensor to GPIO 15
}

void loop() {

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print("Temperature: ");
  Serial.print(temperature); Serial.print(" *C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity); Serial.print(" %\n");
  
  float Tempp_1 = temperature / 10;
  float Tempp_2 = (int)temperature % 10;
  int Tempp_3 = (int)(temperature * 10) % 10;
  int Humi_1 = humidity / 10;
  int Humi_2 = (int)humidity % 10;
  int Humi_3 = int(humidity * 10) % 10;
  
  tm.displayHex(0, Tempp_1);
  tm.displayASCIIwDot(1, Tempp_2 + '0'); // turn on dot
  tm.displayHex(2, Tempp_3);
  tm.display7Seg(3, B01011000); // Code=tgfedcba
  tm.displayHex(4, Humi_1);
  tm.displayASCIIwDot(5, Humi_2 + '0'); // turn on dot
  tm.displayHex(6, Humi_3);
  tm.display7Seg(7, B01110100); // Code=tgfedcba
  delay(2000);
}
