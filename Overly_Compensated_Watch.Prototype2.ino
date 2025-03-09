//This project took me over 3 months to obtain. This shows the endurance and suffering I had to go through, but the result of trail and error to make up what is
//the watch clock I made today!

//_____________________________________________________________________________________________________//

#include <Wire.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

#define LED_PIN_1 8  // Pin for LED 1
#define LED_PIN_2 9  // Pin for LED 2
#define LED_PIN_3 10 // Pin for LED 3
#define LED_DELAY 500 // Delay between turning on each LED in milliseconds


RTC_DS1307 rtc;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // LCD pins
const int timeOffsetHours = 2; // 1 hour behind
const int timeOffsetMinutes = -30; // 30 minutes behind
const int switchPin = 45;  // Pin for the rocker switch
const int ledBinaryPins[] = {23, 25, 27, 29, 31}; // Array to hold the LED pin numbers
const int ledPin4 = 33;
const int ledPin5 = 35;



// Define buttons pin
const int timeButtonPin = 11; // Button for date
const int dateButtonPin = 12; // Button for month

// HC-SR04 ultrasonic sensor pins
const int trigPin = 41;
const int echoPin = 43;

//button Pin
const int buttonPin = 13;

// Variables for storing distances
long duration;
int distance;

// Variables for button state
int buttonState = 0;  // current state of the button
int lastButtonState = 0;  // previous state of the button
int count = 0; // Variable to hold the current count value
bool switchState = false; // Variable to hold the state of the switch

void setup() {
  
  pinMode(timeButtonPin, INPUT_PULLUP);
  pinMode(dateButtonPin, INPUT_PULLUP);

  // Set LED pins as output
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  

   pinMode(switchPin, INPUT_PULLUP); // Set switch pin as input with internal pull-up resistor
  pinMode (ledPin2, OUTPUT);
  // Set all LED pins as output
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 

 // Turn on LED 1
  digitalWrite(LED_PIN_1, HIGH);
  delay(LED_DELAY);

  // Turn on LED 2
  digitalWrite(LED_PIN_2, HIGH);
  delay(LED_DELAY);
  
  //Turn on LED 3
  digitalWrite(LED_PIN_3, HIGH);


  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Take off!");

  delay(2000); // Wait for 2 seconds before changing the text
                                                              //Fun Text to Display
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Toward a Dream!");

  delay(500);

  // Initialize the DS1307 RTC
  rtc.begin();
 Wire.begin();
 
  // If the RTC isn't running, set it to the compile time of the sketch
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();
  switchState = digitalRead(switchPin);
  buttonState = digitalRead(buttonPin);


  // Adjust time by subtracting 1 hour and 30 minutes
  now = now - TimeSpan(1, timeOffsetHours, timeOffsetMinutes, 0);


    

  // Check if time button is pressed
  if (digitalRead(timeButtonPin) == LOW) {
    displayTime(now);
  } 

  // Check if date button is pressed
  if (digitalRead(dateButtonPin) == LOW) {
    displayDate(now);
  } 

  delay(500); // Delay for debounce and to prevent excessive looping

   // Check if button is pressed (low when pressed due to pull-up resistor)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Button press detected, start measuring distance and control LED
    measureDistanceAndControlLED();
  }
  
  // Update last button state
  lastButtonState = buttonState;

  
    binaryTracker();
}

void displayTime(DateTime now) {
  lcd.clear();
  lcd.print("Watch: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  delay(1000); // Display for 1 second
}

void displayDate(DateTime now) {
  lcd.clear();
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  delay(1000); // Display for 1 second
}

void measureDistanceAndControlLED() {
  while (true) {
    // Clear the trigger pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Send a 10 microsecond pulse to trigger the sensor
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Read the echo pulse
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate distance in cm
    distance = duration * 0.034 / 2;
    
    // Print distance to serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // Control LED based on distance
    if (distance < 20) {
      // Close object detected, blink LED fast
      blinkLED(50); // Fast blink interval in milliseconds
    } else {
      // Far object detected, blink LED slowly
      blinkLED(500); // Slow blink interval in milliseconds
    }
    
    // Check if button is pressed again to stop the measurement
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && lastButtonState == HIGH) {
      // Button press detected, stop measuring and break out of loop
      digitalWrite(ledPin3, LOW); // Turn off LED
      break;
    }
    
    // Update last button state
    lastButtonState = buttonState;
    
    delay(100); // Small delay between measurements
  }
}

void blinkLED(int interval) {
  digitalWrite(ledPin3, HIGH);
  delay(interval);
  digitalWrite(ledPin3, LOW);
  delay(interval);
}

void binaryTracker(){
  if(switchState == LOW){
    // Convert count to binary and display on LEDs
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPins[i], bitRead(count, i));
    }
    
    delay(500); // Delay for half a second
    
    count++; // Increment count
    
    if (count == 32) { // Reset count when it reaches 32 (2^5)
      count = 0;
    }
       if (count == 0){
    digitalWrite(ledPin2, HIGH);

  delay(1000);

  digitalWrite(ledPin2, LOW);
  }

   else {
    // If switch is OFF, turn off all LEDs
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledPins[i], LOW);
    }
}
  }
}