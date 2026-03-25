

#define IDLE 100
#define LENGTH 101
#define TEMPATURE 102
#define LIGHT 103
#define SIMON 104
#define END 105
int gameState = 100;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000);  // Give the computer a full 2 seconds to connect

  // 3. Print clear lines so we know it actually booted
  Serial.println();
  Serial.println();
  Serial.println("=================================");
  Serial.println("BOARD HAS BOOTED SUCCESSFULLY!");
  Serial.println("=================================");

  // Serial.println();  // Print empty lines to clear out boot garbage
  // Serial.println();
  // Serial.println("--- PROGRAM START ---");
  // wifi_Setup();
  tempatureSetup();
  lightSetup();
  simoneSetup();
  delay(1000);
  gameState = LIGHT;
}


void loop() {
  // put your main code here, to run repeatedly:
  switch (gameState) {

  case LENGTH:
  if(sensorLoop() == 1){
    Serial.println("{riddle : 1}");
    SendData("riddle", 1);
  }
  gameState = TEMPATURE;
  break;

  case TEMPATURE:
  if(tempatureLoop() == 1){
    Serial.println("succes");
    SendData("riddle", 2);
    gameState = LIGHT;
  }
  break;

  case LIGHT:
  if(lightLoop() == 1){
    Serial.println("succes");
    SendData("riddle", 3);
    gameState = SIMON;
  }

  case SIMON:
  simoneLoop();
  //in the simon loop im changing to end
  break;

  case END:

  break;
  }
}
