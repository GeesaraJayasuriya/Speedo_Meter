
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS1307 rtc;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {12, 11, 10};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int ledSegments[7] = {5, 4, 3, 2, A2, A3, A0};
int decimalPoint = A1;

LiquidCrystal_I2C lcd(0x20, 16, 2);

int demandSpeed = 0;
int actualSpeed = 0;
bool demandSpeedEntered = false;
bool actualSpeedMeasured = false;
bool logging_enabled = false;
DateTime current_time;

// Function prototypes
void initialize();
void updateDemandSpeed(char key);
void clearDemandSpeed();
void printSpeeds();
void measureActualSpeed();
void displaySegment(char character, int* ledSegments, int decimalPoint);

void setup() {
  initialize();
  for (int i = 0; i < 7; i++) {
    pinMode(ledSegments[i], OUTPUT);
  }

  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // set RTC to compile time

  // hardcoded initialization time for testing purposes
  rtc.adjust(DateTime(2023, 5, 2, 12, 0, 0));

  // Set decimal point pin as output
  pinMode(decimalPoint, OUTPUT);
}

void loop() {

  current_time = rtc.now();
  if (current_time.second() == 0 && demandSpeed > 0 && logging_enabled) {
    // log data every second
    log_data(current_time);
  }

  char key = keypad.getKey();
  if (key != NO_KEY) {

    switch (key) {
      case '*':
        // Display 'b' for '*'
        displaySegment('b', ledSegments, decimalPoint);
        clearDemandSpeed();
        break;
      case '#':
        displaySegment('E', ledSegments, decimalPoint);
        if (demandSpeedEntered && demandSpeed >= 0 && demandSpeed <= 999) {
          printSpeeds();
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ERR");
          delay(2000);
          printSpeeds();
        }
        demandSpeedEntered = false;
        actualSpeedMeasured = false;
        demandSpeed = 0;
        actualSpeed = 0;
        break;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        updateDemandSpeed(key);
        break;
    }
  }
  if (!actualSpeedMeasured && millis() % 1000 == 0) {
    measureActualSpeed();
    if (actualSpeedMeasured) {
      printSpeeds();
    }
  }

  update_display(current_time);
}

// Function to initialize the LCD and display a "Starting..." message
void initialize() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEMAND SPD: 0");
  lcd.setCursor(0, 1);
  lcd.print("ACTUAL SPD: 0");
}

// Function to update the demand speed based on the key pressed
void updateDemandSpeed(char key) {
  if (!demandSpeedEntered) {
    demandSpeed = 0;
    lcd.setCursor(13, 0);
    lcd.print("_");
  }
  demandSpeed = demandSpeed * 10 + (key - '0');
  if (demandSpeed > 999) {
    demandSpeed = 999;
  }
  lcd.setCursor(13, 0);
  lcd.print(demandSpeed);
  demandSpeedEntered = true;
}

// Function to clear the demand speed (i.e. backspace)
void clearDemandSpeed() {
  if (demandSpeedEntered) {
    demandSpeed /= 10;
    lcd.setCursor(13, 0);
    lcd.print(" ");
  }
}

// Function to print the demand and actual speeds to the LCD
void printSpeeds() {
  lcd.setCursor(0, 0);
  lcd.print("DEMAND SPD: ");
  lcd.print(demandSpeed);
  lcd.setCursor(0, 1);
  lcd.print("ACTUAL SPD: ");
  lcd.print(actualSpeed);
}

// Function to simulate measuring the actual speed
void measureActualSpeed() {
  actualSpeed = random(200, 300);
  lcd.setCursor(13, 1);
  lcd.print(actualSpeed);
  actualSpeedMeasured = true;
}

void displaySegment(char character, int* ledSegments, int decimalPoint) {
  int segmentMap[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},  // 0
    {0, 1, 1, 0, 0, 0, 0},  // 1
    {1, 1, 0, 1, 1, 0, 1},  // 2
    {1, 1, 1, 1, 0, 0, 1},  // 3
    {0, 1, 1, 0, 0, 1, 1},  // 4
    {1, 0, 1, 1, 0, 1, 1},  // 5
    {1, 0, 1, 1, 1, 1, 1},  // 6
    {1, 1, 1, 0, 0, 0, 0},  // 7
    {1, 1, 1, 1, 1, 1, 1},  // 8
    {1, 1, 1, 1, 0, 1, 1}   // 9
  };

  // Check if the character is a digit (0-9)
  if (isdigit(character)) {
    int digit = character - '0';
    for (int i = 0; i < 7; i++) {
      digitalWrite(ledSegments[i], segmentMap[digit][i]);
    }
  } else {
    // Handle non-digit characters (e.g. 'E', 'b')
    switch (character) {
      case 'E':
        digitalWrite(ledSegments[0], HIGH);
        digitalWrite(ledSegments[1], HIGH);
        digitalWrite(ledSegments[2], LOW);
        digitalWrite(ledSegments[3], HIGH);
        digitalWrite(ledSegments[4], HIGH);
        digitalWrite(ledSegments[5], HIGH);
        digitalWrite(ledSegments[6], HIGH);
        break;
      case 'b':
        digitalWrite(ledSegments[0], LOW);
        digitalWrite(ledSegments[1], LOW);
        digitalWrite(ledSegments[2], LOW);
        digitalWrite(ledSegments[3], LOW);
        digitalWrite(ledSegments[4], LOW);
        digitalWrite(ledSegments[5], LOW);
        digitalWrite(ledSegments[6], HIGH);
        break;
    }
  }

  // Turn on the decimal point
  digitalWrite(decimalPoint, HIGH);
}



void log_data(DateTime time_stamp) {
  Serial.print(time_stamp.day());
  Serial.print(".");
  Serial.print(time_stamp.month());
  Serial.print(".");
  Serial.print(time_stamp.year());
  Serial.print(" ");
  Serial.print(time_stamp.hour());
  Serial.print(":");
  Serial.print(time_stamp.minute());
  Serial.print(":");
  Serial.print(time_stamp.second());
  Serial.print(" ");
  Serial.print(demandSpeed);
  Serial.print(" ");
  Serial.println(actualSpeed);
}


void update_display(DateTime time_stamp) {
  Serial.print("Date: ");
  Serial.print(time_stamp.day());
  Serial.print(".");
  Serial.print(time_stamp.month());
  Serial.print(".");
  Serial.print(time_stamp.year());
  Serial.print(" Time: ");
  Serial.print(time_stamp.hour());
  Serial.print(":");
  if (time_stamp.minute() < 10) {
    Serial.print("0");
  }
  Serial.print(time_stamp.minute());
  Serial.print(":");
  if (time_stamp.second() < 10) {
    Serial.print("0");
  }
  Serial.print(time_stamp.second());
  Serial.print(" Demanded Speed: ");
  Serial.print(demandSpeed);
  Serial.print(" Actual Speed: ");
  Serial.println(actualSpeed);
}

void set_logging(bool enabled) {
  logging_enabled = enabled;
  if (logging_enabled) {
    Serial.println("Logging enabled");
  } else {
    Serial.println("Logging disabled");
  }
}
