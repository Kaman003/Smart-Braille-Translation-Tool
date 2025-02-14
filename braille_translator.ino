#include <Wire.h>

const int topright = 8;
const int topleft = 4;
const int midright = 3;
const int midleft = 2;
const int botright = 12;
const int botleft = 6;

bool brailleDots[3][2] = {{false, false}, {false, false}, {false, false}};
unsigned long inputStartTime = 0;
const unsigned long INPUT_WINDOW = 10000;
bool inputInProgress = false;
bool doTranslate = false;

void setup() {
  Serial.begin(9600);

  pinMode(topright, INPUT_PULLUP);
  pinMode(topleft, INPUT_PULLUP);
  pinMode(midright, INPUT_PULLUP);
  pinMode(midleft, INPUT_PULLUP);
  pinMode(botright, INPUT_PULLUP);
  pinMode(botleft, INPUT_PULLUP);

  Serial.println("Braille Translator Ready. Waiting for input...");
}

void loop() {
  if (!inputInProgress) {
    startInputCapture();
  }

  if (inputInProgress) {
    updateBrailleDot(topright, 0, 1, "Top Right");
    updateBrailleDot(topleft, 0, 0, "Top Left");
    updateBrailleDot(midright, 1, 1, "Mid Right");
    updateBrailleDot(midleft, 1, 0, "Mid Left");
    updateBrailleDot(botright, 2, 1, "Bottom Right");
    updateBrailleDot(botleft, 2, 0, "Bottom Left");

    if (millis() - inputStartTime >= INPUT_WINDOW) {
      doTranslate = true;
      inputInProgress = false;
    }
  }

  if (doTranslate) {
    Serial.println("Final Braille Configuration:");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 2; j++) {
        Serial.print(brailleDots[i][j] ? "1 " : "0 ");
      }
      Serial.println();
    }

    Serial.println("Translation complete.");
    clearBrailleInput();
    doTranslate = false;
    Serial.println("Ready for next input.");
    delay(3000);
  }
}

void startInputCapture() {
  inputStartTime = millis();
  inputInProgress = true;
  clearBrailleInput();
  Serial.println("Input window started.");
}

void updateBrailleDot(int pin, int row, int col, const String& buttonName) {
  static bool lastState[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
  bool currentState = digitalRead(pin);

  if (lastState[pin] == HIGH && currentState == LOW) {
    brailleDots[row][col] = !brailleDots[row][col];
    Serial.print(buttonName + " Button: Dot [");
    Serial.print(row);
    Serial.print("][");
    Serial.print(col);
    Serial.print("] set to ");
    Serial.println(brailleDots[row][col] ? "HIGH" : "LOW");
  }
  
  lastState[pin] = currentState;
}

void clearBrailleInput() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      brailleDots[i][j] = false;
    }
  }
}
