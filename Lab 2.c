// Lab 2
// This program will blink two LEDs, and count with a toggleswitch,
// and display the count on a display


// Defines the 2 LEDs
#define BOARD_LED 13
#define MEGA_LED 11
// Defines the Switch
#define SWITCH_INPUT 2
// Defines all the inputs for display
#define SEGMENT_A 3
#define SEGMENT_B 4
#define SEGMENT_C 5
#define SEGMENT_D 6
#define SEGMENT_E 7
#define SEGMENT_F 8
#define SEGMENT_G 9
#define SEGMENT_H 10

void setup() {
  // Sets pins to be input or outsputs
  pinMode(BOARD_LED, OUTPUT);
  pinMode(MEGA_LED, OUTPUT);
  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  pinMode(SEGMENT_H, OUTPUT);
  pinMode(SWITCH_INPUT, INPUT);
  Serial.begin(9600);
  // Initial prompt on serial monitor
  // Gives format of values for display
  Serial.print("The seven segment display the number of switch changes  \n");
  Serial.print("When the decimal indicater is on, the values shown are");
  Serial.print("numeric values and when it is off, the values shown are ");
  Serial.print("alphabetic value \n");
  Serial.print(" 0 = 0 \n 1 = 1 \n 2 = 2 \n 3 = 3 \n 4 = 4 \n 5 = 5 \n 6 = 6 \n");
  Serial.print(" 7 = 7 \n 8 = 8 \n 9 = 9 \n 10 = A \n 11 = B \n 12 = C \n ");
  Serial.print("13 = D \n 14 = E \n 15 = F \n");
  Serial.print("\n");
}




void loop()
 {

 // Data Types
  enum state
   {
    off,
    on
  };

  // Initial variables
  state LEDSTATE = digitalRead(SWITCH_INPUT);
  bool changeState = false;
  int toggles = 0;
  int numPins[8] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F,
  SEGMENT_G, SEGMENT_H};
  int totalCount = 0;

  // initialize 7-seg display
  writeDecTonumPins(0, numPins);

  while(1)
  {
   //check state of toggle Switch
   if(digitalRead(SWITCH_INPUT) == HIGH)
    {
      if(LEDSTATE == off)
      {
       changeState = true;
      }
      LEDSTATE = on;
    }
    else{
      if(LEDSTATE == on)
      {
       changeState = true;
      }
      LEDSTATE = off;
    }


    // Checks for switch input, then updates display
    if(changeState)
    {
      (toggles < 15) ? toggles++: toggles = 0;
      Serial.print("count = (decimal) ");
      Serial.print(toggles);
      Serial.print("  (hex) ");
      Serial.print(toggles, HEX);
      Serial.print(" Total count: ");
      Serial.print(totalCount);
      Serial.print("\n");
      Serial.print("Decimal point off indicates numeric character, on");
      Serial.print(" indicates alphabeticial. ");
      Serial.print("\n");
      Serial.print("\n");

      totalCount++;
      writeDecTonumPins(toggles, numPins);
      changeState = false;
    }


     // Sets the LEDs to be on for 1 second each, out of phase
    if(LEDSTATE == on)
    {
      digitalWrite(BOARD_LED, HIGH);
      digitalWrite(MEGA_LED, LOW);
      delay(1000);
      digitalWrite(BOARD_LED, LOW);
      digitalWrite(MEGA_LED, HIGH);
      delay(1000);
    }

    // Turn off both leds
    if(LEDSTATE == off)
    {
      digitalWrite(BOARD_LED, LOW);
      digitalWrite(MEGA_LED, LOW);
    }
  }
}


void writeDecTonumPins(int decimal, int* numPins)
{
  // Checks the decimal value
  if(decimal < 0 || decimal > 15)
  {
    return;
  }

  // Stores the binary values into an array
  int binaryOutput[16] =
  {
    // values in binary
    B0111111, // 0
    B0000110, // 1
    B1011011, // 2
    B1001111, // 3
    B1100110, // 4
    B1101101, // 5
    B1111101, // 6
    B0000111, // 7
    B1111111, // 8
    B1100111, // 9
    B1110111, // A
    B1111100, // b
    B0111001, // C
    B1011110, // d
    B1111001, // E
    B1110001  // F
  };

int i;
// Sets the DP to high
if(decimal > 9)
{
  digitalWrite(SEGMENT_H, LOW);
} else
{
  digitalWrite(SEGMENT_H, HIGH);
}

  int binaryShift = B0000001;
  // Binary shift
  for(int i=0; i < 7; i++)
  {
      // Checks if value is greater than 0
      digitalWrite(numPins[i], !(binaryOutput[decimal] & binaryShift) > 0);
      binaryShift = binaryShift << 1;
  }


}
