#include <Ps3Controller.h>

//Right motor
int enableRightMotor=22; 
int rightMotorPin1=16;
int rightMotorPin2=17;
//Left motor
int enableLeftMotor=23;
int leftMotorPin1=18;
int leftMotorPin2=19;

int rSpeed = 0;
int lSpeed = 0;

bool doubleSpeed = false;

#define MAX_MOTOR_SPEED 200

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightPWMSpeedChannel = 4;
const int leftPWMSpeedChannel = 5;

// void notify()
// {
//   int yAxisValue =(Ps3.data.analog.stick.ly);  //Left stick  - y axis - forward/backward car movement
//   int xAxisValue =(Ps3.data.analog.stick.rx);  //Right stick - x axis - left/right car movement

//   if (yAxisValue <= -50)       //Move car Forward
//   {
//     rotateMotor(MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
//   }
//   else if (yAxisValue >= 50)   //Move car Backward
//   {
//     rotateMotor(-MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED);
//   }
//   else if (xAxisValue >= 50)  //Move car Right
//   {
//     rotateMotor(-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED);
//   }
//   else if (xAxisValue <= -50)   //Move car Left
//   {
//     rotateMotor(MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED);
//   }
//   else                                      //Stop the car
//   {
//     rotateMotor(0, 0);
//   } 
// }


void drive(){
  if(Ps3.data.analog.stick.ry < -20 || Ps3.data.analog.stick.ry > 20){
    rSpeed = map(Ps3.data.analog.stick.ry, -127, 127, -100, 100);
  }else{
    rSpeed = 0;
  }
  if(Ps3.data.analog.stick.ly < -20 || Ps3.data.analog.stick.ly > 20){
    lSpeed = map(Ps3.data.analog.stick.ly, -127, 127, -100, 100);
  }else{
    lSpeed = 0;
  }
  if(Ps3.event.button_down.r2){
    doubleSpeed = true;
  }
  if(Ps3.event.button_up.r2){
    doubleSpeed = false;
  }
  if(doubleSpeed){
    rSpeed *= 2; lSpeed *= 2;
  }
  Serial.print(lSpeed);
  Serial.print("    ");
  Serial.println(rSpeed);
  rotateMotor(rSpeed, lSpeed);
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  
  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }  
  ledcWrite(rightPWMSpeedChannel,abs(rightMotorSpeed));
  ledcWrite(leftPWMSpeedChannel, abs(leftMotorSpeed));
}

void setUpPinModes()
{
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  //Set up PWM for motor speed
  ledcSetup(rightPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, rightPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftPWMSpeedChannel);  
  
  rotateMotor(0, 0);
}


void setup()
{
  setUpPinModes();
  Serial.begin(115200);
  Ps3.attach(drive);
  Ps3.begin("c0:49:ef:91:c6:82");
  Serial.println("Ready.");
}

void loop()
{
}
