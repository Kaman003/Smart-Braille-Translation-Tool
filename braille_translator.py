import pytesseract
import pyttsx3
from PIL import Image

def ocr_image(image_path):
    image = Image.open(image_path)
    text = pytesseract.image_to_string(image)
    return text.strip()

def text_to_speech(text):
    engine = pyttsx3.init()
    engine.say(text)
    engine.runAndWait()

if __name__ == "__main__":
    image_path = "braille_sample.png"  
    extracted_text = ocr_image(image_path)
    print("Extracted Text:", extracted_text)
    text_to_speech(extracted_text)
