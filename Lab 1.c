

#define MEGA_LED 13
#define BOARD_LED 6

enum state
{
off, on
};

state LEDSTATE = off;

void setup()
{
  // Sets the outputs for both boards
  // Prints initial input command
  pinMode(MEGA_LED, OUTPUT);
  pinMode(BOARD_LED, OUTPUT);
  Serial.begin(9600);
  Serial.print("To begin press 'g'. Input 's' to stop. Any other input will not work.\n");
}

void loop()
{
  if(Serial.available() > 0)
  {
    String userInput;
    userInput = Serial.readString();

    if(userInput.length() == 1)
    {

      if(userInput == "g")
      {
        // When input is g, set LEDSTATE on
        Serial.print("------------------------------------\n");
        Serial.print("Turning LEDs on. \n");
        Serial.print("Input 'g' to continue blinking, input 's' to stop.\n");
        Serial.print("------------------------------------\n");
        LEDSTATE = on;
      }
      else if(userInput == "s")
      {
        // When input is s, set LEDSTATE off
        Serial.print("------------------------------------\n");
        Serial.print("Turning LEDs off. \n");
        Serial.print("Input 's' to remain off, or 'g' to start.\n");
        Serial.print("------------------------------------\n");
        LEDSTATE = off;
      }
      else
      {
        // Displays error when input is not g or s
        Serial.print("------------------------------------\n");
        Serial.print("Error, not a valid input, try again.\n");
        Serial.print("Input 'g' to start or 's' to stop.\n");
        Serial.print("------------------------------------\n");
      }
    }
    else
    {
      // Displays error if input is longer than 1 character
      Serial.print("------------------------------------\n");
      Serial.print("Error ,not a valid input, try again.\n");
      Serial.print("Input 'g' to start or 's' to stop.\n");
      Serial.print("------------------------------------\n");
    }
  }

  if(LEDSTATE == on)
  {

    //Sets MEGA_LED on and BOARD_LED
    //off for 2 seconds
    digitalWrite(MEGA_LED, HIGH);
    digitalWrite(BOARD_LED, LOW);
    delay(2000);

    //Sets BOARD_LED on and MEGA_LED
    //off for 1 second
    digitalWrite(MEGA_LED, LOW);
    digitalWrite(BOARD_LED, HIGH);
    delay(1000);
  }

if(LEDSTATE == off)
{

   // Sets both leds off, when
   // LEDSTATE is off
   digitalWrite(MEGA_LED, LOW);
   digitalWrite(BOARD_LED, LOW);
}

}
