/* Some heartSample code for the CLOrk performance shield v002
 * Based on heartSample codes from Arduino.cc
 * 20130-11-15
 *
 * Docu: http://clork.isnot.in/wiki/index.php?title=Arduino_Shield
 *
 * Libraries required:
 * compass library http://www.seeedstudio.com/wiki/File:Digital_Compass.zip
 * encoder library http://www.seeedstudio.com/wiki/File:Encoder.zip
 * timerone library http://www.seeedstudio.com/wiki/File:TimerOne.zip
 */
 
#include <Wire.h>
#include <math.h>
#include <Encoder.h>
#include <TimerOne.h>
#include <HMC5883L.h>

//Setting up pinout
const int pingPin     = 4;
const int redPin      = 3;
const int greenPin    = 5;
const int bluePin     = 6;
const int button1Pin  = 2;
const int button2Pin  = A3;
const int tempSensorPin  = 0;
const int heartSensorPin = 12;

//Other variables
int colorChoice=0;

byte heartheartoldheartSample, heartSample;

float temperature;
int B=3975;          //B value of the thermistor

HMC5883L compass;                  // Store our compass as a variable.
float declinationAngle = -0.0457;  // set this up for the Compass -- http://www.magnetic-declination.com/

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  //Start I2C
  Wire.begin(); // Start the I2C interface.
  
  // setup timer
  encoder.Timer_init();
  
  //RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
 
  //Setup the pullup for buttons 
  pinMode(button1Pin,INPUT_PULLUP);
  pinMode(button2Pin,INPUT_PULLUP);
  
  //Setup heart sensor
  pinMode (heartSensorPin, INPUT);  //Signal pin to input  
  
  //Setup Compass
  compass.setScale(1.3); // Set the scale of the compass.
  compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
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
  Serial.print("button1Pin: ");
  Serial.println(digitalRead(button1Pin), DEC);
  Serial.print("button2Pin: ");
  Serial.println(digitalRead(button2Pin), DEC);
  
  //Heart -- NB the delay of the loop should be really short if you want this to be reliable and not skip beats
  heartSample = digitalRead(heartSensorPin);  //Store signal output 
  if (heartSample && (heartoldheartSample != heartSample)) {
    Serial.println("Beat!");
  }
  heartoldheartSample = heartSample;           //Store last signal received 
  
  //Temp
  int a=analogRead(tempSensorPin);
  float resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
  temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
  
  Serial.print("Current temperature is ");
  Serial.println(temperature);
  
  //Encoder
    if (encoder.rotate_flag ==1){
      if (encoder.direct==0){
        Serial.println("counter-clockwise");
      }else{
        Serial.println("clockwise");}
    encoder.rotate_flag =0;
  }
  
  //Compass
  // Get scaled values
  MagnetometerScaled scaled = compass.readScaledAxis();
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
  heading += 2*PI;
  
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
  heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  Serial.print("Heading:\t");
  Serial.print(heading);
  Serial.print(" Radians   \t");
  Serial.print(headingDegrees);
  Serial.println(" Degrees   \t");
  
  delay(100);
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
