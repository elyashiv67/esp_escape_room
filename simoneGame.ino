#define ledR D0
#define ledG D3
#define ledB D4
#define ledY D1
#define btnR D5
#define btnG D7
#define btnB D2
#define btnY D6

#define IDLE 100
#define SHOW 101
#define GAME 102
#define RESET 103
#define SOLVED 104



int leds[4] = { ledR, ledG, ledB, ledY };
int btns[4] = { btnR, btnG, btnB, btnY };
bool isPressed[4] = { false, false, false, false };
int levels[8] = {
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
};
int levelIndex = 0;
int simoneState = IDLE;
int timeGame = 800;

void idle() {
  unsigned long blinkStart = millis();
  int Random = random(4);
  bool OnOff = true;
  digitalWrite(leds[Random], HIGH);
  OnOff = true;
  while (OnOff) {
    yield(); // הוספתי את זה כי הלוח כל הזמן קרס כי הייתי תקוע על הלולאה
    if (millis() - blinkStart > 500) {
      digitalWrite(leds[Random], LOW);
    }
    if (millis() - blinkStart > 700) {
      OnOff = false;
    }
  }
  blinkStart = millis();

  if (digitalRead(btnG) == LOW && digitalRead(btnY) == LOW) {
    blinkOnce();
    simoneState = SHOW;
  }
}


void blinkGame() {

  unsigned long blink = millis();
  bool onOFF = true;
  for (int i = 0; i < 8; i++) {
    int Random = random(4);
    levels[i] = Random;
    digitalWrite(leds[levels[i]], HIGH);
    onOFF = true;
    while (onOFF) {
      yield();
      if (millis() - blink > timeGame) {
        digitalWrite(leds[levels[i]], LOW);
        if (millis() - blink > timeGame + 150) {
          onOFF = false;
        }
      }
    }
    blink = millis();
  }
  simoneState = GAME;
}

bool game() {
  for (int i = 0; i < 8; i++) {
    if (whichBtn() == levels[i]) {
      Serial.println("yes");
    } else {
      Serial.println("no");
      simoneState = RESET;
      return false;
    }
  }
  return true;
}



int whichBtn() {
  int index = -1;
  while (index == -1) {
    yield();
    for (int i = 0; i < 4; i++) {
      if (digitalRead(btns[i]) == HIGH) {
        isPressed[i] = false;
      }
    }

    for (int i = 0; i < 4; i++) {
      if (digitalRead(btns[i]) == LOW && !isPressed[i]) {
        while (digitalRead(btns[i]) == LOW) {  //פה בעצם הוא מחכה שאני ישחרר את הכפתור ורק אז הוא ממשיך copilot
          yield();
          digitalWrite(leds[i], HIGH);
        }
        isPressed[i] = true;
        digitalWrite(leds[i], LOW);
        index = i;
        delay(100);
      }
    }
  }
  return index;
}

void reset() {
  levelIndex = 0;
  timeGame = 800;
  blinkOnce();
  for (int i = 0; i < 8; i++) {
    levels[i] = -1;
    if (i < 4) {
      isPressed[i] = false;
    }
  }

  simoneState = IDLE;
}


void blinkOnce() {

  unsigned int time = millis();
  bool on = true;

  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
  }
  on = true;
  while (on) {
    yield();
    if (millis() - time > 600) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(leds[i], LOW);
      }
      if (millis() - time > 1000) {
        on = false;
      }
    }
  }
}


void simoneSetup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(A0));
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(btns[i], INPUT_PULLUP);
  }
}

void simoneLoop() {
  // put your main code here, to run repeatedly:
  switch (simoneState) {
    case IDLE:
      idle();
      break;

    case SHOW:
      blinkGame();
      break;

    case GAME:
        if(game()){
          Serial.println("succes");
          simoneState = SOLVED;
        }
      break;

    case RESET:
      reset();
      break;

    case SOLVED:
    SendData("riddle", 3);
    gameState = LENGTH; // in here i will put the next riddle
    break;
  }
}
