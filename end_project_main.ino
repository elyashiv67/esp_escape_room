

#define IDLE 100
#define LENGTH 101
#define TEMPATURE 102



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000);  // Give the computer a full 2 seconds to connect

  Serial.println();  // Print empty lines to clear out boot garbage
  Serial.println();
  Serial.println("--- PROGRAM START ---");
  wifi_Setup();
  SendData("try", 8);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(sensorLoop() == 1){
    SendData("riddle", 1);
  }
}
