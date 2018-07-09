// Lab 6
// ADC convertions



// WDT
#include <avr/wdt.h>
// Defining pins
#define potenMeter 0
#define SEG_A 9
#define SEG_B 8
#define SEG_C 7
#define SEG_D 6
#define SEG_E 5
#define SEG_F 4
#define SEG_G 3
#define SEG_H 2
#define numPins 8
unsigned long ISR_Time = 0;

void setup()
{
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_H, OUTPUT);
  // Starts the segment display at 4
  writeDecToPins(4);
  ADCSRA |= _BV(ADEN);
  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.print("\nBoard was reset\n\n");
  // Asks user for input
  Serial.print("Select a type of conversion");
  Serial.print("('a' for lab #4; 'b' for lab #5; 'c' for lab #6) \n");
}


// Continuous loop
void loop()
{

  // initialize varables
  String userInput;
  wdt_enable(WDTO_4S);
  unsigned long timeInput = 0;
  unsigned long oldTime = 0;
  int timeLeft = 4;

  while(1)
  {
    // Checking for input
    if(Serial.available() > 0)
    {
      wdt_reset();
      userInput = Serial.readString();

      // If user input is longer than 1 character
      if(userInput.length() > 1)
      {
        Serial.print("Invalid input. \n");
      }
      // If user input is "a"
      else if(userInput == "a")
      {
        wdt_disable();
        writeDecToPins(4);
        analogConvert(30);
        wdt_enable(WDTO_4S);
      }
      // If user input is "b"
      else if(userInput == "b")
      {
        wdt_disable();
        writeDecToPins(4);
        pollingConvert(30);
        wdt_enable(WDTO_4S);
      }
      // If user input is "c"
      else if(userInput == "c")
      {
        wdt_disable();
        writeDecToPins(4);
        interruptConvert(30);
        wdt_enable(WDTO_4S);
      }
      // If user input is not "a", "b", or "c"
      else
      {
        Serial.print("Invalid input. \n");
      }

      serialFlush();
      Serial.print("Select a type of conversion");
      Serial.print("('a' for lab #4; 'b' for lab #5; 'c' for lab #6) \n");
      wdt_reset();
      oldTime = millis();
    }

    timeLeft = 4 - (millis() - oldTime)/1000;
    writeDecToPins(timeLeft);
  }

}

// Lab 4 Analog converting function
void analogConvert(int converstions)
{
  Serial.print("Starting a set of conversions using AnalogRead:\n");

  // Initialize varables
  int reading = 0;
  unsigned long readTime = 0;
  unsigned long totalTime = 0;
  float avgReadTime = 0;

  // Displays conversion times
  for(int i=1; i <= converstions; i++)
  {
    // Sets time in microseconds
    readTime = micros();
    reading = analogRead(potenMeter);
    // Time for conversion
    readTime = micros() - readTime;
    Serial.print("#");
    Serial.print(i);
    Serial.print(":\t");
    convertResults(reading, readTime);
    totalTime = totalTime + readTime;
    // Delay to allow text to be printed
    delay(350);
  }
  // Print times
  avgReadTime = totalTime/converstions;
  Serial.print("\nAnalogRead avg conversion time = ");
  Serial.print(avgReadTime);
  Serial.print(" usecs\n");
  Serial.print("\n\n");
}

// Lab 5 Polling converting function
void pollingConvert(int converstions)
{
  // Initialize varables
  ADMUX = B01000000 | potenMeter;
  ADCSRA |= B00000111;
  int lowReg, highReg = 0;
  unsigned long readTime = 0;
  unsigned long totalTime = 0;
  float avgReadTime = 0;

  Serial.print("Starting a set of conversions using polling:\n");
  for(int i=1; i <= converstions; i++)
  {
    ADCSRA |= _BV(ADSC);
    readTime = micros();
    // while loop to take up time
    while(bit_is_set(ADCSRA, ADSC))
    {
      // while loop to take up time
    }
    readTime = micros() - readTime;

    // ADC registers
    lowReg = ADCL;
    highReg = ADCH;

    Serial.print("#");
    Serial.print(i);
    Serial.print(":\t");
    convertResults((highReg << 8)| lowReg, readTime);
    totalTime = totalTime + readTime;
    // Delay time for printing
    delay(350);
  }
  // Displays time
  avgReadTime = totalTime/converstions;
  Serial.print("\nPolling avg conversion time = ");
  Serial.print(avgReadTime);
  Serial.print(" usecs\n");
  Serial.print("\n\n");
}

// Lab 6 interrput convertion
void interruptConvert(int converstions)
{
  // Initialize varables
  ADMUX = B01000000 | potenMeter;
  ADCSRA |= B00001111;
  int lowReg, highReg = 0;
  unsigned long totalTime = 0;
  float avgReadTime = 0;

  Serial.print("Starting a set of conversions using interrupts:\n");
  for(int i=1; i <= converstions; i++)
  {
    ADCSRA |= _BV(ADSC);
    ISR_Time = micros();
    while(bit_is_set(ADCSRA, ADSC))
    {
      // While loop to take up time
    }

    // ADC registers
    lowReg = ADCL;
    highReg = ADCH;

    Serial.print("#");
    Serial.print(i);
    Serial.print(":\t");
    convertResults((highReg << 8)| lowReg, ISR_Time);
    totalTime = totalTime + ISR_Time;
    // Delay time for printing
    delay(350);
  }

  ADCSRA &= B10000000;
  // Displays converting times
  avgReadTime = totalTime/converstions;
  Serial.print("\nInterrupt-driven avg conversion time = ");
  Serial.print(avgReadTime);
  Serial.print(" usecs\n");
  Serial.print("\n\n");
}

// ISR function
ISR(ADC_vect)
{
    ISR_Time = micros() - ISR_Time;
}

// Flushing serial
void serialFlush()
{
  if(Serial.available() > 0)
  {
    Serial.readString();
  }
}


// function for printing ADC convertion values and times
void convertResults(int reading, unsigned long timeToRead)
{
  Serial.print("digital value = ");
  if(reading <= 0x00F)
  {
    Serial.print("00");
  }
  else if(reading <= 0x0FF)
  {
    Serial.print("0");
  }
  Serial.print(reading, HEX);
  Serial.print("\tTime = ");
  Serial.print(timeToRead);
  Serial.print(" usecs\n");
}


// funciton for printing to segment display
void writeDecToPins(int decimal)
{

  if(decimal < 0 || decimal > 9)
  {
    return;
  }

  static int segPins[numPins] = {SEG_A,SEG_B,SEG_C,SEG_D,SEG_E,SEG_F,SEG_G,SEG_H};
  static int binSegOutput[10] =
  {

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
  };

  outputBinToDisplay(binSegOutput[decimal], segPins);

}


// Output to segment display
void outputBinToDisplay(int bin, int* segPins)
{
  int binShift = B00000001;

  for(int i=0; i < numPins; i++){

      digitalWrite(segPins[i], !(bin & binShift) > 0);
      binShift = binShift << 1;
  }
}
