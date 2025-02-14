#include <Wire.h>

// Pin Definitions for the push-button grid
const int topright = 8;  // Top right pushbutton pin
const int topleft = 4;   // Top left pushbutton pin
const int midright = 3;  // Middle right pushbutton pin//[1][1]
const int midleft = 2;   // Middle left pushbutton pin
const int botright = 12;  // Bottom right pushbutton pin
const int botleft = 6;   // Bottom left pushbutton pin

// Variables
bool brailleDots[3][2] = {{false, false}, {false, false}, {false, false}};  // Array to store Braille input
unsigned long inputStartTime = 0;
const unsigned long INPUT_WINDOW = 10000;  // 10-second input window
bool inputInProgress = false;
bool doTranslate = false;

void setup() {
  Serial.begin(9600);

  // Set push-button pins as INPUT_PULLUP
  pinMode(topright, INPUT_PULLUP);
  pinMode(topleft, INPUT_PULLUP);
  pinMode(midright, INPUT_PULLUP);
  pinMode(midleft, INPUT_PULLUP);
  pinMode(botright, INPUT_PULLUP);
  pinMode(botleft, INPUT_PULLUP);

  Serial.println("Braille Translator Ready. Waiting for input...");
}

void loop() {
  // Start input window automatically if not already in progress
  if (!inputInProgress) {
    startInputCapture();
  }

  // Check if input window is active
  if (inputInProgress) {
    // Dynamic dot setting for each button with MORE VERBOSE DEBUG
    updateBrailleDot(topright, 0, 1, "Top Right");
    updateBrailleDot(topleft, 0, 0, "Top Left");
    updateBrailleDot(midright, 1, 1, "Mid Right");
    updateBrailleDot(midleft, 1, 0, "Mid Left");
    updateBrailleDot(botright, 2, 1, "Bottom Right");
    updateBrailleDot(botleft, 2, 0, "Bottom Left");

    // Check if input window has expired
    if (millis() - inputStartTime >= INPUT_WINDOW) {
      doTranslate = true;
      inputInProgress = false;
    }
  }

  // Separate translation to ensure it happens after input window
  if (doTranslate) {
    // Print full Braille configuration
    Serial.println("Final Braille Configuration:");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 2; j++) {
        Serial.print(brailleDots[i][j] ? "1 " : "0 ");
      }
      Serial.println();
    }

    String translatedText = translateBraille();
    Serial.print("Translated Text: ");
    Serial.println(translatedText);
    
    // Reset for next input
    clearBrailleInput();
    doTranslate = false;
    Serial.println("Ready for next input. Input window will start soon.");
    delay(3000);  // Pause before next input window
  }
}

void startInputCapture() {
  inputStartTime = millis();
  inputInProgress = true;
  clearBrailleInput();
  Serial.println("Input window started. You have 10 seconds to set Braille dots.");
}

void updateBrailleDot(int pin, int row, int col, const String& buttonName) {
  static bool lastState[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
  
  // Check current state of the button
  bool currentState = digitalRead(pin);
  
  // Detect button press and release (transition from HIGH to LOW)
  if (lastState[pin] == HIGH && currentState == LOW) {
    // Toggle the dot state
    brailleDots[row][col] = !brailleDots[row][col];
    
    // More verbose debug print
    Serial.print(buttonName + " Button: Dot [");
    Serial.print(row);
    Serial.print("][");
    Serial.print(col);
    Serial.print("] set to ");
    Serial.println(brailleDots[row][col] ? "HIGH" : "LOW");
  }
  
  // Update last state
  lastState[pin] = currentState;
}

String translateBraille() {
  // Comprehensive translation logic with A-Z and number support
  // Letters A-Z
  if (!brailleDots[0][0] && !brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "A";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "B";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "C";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      brailleDots[1][0] && brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "D";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      !brailleDots[1][0] && brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "E";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "F";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "G";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      !brailleDots[1][0] && brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "H";
  
  if (!brailleDots[0][0] && brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "I";
  
  if (!brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      !brailleDots[2][0] && !brailleDots[2][1]) return "J";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "K";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "L";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "M";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "N";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      !brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "O";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "P";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "Q";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      !brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "R";
  
  if (!brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "S";
  
  if (!brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && !brailleDots[2][1]) return "T";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) return "U";
  
  if (brailleDots[0][0] && brailleDots[0][1] && 
      !brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) return "V";
  
  if (!brailleDots[0][0] && brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) return "W";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      brailleDots[1][0] && !brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) return "X";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) return "Y";
  
  if (brailleDots[0][0] && !brailleDots[0][1] && 
      !brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) return "Z";
  
  // Numbers (using special Braille number prefix)
  // For numbers, add a special number sign (dots 3,4,5,6)
  if (brailleDots[1][0] && brailleDots[1][1] && 
      brailleDots[2][0] && brailleDots[2][1]) {
    // Number translations
    if (!brailleDots[0][0] && !brailleDots[0][1]) return "1";
    if (brailleDots[0][0] && !brailleDots[0][1]) return "2";
    if (brailleDots[0][0] && brailleDots[0][1]) return "3";
    if (brailleDots[0][0] && brailleDots[0][1] && brailleDots[1][0]) return "4";
    if (brailleDots[0][0] && brailleDots[0][1] && !brailleDots[1][1]) return "5";
    if (brailleDots[0][0] && brailleDots[0][1] && brailleDots[1][0] && brailleDots[1][1]) return "6";
    if (brailleDots[0][0] && brailleDots[0][1] && brailleDots[1][0] && !brailleDots[1][1]) return "7";
    if (brailleDots[0][0] && brailleDots[0][1] && !brailleDots[1][0] && brailleDots[1][1]) return "8";
    if (!brailleDots[0][0] && brailleDots[0][1] && brailleDots[1][0] && !brailleDots[1][1]) return "9";
    if (!brailleDots[0][0] && brailleDots[0][1] && brailleDots[1][0] && brailleDots[1][1]) return "0";
  }
  
  return "Invalid Input";
}

void clearBrailleInput() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      brailleDots[i][j] = false;
    }
  }
}
