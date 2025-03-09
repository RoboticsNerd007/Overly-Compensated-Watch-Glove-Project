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
//const int dateOffsetDays = -10; 


// Define buttons pin
const int timeButtonPin = 11; // Button for date
const int dateButtonPin = 12; // Button for month
const int gyroButtonPin = 13; //Button for Gyro data


//Define calibration for MPU - 6050
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyX,GyY;
void setup() {
  
  pinMode(timeButtonPin, INPUT_PULLUP);
  pinMode(dateButtonPin, INPUT_PULLUP);
  pinMode(gyroButtonPin, OUTPUT);

  // Set LED pins as output
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  
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

  delay(1000);

  // Initialize the DS1307 RTC
  rtc.begin();
 Wire.begin();
 
  // If the RTC isn't running, set it to the compile time of the sketch
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
 
 //Initiate Wire Transmission
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

}

void loop() {
  DateTime now = rtc.now();
  
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

   //Check if Gyro button is pressed
  if (digitalRead(gyroButtonPin) == LOW){
    displayGyro();
  }

  delay(1000); // Delay for debounce and to prevent excessive looping
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
  lcd.print(now.day() + 1, DEC);
  lcd.print('/');
  lcd.print(now.month() + 4, DEC);
  lcd.print('/');
  lcd.print(now.year() + 4, DEC);
  delay(1000); // Display for 1 second
}

void displayGyro(){
Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
   Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  lcd.clear();
    lcd.print("GyroX: ");
    lcd.print(GyX);
    lcd.setCursor(0, 1);
    lcd.print("GyroY: ");
    lcd.print(GyY);
    lcd.setCursor(0, 2);

    delay(1000);
}