#include <Arduino.h>

#define X_PULSE_PIN 15 //pin for pulsing step in the x axis
#define Y_PULSE_PIN 14 //pin for pulsing step in the y axis

#define X_ENABLE_INV 3 //inverted pin for enableing x axis
#define X_ENABLE_PIN 2 //pin for enableing x axis

#define Y_ENABLE_INV 8 //inverted pin for enableing y axis
#define Y_ENABLE_PIN 9 //pin for enableing y axis

#define X_DIR_INV 6 //invered pin for setting x axis direction
#define X_DIR_PIN 7 //pin for setting x axis direction

#define Y_DIR_INV 11 //invered pin for setting y axis direction
#define Y_DIR_PIN 12 //pin for setting y axis direction

#define FIRE_ENABLE 16 //not currently working but enables fireing
#define FIRE 17 //pin to fire the cannon

#define STEP_DELAY 4
#define NUM_OF_STEPS 800

String inputString = ""; // A string to hold incoming data

enum axis{
  x,
  y
};

void setup()
{
  Serial.begin(9600);
  pinMode(X_PULSE_PIN, OUTPUT);
  pinMode(X_ENABLE_INV, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_DIR_INV, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);

  pinMode(Y_PULSE_PIN, OUTPUT);
  pinMode(Y_ENABLE_INV, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_DIR_INV, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);

  pinMode(FIRE_ENABLE, OUTPUT);
  pinMode(FIRE, OUTPUT);

  // Set the enable pins to off state initially
  digitalWrite(X_ENABLE_INV, HIGH);
  digitalWrite(X_ENABLE_PIN, LOW);

  digitalWrite(Y_ENABLE_INV, HIGH);
  digitalWrite(Y_ENABLE_PIN, LOW);

  digitalWrite(FIRE_ENABLE, HIGH);
  digitalWrite(FIRE, LOW);

  Serial.println("Enter a number of steps:");
}

void enableMotor(axis a)
{
  if(a == axis::x)
  {
    digitalWrite(X_ENABLE_INV, HIGH);
    digitalWrite(X_ENABLE_PIN, LOW);
  } else if (a == axis::y){
    digitalWrite(Y_ENABLE_INV, HIGH);
    digitalWrite(Y_ENABLE_PIN, LOW);
  }
}

void disableMotor(axis a)
{
  if(a == axis::x)
  {
    digitalWrite(X_ENABLE_INV, LOW);
    digitalWrite(X_ENABLE_PIN, HIGH);
  } else if (a == axis::y){
    digitalWrite(Y_ENABLE_INV, LOW);
    digitalWrite(Y_ENABLE_PIN, HIGH);
  }
  
}

void moveMotor(axis a, int steps)
{
  enableMotor(a);

  if (steps > 0)
  {
    Serial.println("SET direction cw");
    if(a == axis::x){
      digitalWrite(X_DIR_INV, LOW);
      digitalWrite(X_DIR_PIN, HIGH);
    } else if(a == axis::y){
      digitalWrite(Y_DIR_INV, LOW);
      digitalWrite(Y_DIR_PIN, HIGH);
    }
  }
  else
  {
    Serial.println("SET direction ccw");
    if(a == axis::x){
      digitalWrite(X_DIR_INV, HIGH);
      digitalWrite(X_DIR_PIN, LOW);
    } else if(a == axis::y){
      digitalWrite(Y_DIR_INV, HIGH);
      digitalWrite(Y_DIR_PIN, LOW);
    }
  }
  for (int i = 0; i < abs(steps); i++)
  {
    if(a == axis::x){
      digitalWrite(X_PULSE_PIN, HIGH);
      delay(STEP_DELAY);
      digitalWrite(X_PULSE_PIN, LOW);
      delay(STEP_DELAY);
    } else if(a == axis::y){
      digitalWrite(Y_PULSE_PIN, HIGH);
      delay(STEP_DELAY);
      digitalWrite(Y_PULSE_PIN, LOW);
      delay(STEP_DELAY);
    }
    
  }

  disableMotor(a);
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

        int stepsX = x.toInt();
        int stepsY = y.toInt();
        if (stepsX != 0)
        {
          moveMotor(axis::x, stepsX);
        }
        if(stepsY != 0){
          moveMotor(axis::y, stepsY);
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