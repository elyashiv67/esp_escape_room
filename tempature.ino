#include "DHT.h"
#define DHTPIN D7
#define DHTTYPE DHT22

#define fanR D8
#define fanL D1


DHT dht(DHTPIN, DHTTYPE);
float startingTemp = 0;

void tempatureSetup() {
  // put your setup code here, to run once:
  Serial.println(F("DHTxx test!"));
  dht.begin();
  pinMode(fanR,OUTPUT);
  pinMode(fanL, OUTPUT);
  delay(2000);
  startingTemp = dht.readTemperature();
  Serial.print("starting Temp : ");
  Serial.println(startingTemp);
  // digitalWrite(fanR,HIGH);
  // digitalWrite(fanL,LOW);
}

int tempatureLoop() {
  // put your main code here, to run repeatedly:
  delay(500);
  bool result = false;

  float C = dht.readTemperature();
  if (isnan(C) || isnan(startingTemp)) {
    Serial.println("Error: Sensor returned NAN. Check wiring!");
    return 0;
  }

  Serial.print("Temperature: ");
  Serial.println(C);

  if (startingTemp - C > 1) {
    Serial.println("succes");
    digitalWrite(fanR,LOW);
    return 1;
  }

  return 0;
}
