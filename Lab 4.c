// Lab 4
// Analog to digital converter


// WDT
#include <avr/wdt.h>
// Sets potenMetereniometer to pin A3
#define potenMeter A3

// Sets up input pin, and asks for user input
void setup()
{
  pinMode(potenMeter, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.print("Board has been reset\n");
  Serial.print("Enter 'c' to start conversions: \n");
}

// Main function
void loop()
{
  String userInput;
  // WDT reset
  wdt_enable(WDTO_4S);

  while(1)
  {
    // Checking for input
    if(Serial.available() > 0)
    {
      wdt_reset();
      userInput = Serial.readString();

      // Checks if input is larger than 1 character
      if(userInput.length() > 1)
      {
        Serial.print("Invalid input. Your input was too long.\n");
      }
      // Checks if input is not equal to c
      else if(userInput != "c")
      {
        Serial.print("Invalid input. You entered the wrong character.\n");
      }
      else
      {
        // Runs when input is c
        wdt_disable();
        // Sets converting to run 30 times
        convert(30);
        clearSerial();
        wdt_enable(WDTO_4S);
      }
      // Asks for user input
      Serial.print("Please enter a 'c' to start a set of conversions: \n");
      // Resets WDT
      wdt_reset();
    }
  }

}

// Converting function
void convert(int conversions)
{
  Serial.print("\n");
  Serial.print("Starting a set of conversions:");
  Serial.print("\n");

  //Initalizing variables
  int reading = 0;
  unsigned long readTime = 0;
  unsigned long totalTime = 0;

  // loop that displays converting data
  for(int i=1; i <= conversions; i++)
  {
    readTime = micros();
    reading = analogRead(potenMeter);
    readTime = micros() - readTime;
    Serial.print("#");
    Serial.print(i);
    Serial.print(":\tdigital value = ");
    Serial.print(reading, HEX);
    Serial.print("\tTime = ");
    Serial.print(readTime);
    Serial.print(" usecs");
    Serial.print("\n");
    totalTime = totalTime + readTime;
    delay(500);
  }
  // Finds the average time for all conversions
  Serial.print("\n");
  Serial.print("avg conversion time = ");
  Serial.print(totalTime/conversions);
  Serial.print(" usecs");
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("\n");

}

// Clears that serial buffer
void clearSerial()
{
  if(Serial.available() > 0)
  {
    Serial.readString();
  }
}
