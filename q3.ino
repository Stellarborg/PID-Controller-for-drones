const int fanPin = 9;  //fan
const int trigPin = 4;  //ultrasonic sensor
const int echoPin = 3;  //ultrasonic sensor
const int minDistance = 45; // in cm
const int maxDistance = 55; // in cm
float duration, distance;   
const int kp = 2;  //proportional parameter
const int ki = 5;  //integral parameter
const int kd = 1;  //differential parameter
float setPoint = 50;  //hover drone at 50m
float error,derivative; 
int fanSpeed = 0;
int PWM;
int previousError = 0;
int integral = 0;

float getDistance();

void setup() 
{
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(fanPin, OUTPUT);   
}
   
void loop()
{
  distance = getDistance();
  if (distance >= minDistance && distance <= maxDistance)
  {
    error = setPoint - distance;
    derivative = error - previousError;
    integral += error;
    previousError = error;
    fanSpeed = kp * error + ki * integral + kd * derivative;
    if(distance >= 50)
      {
        PWM = abs(map(fanSpeed,0,76,0,150));
      }
    else
    {
      PWM = abs(map(fanSpeed,0,51,0,150));
    }
     analogWrite(fanPin, PWM);
  }
  else
  {
    PWM = 0;
    analogWrite(fanPin, PWM);
  }
  delay(1000);    
}
//to get distance above ground level
float getDistance() {
   digitalWrite(trigPin, LOW); //send trigger signal
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH);  //calculate duration
   return (duration / 2) / 29.1;  //return distance
}

