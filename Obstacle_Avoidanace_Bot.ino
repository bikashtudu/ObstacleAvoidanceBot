#include <AFMotor.h>
#include <Servo.h>

#define trigPin 13
#define echoPin 10
#define servoPin 2
#define MoveBotFwd 1
#define FindBotDir 2
#define ChangeBotDir 3
#define RIGHT_TURN 1
#define LEFT_TURN 0
#define dist_threshold 20
#define init_angle 30
#define final_angle 150
#define delta 20
#define max_rpm 100
#define curr_speed 105 
AF_DCMotor motor1(1,MOTOR12_64KHZ); // set up motors.
AF_DCMotor motor2(2, MOTOR12_8KHZ); 
 


Servo servo;
int sound = 250;

int rotation_angle;
int max_angle, curr_angle;
float curr_dist, max_dist = 0;

int TODO = MoveBotFwd , PREVTODO =-1;

float GetDistance(int );

void setup() 
{
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  motor1.setSpeed(curr_speed);
  motor2.setSpeed (curr_speed);
  }
  
  
void loop() {
  int t_angle=0, curr_rpm=0, t_delay=0;
  switch(TODO)
  {
    case MoveBotFwd: //Move Bot Forward
        curr_dist = GetDistance(90);
        if (curr_dist < dist_threshold)
        {
          PREVTODO = TODO;
          motor1.run(RELEASE);
          motor2.run(RELEASE); 
          TODO = FindBotDir;
          curr_angle = init_angle;
        }
        else
        {
          motor1.run(FORWARD);
          motor2.run(FORWARD);
          PREVTODO = TODO;
        }
        break;
     case FindBotDir: //Find Suitable Direction
        while (curr_angle <= final_angle)
        {
          curr_dist = GetDistance(curr_angle);
          if( max_dist < curr_dist )
          {
            max_angle = curr_angle;
            max_dist = curr_dist;
          }
          curr_angle = curr_angle + delta;
        }
        rotation_angle = max_angle;
        PREVTODO = TODO;
        TODO = ChangeBotDir;
        break;
     case ChangeBotDir: //Change Bot Direction...
        t_angle = abs(90 - rotation_angle);
        curr_rpm = (curr_speed/255)*max_rpm;
        t_delay = t_angle / (2*3.14*curr_rpm);
        motor_config = (rotation_angle< 90) ? RIGHT_TURN : LEFT_TURN;
      
        if(motor_config == RIGHT_TURN)   
        {
          motor1.run(BACKWARD);
          motor2.run(RELEASE);
        }
        if(motor_config == LEFT_TURN)
        {
          motor1.run(RELEASE);
          motor2.run(BACKWARD);
        }
        delay(t_delay);
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        PREVTODO = TODO;
        TODO = MoveBotFwd;
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
      switch(PREVTODO)
      {
        case MoveBotFwd:
          Serial.println("BOT moved forward...");
          Serial.print("\tCurrent Distance: ");
          Serial.println(curr_dist);
        case FindBotDir:
          Serial.println("BOT searched direction...");
          Serial.print("\tSelected Angle: ");
          Serial.print(curr_angle);
          Serial.print("\tMax Distance Distance: ");
          Serial.println(curr_dist);
        case ChangeBotDir:
          Serial.println("BOT changed direction...");
        default:
          Serial.println("BOT is confused...");
        }
}

