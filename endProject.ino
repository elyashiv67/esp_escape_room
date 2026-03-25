
const int clockPin = D5;  
const int latchPin = D6;  
const int dataPin = D7;   

const int relayPin = D4;
unsigned long unlockStartTime = 0; 
bool isDoorOpen = false;           
const unsigned long openDuration = 5000; 
const int secretCode[4] = {7, 3, 9, 2}; 

int revealedDigits = 0; 

const byte digitMap[11] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5 
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x40  // 10: קו אמצעי (-) לספרה מוסתרת
};

const byte digitSelect[4] = {0x08, 0x04, 0x02, 0x01}; 
int currentDigit = 0;
void setup(){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  wifi_setup();
}


void loop(){
  wifi_loop();
  updateDisplay();

  if (isDoorOpen) {
    if (millis() - unlockStartTime >= openDuration) {
      isDoorOpen = false;
      digitalWrite(relayPin, LOW); 
    }
  }
}