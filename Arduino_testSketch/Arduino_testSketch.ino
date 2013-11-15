/* Some sample code for the CLOrk performance shield v002
 * Based on sample codes from Arduino.cc
 * 20130-11-15
 */

const int pingPin = 4;
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;
const int button1 = 2;
const int button2 = A3;
int colorChoice=0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  //RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
 
 //Setup the pullup for buttons 
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
}

void loop()
{
  //Ping init
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  //Ping read data
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print("Distance in cm: ");
  Serial.print(cm);
  Serial.println();
  
  //Cycle some colors
  colorChoice=cycleColor(colorChoice);

  //The buttons
  Serial.print("Button1: ");
  Serial.println(digitalRead(button1), DEC);
  Serial.print("Button2: ");
  Serial.println(digitalRead(button2), DEC);
  
  
  delay(500);
}


////// helper functions

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int cycleColor(int colll){
  switch(colll){
   case 0:
  setColor(255, 0, 0);  // red
  break;
   case 1:
  setColor(0, 255, 0);  // green
  break;
   case 2:
  setColor(0, 0, 255);  // blue
  break;
   case 3:
  setColor(255, 255, 0);  // yellow
  break;
   case 4:
  setColor(80, 0, 80);  // purple
  break;
   case 5:
  setColor(0, 255, 255);  // aqua
  break;
  }
  colll++;
  if (colll>5)colll=0;
  return colll;
  
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
