// Lab 5
// ADC Conversions

// Inlcuding WatchDog Timer
#include <avr/wdt.h>
// Sets potenMeterenMeter to A0
#define potenMeter 0

// Asks for user for input
void setup()
{
  ADCSRA |= _BV(ADEN);
  Serial.begin(9600);
  Serial.setTimeout(50);
  Serial.print("\nBoard has been reset\n\n");
  Serial.print("Select a conversion to perform, 'a' for lab #4 or 'b' for lab #5 \n");
}

// Main function
void loop()
{
  String userInput;
  wdt_enable(WDTO_4S);

  while(1)
  {
    // Checking for input
    if(Serial.available() > 0)
    {
      wdt_reset();
      userInput = Serial.readString();
      // Checks string length
      if(userInput.length() > 1)
      {
        Serial.print("Error, invalid input. \n");
      }
      // Lab 4 conversions
      else if(userInput == "a")
      {
        wdt_disable();
        analogConvert(30);
        wdt_enable(WDTO_4S);
      }
      // Lab 5 converstions
      else if(userInput == "b")
      {
        wdt_disable();
        pollingConvert(30);
        wdt_enable(WDTO_4S);
      }
      else
      {
        // if input is string length is greater than 1
        Serial.print("Error, invalid input.\n");
      }
      // Clears serial
      flushSerialBuf();
      Serial.print("Select a conversion to perform, 'a' for lab #4 or 'b' for lab #5 \n");
      // WDT reset
      wdt_reset();
    }
  }

}

// Analog converting function
void analogConvert(int totalReads)
{
  Serial.print("Starting conversions using AnalogRead:\n");

  // Initialize values
  int reading = 0;
  unsigned long readTime = 0;
  unsigned long convertTime = 0;
  float avgReadTime = 0;

  for(int i=1; i <= totalReads; i++)
  {
    readTime = micros();
    reading = analogRead(potenMeter);
    // time conversions finish
    readTime = micros() - readTime;
    Serial.print("#");
    Serial.print(i);
    Serial.print(":\t");
    convertResults(reading, readTime);
    convertTime = convertTime + readTime;
    // Delays until values are printed
    delay(350);
  }

  avgReadTime = convertTime/totalReads;
  Serial.print("\navg conversion time = ");
  Serial.print(avgReadTime);
  Serial.print(" usecs\n");
  Serial.print("\n\n");
}

// polling function
void pollingConvert(int totalReads)
{
  // Sets Vref to 5V and port
  ADMUX = B01000000 | potenMeter;
  // Sets ADC control and default prescale
  ADCSRA |= B00000111;
  // For lower 8 bits
  int lowRegister, highRegister = 0;
  // Holds the time for reads
  unsigned long readTime = 0;
  unsigned long convertTime = 0;
  float avgReadTime = 0;

  Serial.print("Starting conversion using polling and port manipulation:\n");
  // Starting ADC conversions
  for(int i=1; i <= totalReads; i++)
  {
    ADCSRA |= _BV(ADSC);
    // Sets read time to micros
    readTime = micros();
    // Checks ADC until done
    while(bit_is_set(ADCSRA, ADSC))
    {

    }
    // Time of conversion
    readTime = micros() - readTime;


    // ADC registers
    lowRegister = ADCL;
    highRegister = ADCH;

    Serial.print("#");
    Serial.print(i);
    Serial.print(":\t");
    convertResults((highRegister << 8)| lowRegister, readTime);
    // Total time for conversions
    convertTime = convertTime + readTime;
    // Stalls for values to print
    delay(350);
  }
  // Displays average time of conversions on serial monitor
  avgReadTime = convertTime/totalReads;
  Serial.print("\navg conversion time = ");
  Serial.print(avgReadTime);
  Serial.print(" usecs\n");
  Serial.print("\n\n");
}

// function to flush serial
void flushSerialBuf()
{
  if(Serial.available() > 0)
  {
    Serial.readString();
  }
}

// function to display conversion results in hex and times.
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
