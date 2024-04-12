#include <AFMotor.h>
#include <Servo.h>              // Add library
            // Define any servo name
Servo servoLook;  
// Create motor objects for M1, M2, M3, and M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
// defines pins numbers

byte maxDist = 150;                               //Maximum sensing distance (Objects further than this distance are ignored)
                               //Minimum distance from an object to stop in cm
float timeOut = 2*(maxDist+10)/100/340*1000000; 
byte stopDist = 25;     

const int trig = A2;
const int echo = A5;

// defines variables
long duration;
int distance;

void setup() {
 
  pinMode(trig, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo, INPUT); // Sets the echoPin as an Input
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Motor control example");
  servoLook.attach (10);    
 
  motor1.setSpeed(240); // Full speed 255
  motor2.setSpeed(240);
  motor3.setSpeed(240);
  motor4.setSpeed(240);


}

int getDistance();
void moveForward();
void stopIt();
int checkDirection();
void turnLeft();
void turnRight();
void moveBackward();
void stopit();
void loop() {
//my_servo.write(180);
Serial.print("Distance: ");
Serial.println(distance);

while (true)
 {
  distance = getDistance();
  if (distance > 25)
    { 
        moveForward();
        delay(500);
    }
   else 
  {
    stopIt();
    delay(1000);
    int turnDir;
    turnDir = checkDirection();
    if (turnDir == 0)
    {
      turnLeft();
      delay(700);
      break;
    }
    
    else if (turnDir == 1)
  {
    turnRight();
    delay(700);
    break;
  }
  else if(turnDir == 2)
  {
    moveBackward();
    delay(2000);
    checkDirection();
  
    }
    

  
 }
 }
  

}




  void moveForward()                               
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward()                             
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnRight()                               
{
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}


void turnLeft()                                
{
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void stopIt()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
int getDistance()                                   //Measure the distance to an object
{
  unsigned long pulseTime;                          //Create a variable to store the pulse travel time
  int distance;                                     //Create a variable to store the calculated distance
  digitalWrite(trig, HIGH);                         //Generate a 10 microsecond pulse
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  pulseTime = pulseIn(echo, HIGH, timeOut);         //Measure the time for the pulse to return
  distance = (float)pulseTime * 340 / 2 / 10000;    //Calculate the object distance based on the pulse time
  return distance;
}


int checkDirection() //Check the left and right directions and decide which way to turn
{
  
  stopIt();
  delay(500);
  int right, left;
  servoLook.write(0); // Turn servo to look left
  delay(1000);
  left = getDistance();
  
  servoLook.write(180); // Turn servo to look right
  delay(1000);
  right = getDistance();
  
  servoLook.write(90); // Return servo to center position
  delay(1000);
  if (right > left && right > 25) { // Corrected the condition
    return 0; // More space on the right
  } else if (left > right && left > 25) { // Corrected the condition
    return 1; // More space on the left
  } else {
    return 2; // No clear direction, or equal space on both sides
  }
  
}
