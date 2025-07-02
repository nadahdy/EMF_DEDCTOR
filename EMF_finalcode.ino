#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1  // Reset pin not used with I2C
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIN_ANTENNA A0
#define BUZZER_PIN 12  // Pin where the buzzer is connected
#define CHECK_DELAY 1000
#define THRESHOLD 600  // Set threshold for buzzer trigger

// Example image (must match the correct size or be resized accordingly)
const unsigned char zmaj[] PROGMEM = {
  // Your actual image data goes here. The current example is incomplete.
  0xFF, 0xFF, 0x81, 0x81, 0x81, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF
  // Ensure this array has correct image data for the proper display size
};

void setup() {
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Stay here if OLED fails to initialize
  }

  display.clearDisplay();

  // Check the image size matches (60x60 for now)
  display.drawBitmap(34, 2, zmaj, 60, 60, SSD1306_WHITE);  
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(BUZZER_PIN, OUTPUT);  // Set the buzzer pin as output
}

void loop() {
  static unsigned long lastCheck = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastCheck >= CHECK_DELAY) {
    lastCheck = currentMillis;

    int antennaValue = analogRead(PIN_ANTENNA);  // Read the antenna value (0-1023)
    Serial.print("Antenna Value: ");
    Serial.println(antennaValue);

    // Display antenna value on the OLED screen
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Antenna:");
    display.setCursor(0, 10);
    display.print(antennaValue);
    display.display();

    // Check if the antenna value exceeds the threshold to trigger the buzzer
    if (antennaValue > THRESHOLD) {
      Serial.println("Antenna Value exceeds threshold. Buzzer ON!");
      digitalWrite(BUZZER_PIN, HIGH);  // Turn the buzzer on
    } else {
      digitalWrite(BUZZER_PIN, LOW);   // Turn the buzzer off
    }
  }
}
