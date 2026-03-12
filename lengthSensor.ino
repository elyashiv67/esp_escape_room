#include <NewPing.h>

#define TRIGGER_PIN D5
#define ECHO_PIN D6
#define MAX_DISTANCE 60

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned long timeC = millis();
bool check = false;


int sensorLoop() {
  int length = sonar.ping_cm();
  bool temp = false;
  while (!check) {
    delay(50);
    length = sonar.ping_cm();
    if (length > 1 && length < 3) {
      Serial.println("hold");
      temp = true;
      if (millis() - timeC > 2000 && temp) {
        Serial.println("succes");
        check = true;
        return 1;
      }
    } else {
      timeC = millis();
      Serial.println("go to position");
      temp = false;
    }
  }
  return 0;
}