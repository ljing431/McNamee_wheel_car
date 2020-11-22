//The sample code for driving one way motor encoder
const byte encoderA = 2;//A pin -> the interrupt pin 0
const byte encoderB = 3;//B pin -> the digital pin 3
byte encoderALast;
int duration;//the number of the pulses
boolean Direction;//the rotation direction 
 
 
void setup()
{  
  Serial.begin(9600);//Initialize the serial port
  EncoderInit();//Initialize the module
}
 
void loop()
{
  Serial.print("Pulse:");
  Serial.println(duration);
  duration = 0;
  delay(100);
}
 
void EncoderInit()
{
  Direction = true;//default -> Forward  
  pinMode(encoderB,INPUT);  
  attachInterrupt(0, wheelSpeed, CHANGE);
}
 
void wheelSpeed()
{
  int Lstate = digitalRead(encoderA);
  if((encoderALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoderB);
    if(val == LOW && Direction)
    {
      Direction = false; //Reverse
    }
    else if(val == HIGH && !Direction)
    {
      Direction = true;  //Forward
    }
  }
  encoderALast = Lstate;
 
  if(!Direction)  duration++;
  else  duration--;
}
