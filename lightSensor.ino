int light_sensor = A0;

unsigned long dimTime = 0;
bool isDim = false;


int start_light = 0;


void lightSetup() {
  start_light = map(analogRead(light_sensor), 0, 1023, 0, 100);
  Serial.println("light game start");
}

int lightLoop() {
  int raw_light = analogRead(light_sensor);
  int light = map(raw_light, 0, 1023, 0, 100);

  int target_light = start_light - 20;

  if (light <= target_light) {

    if (!isDim) {
      dimTime = millis();
      isDim = true;
      Serial.println("stay here");
      
    } else {
      if (millis() - dimTime >= 2000) {
        Serial.println("succes");
        return 1;
      }
    }

  } else {
    if (isDim) {
      Serial.println("light got stronger dim the room");
      isDim = false;
    }
  }
  return 0;
}