// Lab 3
// Watchdog Timer


// Defines the WDT
#include <avr/wdt.h>
#define MEGA_LED 13
// Defines 7 segment display segments
#define SEGMENT_A 3
#define SEGMENT_B 4
#define SEGMENT_C 5
#define SEGMENT_D 6
#define SEGMENT_E 7
#define SEGMENT_F 8
#define SEGMENT_G 9
#define SEGMENT_H 10
#define numPins 8



// Sets the pins to output
void setup()
{
  pinMode(MEGA_LED, OUTPUT);
  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  pinMode(SEGMENT_H, OUTPUT);

  // Sets the inital state of the display
  // Just the G segment is on
  digitalWrite(SEGMENT_A, HIGH);
  digitalWrite(SEGMENT_B, HIGH);
  digitalWrite(SEGMENT_C, HIGH);
  digitalWrite(SEGMENT_D, HIGH);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, LOW);
  digitalWrite(SEGMENT_H, HIGH);
  Serial.begin(9600);

  // Initial display on the serial monitor to the user
  Serial.print("User input will be shown on 7 segment display.\n");
  Serial.print("Please enter an integer from 0 to 15\n");
}


void loop()
{
  unsigned long timeSinceInput = 0;
  unsigned long prevInputTime = 0;

  // Sets up array for the segments of the display
  int segPins[numPins] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E,
                          SEGMENT_F, SEGMENT_G, SEGMENT_H};

  while(1)
  {
    int input;
    // Checks for input
    if(Serial.available() > 0)
    {
      input = Serial.parseInt();
      Serial.print("User entered: ");
      Serial.print(input);
      Serial.print("\n");

      // Checks for invalid input.
      if(input < 0 || input > 15)
      {
        outputBinToDisplay(B01001001, segPins);
        Serial.print("Invalid integer.\n");
      }
      else
      {
        writeDecToSegPins(input, segPins);
      }

      // Asks user for an input
      Serial.print("Enter an integer from 0 to 15\n");
      // Resets time
      prevInputTime = millis();
    }

    timeSinceInput = millis() - prevInputTime;
    // Checks to see if idle time is 4 seconds
    if(timeSinceInput > 4000)
    {
      resetSequence();
    }

  }
}

// Function displays value on display from input
void writeDecToSegPins(int decimal, int* segPins)
{

  // Checks for the value of input
  if(decimal < 0 || decimal > 15)
  {
    return;
  }

  static int binSegOutput[16] =
  {
    // List of values for the display
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01100111, // 9
    B01110111, // A
    B01111100, // b
    B00111001, // C
    B01011110, // d
    B01111001, // E
    B01110001  // F
  };

  outputBinToDisplay(binSegOutput[decimal], segPins);


// Checks value for decimal point
if(decimal > 9)
{
  digitalWrite(SEGMENT_H, LOW);
} else
{
  digitalWrite(SEGMENT_H, HIGH);
}

}

// function outputs value to display
void outputBinToDisplay(int bin, int* segPins)
{
  int binShift = B00000001;

  for(int i=0; i < numPins; i++){

      digitalWrite(segPins[i], !(bin & binShift) > 0);
      binShift = binShift << 1;
  }
}

// Function blinks to DP on display
void resetSequence()
{
  clearDisplay();
  Serial.print("\n");
  Serial.print("Timeout. Application will reset in:");
  Serial.print("\n");


  // Starts countdown on serial monitor
  for(int i=5; i > 0; i-- )
  {
    Serial.print(i);
    Serial.print("...");
    Serial.print("\n");
    digitalWrite(SEGMENT_H, LOW);
    delay(500);
    digitalWrite(SEGMENT_H, HIGH);
    delay(500);

  }

  Serial.print("\n\n");
  // Watch dog timer reset
  wdt_enable(WDTO_15MS);
  delay(100);
}

// Clears display
void clearDisplay()
{
  digitalWrite(SEGMENT_A, HIGH);
  digitalWrite(SEGMENT_B, HIGH);
  digitalWrite(SEGMENT_C, HIGH);
  digitalWrite(SEGMENT_D, HIGH);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, HIGH);
  digitalWrite(SEGMENT_H, HIGH);
}
