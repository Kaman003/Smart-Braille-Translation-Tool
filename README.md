# Smart Braille Translation Tool Using Arduino Uno

## 📌 Project Overview
The **Smart Braille Translation Tool** is an assistive technology designed to help visually impaired individuals convert Braille input into readable text and speech. The system integrates an **Arduino Uno-based push-button interface** for manual Braille input and a **camera-based OCR module** for automatic Braille recognition.

## 🎯 Features
- **Push-Button Braille Input**: A 2x3 push-button grid to simulate Braille character input.
- **Camera-Based Braille Recognition**: Uses image processing (OCR) to recognize Braille text from images.
- **Text-to-Speech (TTS) Output**: Converts translated text to audio for auditory feedback.
- **Arduino Integration**: Handles input processing and text conversion.
- **User-Friendly Interface**: Simple and accessible UI for users with disabilities.

---

## 🔧 Hardware Requirements
- **Arduino Uno**
- **2x3 Push-Button Grid** (for Braille input)
- **Camera Module / Webcam** (for OCR-based input)
- **Speaker / Audio Output** (for text-to-speech functionality)
- **Resistors & Wires** (for button connections)

## 🖥️ Software Requirements
- **Arduino IDE** (for programming the Arduino Uno)
- **Python 3.x** (for image processing and text-to-speech conversion)
- **Gradio** (for UI interface)
- **Pytesseract** (for OCR processing)
- **pyttsx3** (for offline text-to-speech conversion)

---

## 🛠️ Installation Guide
### **1️⃣ Setting up Arduino**
1. Install **Arduino IDE**.
2. Upload the `braille_translator.ino` sketch to your Arduino Uno.
3. Connect the **2x3 push-button grid** to the correct pins on the Arduino board.

### **2️⃣ Setting up Python Environment**
1. Install Python (>=3.7)
2. Install the required dependencies:
   ```bash
   pip install gradio pytesseract pyttsx3 numpy pillow speechrecognition

`
### **🎮 How It Works**
- **Braille Input via Push-Buttons**: Users press buttons on the **2x3 grid**, and the system translates them into letters.  
- **Braille Recognition via OCR**: Users upload a Braille image, and OCR detects the Braille pattern.  
- **Translation & Output**:  
  - The detected Braille text is converted into **English text**.  
  - The output is **displayed on-screen** and **read aloud** using TTS.  
```

```
## 💡 Future Improvements
- **Support for multiple languages**  
- **Integration with AI for better Braille recognition**  
- **Mobile App Version for easier accessibility**  
- **Wireless Module for portability**  
