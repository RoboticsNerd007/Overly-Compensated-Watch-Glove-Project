
#include <Wire.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LED_DELAY 500




void setup() {
  // Initialize the display
  if(!display.begin(SSD1306_PAGEADDR, 0x3C)) {  // Change 0x3C if needed (common address is 0x3C)
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // Don't proceed, loop forever
  }

  display.display();  // Show initial splash screen
  delay(2000);        // Pause for 2 seconds

  display.clearDisplay();  // Clear the display buffer

  // Set text size and color
  display.setTextSize(1);  // Text size
  display.setTextColor(SSD1306_WHITE);  // White text

  // Set text cursor position
  display.setCursor(0,0);

  // Print text to the display
  display.print(F("Hello, World!"));

  // Update the display with the text
  display.display();
}

void loop() {
  // You can add code here to update the display periodically if needed
}