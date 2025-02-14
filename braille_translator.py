CODE FOR ML
import gradio as gr
import pytesseract
import pyttsx3
import numpy as np
import io
import os
import speech_recognition as sr
from PIL import Image, ImageOps
import uuid

class BrailleTranslator:
    BRAILLE_DICT = {
        # [Previous comprehensive Braille dictionary]
         '1': 'O.....', '2': 'O.O...', '3': 'OO....', '4': 'OO.O..',
        '5': 'O..O..', '6': 'OOO...', '7': 'OOOO..', '8': 'O.OO..',
        '9': '.OO...', '0': '.OOO..', 
        'a': 'O.....', 'b': 'O.O...', 'c': 'OO....', 'd': 'OO.O..', 'e': 'O..O..',
        'f': 'OOO...', 'g': 'OOOO..', 'h': 'O.OO..', 'i': '.OO...', 'j': '.OOO..',
        'k': 'O...O.', 'l': 'O.O.O.', 'm': 'OO..O.', 'n': 'OO.OO.', 'o': 'O..OO.',
        'p': 'OOO.O.', 'q': 'OOOOO.', 'r': 'O.OOO.', 's': '.OO.O.', 't': '.OOOO.',
        'u': 'O...OO', 'v': 'O.O.OO', 'w': '.OOO.O', 'x': 'OO..OO', 'y': 'OO.OOO',
        'z': 'O..OOO', ' ': '......', '.': '..OO.O', ',': '..O...', '?': '..O.OO',
        ':': '..OO..', ';': '..O.O.', '-': '....OO', '/': '.O..O.', '<': '.OO..O',
        '>': 'O.OOOO', '(': 'O.O..O', ')': '.O.OO.', 'capital': '.....O', 'decimal': '.O...O', 'number': '.O.OOO'
    }

    def _init_(self):
        # Initialize TTS engine with multiple voices
        self.tts_engine = pyttsx3.init()
        
        # Get available voices
        self.voices = self.tts_engine.getProperty('voices')
        
        # Default settings
        self.tts_engine.setProperty('rate', 150)
        self.tts_engine.setProperty('volume', 0.9)
    
    def get_voice_options(self):
        """Return list of available voice names"""
        return [voice.name for voice in self.voices]

    def set_voice(self, voice_name):
        """Set TTS voice by name"""
        for voice in self.voices:
            if voice.name == voice_name:
                self.tts_engine.setProperty('voice', voice.id)
                return True
        return False

    def text_to_speech(self, text, voice_name=None, speech_rate=150, volume=0.9):
        """Advanced text-to-speech with more customization"""
        try:
            # Set voice if specified
            if voice_name:
                self.set_voice(voice_name)
            
            # Set speech rate and volume
            self.tts_engine.setProperty('rate', speech_rate)
            self.tts_engine.setProperty('volume', volume)
            
            # Generate unique filename
            unique_filename = f"audio_{uuid.uuid4()}.wav"
            output_file = os.path.join("temp_audio", unique_filename)
            
            # Ensure temp directory exists
            os.makedirs("temp_audio", exist_ok=True)
            
            # Save speech to the wav file
            self.tts_engine.save_to_file(text, output_file)
            self.tts_engine.runAndWait()
            
            return output_file
        except Exception as e:
            print(f"Error in text-to-speech conversion: {str(e)}")
            return None
    def _is_valid_braille_length(self, text):
        return len(text) % 6 == 0

    def is_braille(self, text):
        if text is None:
            return "Your input is null. Please enter a valid text."
        return all(char in 'O.' for char in text.replace('\n', '').replace(' ', ''))

    def english_to_braille(self, text):
        # Wrap the input text in quotes if not already quoted
        if (text.startswith('"') and text.endswith('"')) or (text.startswith("'") and text.endswith("'")):
            text = text  # Already quoted
        else:
            text = f'"{text}"'  # Wrap in double quotes

        if text is None:
            return "Your input is null. Please enter a valid text."
        
        if not isinstance(text, str):
            return "Input text must be a string."

        # Remove the surrounding quotes for processing
        text = text.strip('"\'')
        
        if len(text) == 0:
            return "Input text is empty. Please enter some text."

        braille_text = ''
        is_number = False
        
        for char in text:
            if char.isupper():
                braille_text += self.BRAILLE_DICT.get('capital', '')
                braille_text += self.BRAILLE_DICT.get(char.lower(), '')
                is_number = False
            elif char.isdigit():
                if not is_number:
                    braille_text += self.BRAILLE_DICT.get('number', '')
                    is_number = True
                braille_text += self.BRAILLE_DICT.get(char, '')
            elif char == '.':
                braille_text += self.BRAILLE_DICT.get('decimal', '')
                is_number = False
            elif char == ' ':
                braille_text += self.BRAILLE_DICT.get(' ', '')
                is_number = False
            elif char in self.BRAILLE_DICT:
                braille_text += self.BRAILLE_DICT[char]
                is_number = False
            else:
                braille_text += f"Character '{char}' not found in Braille dictionary. Skipping."
        
        return braille_text.strip()

    def braille_to_english(self, text):
        if text is None:
            return "Your input is null. Please enter a valid text."

        if not isinstance(text, str):
            return "Input text must be a string."
        
        cleaned_text = text.replace('\n', '').replace(' ', '')
        if not self._is_valid_braille_length(cleaned_text):
            return "Braille text length must be a multiple of 6 characters."
        
        braille_chars = [cleaned_text[i:i+6] for i in range(0, len(cleaned_text), 6)]
        reversed_braille_dict = {v: k for k, v in self.BRAILLE_DICT.items()}
        
        english_text = ''
        skip_next = False
        
        for i, braille in enumerate(braille_chars):
            if skip_next:
                skip_next = False
                continue
            
            if braille == '.....O':
                if i + 1 < len(braille_chars):
                    next_braille = braille_chars[i + 1]
                    if next_braille in reversed_braille_dict:
                        english_text += reversed_braille_dict[next_braille].upper()
                        skip_next = True
                    else:
                        english_text += '?'
                continue
            
            if braille == '.O...O':
                if i + 1 < len(braille_chars):
                    next_braille = braille_chars[i + 1]
                    if next_braille in reversed_braille_dict:
                        english_text += '.'
                        skip_next = True
                    else:
                        english_text += '?'
                continue
            
            if braille == '.O.OOO':
                braille_to_number = {
                    'O.....': '1', 'O.O...': '2', 'OO....': '3', 'OO.O..': '4',
                    'O..O..': '5', 'OOO...': '6', 'OOOO..': '7', 'O.OO..': '8',
                    '.OO...': '9', '.OOO..': '0'
                }
                reversed_braille_dict_number = {v: k for k, v in braille_to_number.items()}
                
                remaining_string = ''.join(braille_chars[i+1:])
                symbols = remaining_string.split('......')
                symbols_to_convert = symbols[0]
    
                numbers = []
                k = 0
                while k < len(symbols_to_convert):
                    symbol = symbols_to_convert[k:k+6]
                    if len(symbol) == 6 and symbol in braille_to_number:
                        numbers.append(braille_to_number[symbol])
                    k += 6
                
                english_text += ''.join(numbers)
                break

            if braille in reversed_braille_dict:
                english_text += reversed_braille_dict[braille]
            else:
                english_text += f"Braille pattern '{braille}' not found in dictionary. Skipping."
        
        return english_text.strip()

    def translate(self, text):
        if text is None:
            return "Your input is null. Please enter a valid text."
        
        if not isinstance(text, str):
            return "Input text must be a string."
        
        if len(text) == 0:
            return "Input text is empty. Please enter some text."
        
        if self.is_braille(text):
            return self.braille_to_english(text)
        else:
            return self.english_to_braille(text)
    # [Rest of the previous translation methods remain the same]
    # Include methods like english_to_braille, braille_to_english, ocr_image, etc.
    def translate(self, text):
        if not isinstance(text, str):
            return "Input text must be a string."
        if not text:
            return "Input text is empty."

        if self.is_braille(text):
            return self.braille_to_english(text)
        else:
            return self.english_to_braille(text)# [Previous BrailleTranslator class code remains the same]
    # Include all previous methods like _init_, english_to_braille, etc.

    def audio_to_text(self, audio_input):
        """
        Convert audio input to text using speech recognition
        """
        try:
            recognizer = sr.Recognizer()
            
            # If audio is a filepath, open it
            if isinstance(audio_input, str):
                with sr.AudioFile(audio_input) as source:
                    audio_data = recognizer.record(source)
            else:
                # Convert Gradio audio to file
                temp_audio_path = "temp_audio_input.wav"
                audio_input.save(temp_audio_path)
                with sr.AudioFile(temp_audio_path) as source:
                    audio_data = recognizer.record(source)
                # Clean up temporary file
                os.remove(temp_audio_path)
            
            # Recognize speech
            text = recognizer.recognize_google(audio_data)
            return text
        except sr.UnknownValueError:
            return "Could not understand audio"
        except sr.RequestError:
            return "Speech recognition service unavailable"
        except Exception as e:
            return f"Error processing audio: {str(e)}"

def advanced_translate(
    text_input, 
    image_input, 
    audio_input,
    convert_to_voice, 
    voice_choice, 
    speech_rate, 
    volume
):
    # Translate or OCR first
    if image_input:
        text_from_image = translator.ocr_image(image_input)
        if text_from_image and not text_from_image.startswith("Error") and not text_from_image.startswith("No image"):
            text_input = text_from_image
    
    if audio_input is not None:
        text_from_audio = translator.audio_to_text(audio_input)
        if text_from_audio and not text_from_audio.startswith("Error"):
            text_input = text_from_audio

    if not text_input:
        return "Please provide text input, upload a clear image, or record audio.", None

    # Translate the text
    translated_text = translator.translate(text_input)

    # Convert to voice if requested
    audio_output = None
    if convert_to_voice:
        audio_output = translator.text_to_speech(
            translated_text, 
            voice_name=voice_choice, 
            speech_rate=speech_rate, 
            volume=volume
        )

    return translated_text, audio_output

# Initialize translator
translator = BrailleTranslator()

# Create Gradio interface with enhanced styling
with gr.Blocks(theme=gr.themes.Soft()) as iface:
    gr.Markdown("# 🌟 Braille Translator & Voice Assistant")
    
    with gr.Row():
        with gr.Column(scale=2):
            text_input = gr.Textbox(
                label="Enter Text", 
                placeholder="Type or paste text here...",
                lines=4
            )
            image_input = gr.Image(
                type="pil", 
                label="Upload Image (Optional)", 
                sources=["upload", "webcam"]
            )
            audio_input = gr.Audio(
                type="filepath", 
                label="Record Audio (Optional)"
            )
        
        with gr.Column(scale=1):
            convert_to_voice = gr.Checkbox(label="Convert to Voice")
            voice_choice = gr.Dropdown(
                label="Select Voice", 
                choices=translator.get_voice_options(),
                value=translator.get_voice_options()[0] if translator.get_voice_options() else None
            )
            speech_rate = gr.Slider(
                minimum=50, 
                maximum=300, 
                value=150, 
                label="Speech Rate"
            )
            volume = gr.Slider(
                minimum=0, 
                maximum=1, 
                value=0.9, 
                label="Volume"
            )
    
    with gr.Row():
        translate_btn = gr.Button("Translate & Convert", variant="primary")
        clear_btn = gr.Button("Clear", variant="secondary")
    
    output_text = gr.Textbox(label="Translated Text")
    audio_output = gr.Audio(label="Audio Output", type="filepath")
    
    # Button actions
    translate_btn.click(
        fn=advanced_translate, 
        inputs=[
            text_input, 
            image_input, 
            audio_input,
            convert_to_voice, 
            voice_choice, 
            speech_rate, 
            volume
        ],
        outputs=[output_text, audio_output]
    )
    
    clear_btn.click(
        fn=lambda: (None, None, None, False, None, 150, 0.9),
        outputs=[
            text_input, 
            image_input, 
            audio_input,
            convert_to_voice, 
            voice_choice, 
            speech_rate, 
            volume
        ]
    )

    gr.Markdown("""
    ### 🚀 Features
    - Translate between English and Braille
    - OCR from images
    - Audio input support
    - Text-to-Speech conversion
    - Multiple voice options
    - Customizable speech settings
    """)

# Launch the interface
iface.launch(debug=True)
