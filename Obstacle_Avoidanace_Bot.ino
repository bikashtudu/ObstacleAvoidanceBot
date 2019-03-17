#include <AFMotor.h>
#include <Servo.h>

#define trigPin 13
#define echoPin 10
#define servoPin 2
#define MoveBotFwd 1
#define FindBotDir 2
#define ChangeBotDir 3

#define dist_threshold 20
#define init_angle 30
#define final_angle 150
#define delta 20
AF_DCMotor motor1(1,MOTOR12_64KHZ); // set up motors.
AF_DCMotor motor2(2, MOTOR12_8KHZ); 
 


Servo servo;
int sound = 250;

int rotation_angle;
int max_angle, curr_angle;
float curr_dist, max_dist = 0;

int TODO = MoveBotFwd;

float GetDistance(int );

void setup() 
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  motor1.setSpeed(105);
  motor2.setSpeed (105);
  }
  
  
void loop() {
  switch(TODO)
  {
    case MoveBotFwd: //Move Bot Forward
        curr_dist = GetDistance(90);
        if (curr_dist < dist_threshold)
        {
          TODO = FindBotDir;
          curr_angle = init_angle;
        }
        else
        {
          motor1.run(FORWARD);
          motor2.run(FORWARD);
        }
        break;
     case FindBotDir: //Find Suitable Direction 
        if (curr_angle <= final_angle)
        {
          curr_dist = GetDistance(curr_angle);
          if( max_dist < curr_dist )
          {
            max_angle = curr_angle;
            max_dist = curr_dist;
          }
          curr_angle = curr_angle + delta;
        }
        else
        {
          rotation_angle = max_angle;
          TODO = ChangeBotDir;
        }
        break;
     case ChangeBotDir: //Change Bot Direction...
        break;
  }
 monitor_print();
 delay(60);
 }

float GetDistance(int angle)
{
  
    long duration, distance;
    servo.write(angle);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
      
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    return distance;
}

void monitor_print()
{
      Serial.print("Status : ");
      switch(TODO)
      {
        case MoveBotFwd:
          Serial.println("BOT moving forward...");
          Serial.print("\tCurrent Distance: ");
          Serial.println(curr_dist);
        case FindBotDir:
          Serial.println("BOT is searching a direction...");
          Serial.print("\tCurrent Angle: ");
          Serial.print(curr_angle);
          Serial.print("\tCurrent Distance: ");
          Serial.println(curr_dist);
        case ChangeBotDir:
          Serial.println("BOT is changing direction...");
        default:
          Serial.println("BOT is confused...");
        }
}

