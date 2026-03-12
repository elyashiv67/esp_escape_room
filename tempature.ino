#include "DHT.h"
#define DHTPIN D7
#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);
float startingTemp = 0;

void tempatureSetup() {
  // put your setup code here, to run once:
  Serial.println(F("DHTxx test!"));
  dht.begin();
  delay(2000);
  startingTemp = dht.readTemperature();
  Serial.println(startingTemp);
}

int tempatureLoop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  bool result = false;

  float C = dht.readTemperature();
  if (isnan(C) || isnan(startingTemp)) {
    Serial.println("Error: Sensor returned NAN. Check wiring!");
    return 0;
  }

  Serial.print("Temperature: ");
  Serial.println(C);

  if (C - startingTemp > 1) {
    Serial.println("succes");
    return 1;
  }

  return 0;
}
