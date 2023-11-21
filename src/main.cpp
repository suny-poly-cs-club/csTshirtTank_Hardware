#include <Arduino.h>

#define PULSE_PIN 15 // LED connected to pin 13

#define ENABLE_INV 3 // LED connected to pin 13
#define ENABLE_PIN 2 // LED connected to pin 13

#define DIR_INV 6 // LED connected to pin 13
#define DIR_PIN 7 // LED connected to pin 13

#define FIRE_ENABLE 16
#define FIRE 17

#define STEP_DELAY 4
#define NUM_OF_STEPS 800

String inputString = ""; // A string to hold incoming data

void setup()
{
  Serial.begin(9600);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(ENABLE_INV, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIR_INV, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(FIRE_ENABLE, OUTPUT);
  pinMode(FIRE, OUTPUT);

  // Set the enable pins to off state initially
  digitalWrite(ENABLE_INV, HIGH);
  digitalWrite(ENABLE_PIN, LOW);

  digitalWrite(FIRE_ENABLE, HIGH);
  digitalWrite(FIRE, LOW);

  Serial.println("Enter a number of steps:");
}

void enableMotor()
{
  digitalWrite(ENABLE_INV, HIGH);
  digitalWrite(ENABLE_PIN, LOW);
}

void disableMotor()
{
  digitalWrite(ENABLE_INV, LOW);
  digitalWrite(ENABLE_PIN, HIGH);
}

void moveMotor(int steps)
{
  enableMotor();

  if (steps > 0)
  {
    Serial.println("SET direction cw");
    digitalWrite(DIR_INV, LOW);
    digitalWrite(DIR_PIN, HIGH);
  }
  else
  {
    Serial.println("SET direction ccw");
    digitalWrite(DIR_INV, HIGH);
    digitalWrite(DIR_PIN, LOW);
  }
  for (int i = 0; i < abs(steps); i++)
  {
    digitalWrite(PULSE_PIN, HIGH);
    delay(STEP_DELAY);
    digitalWrite(PULSE_PIN, LOW);
    delay(STEP_DELAY);
  }

  disableMotor();
}

void split(arduino::String input, arduino::String &x, arduino::String &y){
  x = "";
  y = "";
  bool encounteredDelimiter = false;
  for(int i = 0; i < input.length(); i++){
    if(input[i] == ':'){
      encounteredDelimiter = true;
    } else{
      if(!encounteredDelimiter)
        x = x + input[i];
      else
        y = y + input[i];
    }
  }
}

void loop()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read(); // Read a character
    Serial.print(inChar);              // Echo back the character
    if (inChar == '\n')
    {
      //here is where we parse the input should be #ofsteps:#ofsteps or fire
      Serial.println(inputString);
      const char FIRESTRING = 'f';
      Serial.println(inputString[0]);
      if(inputString[0] == FIRESTRING){
        //here is where we fire the gun on command
        Serial.println("fireing");
        digitalWrite(FIRE, HIGH);
        delay(100);
        digitalWrite(FIRE, LOW);
        inputString = "";
      }else{
        Serial.println("moving");
        arduino::String x;
        arduino::String y;

        split(inputString, x, y);

        int steps = x.toInt(); // Convert the string to an integer
        if (steps != 0)
        {
          moveMotor(steps);
        }
        inputString = ""; // Clear the string for new input
        Serial.println("Enter input:");
      }
    }
    else
    {
      inputString += inChar; // Add the character to our string
    }
  }
}