# Smart Braille Translation Tool Using Arduino Uno

## Overview
This project enables Braille translation using an **Arduino Uno-based push-button input system** and **Python-based OCR for Braille image recognition**.

## Files Included
- `braille_translator.ino` - Arduino code for push-button Braille input system.
- `braille_translator.py` - Python script for OCR-based Braille recognition and TTS conversion.
- `README.md` - Project documentation.

## Setup Instructions
### 1️⃣ Setting up Arduino
- Upload `braille_translator.ino` to Arduino Uno.
- Connect the push-button grid to the appropriate pins.

### 2️⃣ Setting up Python Environment
- Install dependencies:
  ```bash
  pip install pytesseract pyttsx3 pillow
  ```
- Run the program:
  ```bash
  python braille_translator.py
  ```

## Contributors
- **Vijay Shivannagol** - Developer
- **Shrushti Kamanache** - Developer
- **Prof. Shahak Patil** - Project Guide

## License
This project is licensed under the MIT License.
