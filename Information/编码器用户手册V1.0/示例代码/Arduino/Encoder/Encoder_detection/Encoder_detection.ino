/************************************
主控驱动板为：Arduino UNO
电机驱动：L298N
*************************************/


#define ENCODEROUTPUT 12 // 每次旋转插入电机编码器输出脉冲(cpr)

#define encoderA 2       //external interrupt

#define DIR1 4      // DIR pin 4
#define DIR2 5      // DIR pin 5
#define PWM 6       // PWM pin 6

int Val = 255;       //电机占空比 0-255;
int rpm = 0;
int interval = 1000;
long previousMillis = 0;
long currentMillis = 0;
volatile long encoderValue = 0;

void setup()
{
  Serial.begin(9600); // 波特率
  pinMode(encoderA, INPUT_PULLUP); // 将霍尔传感器A设置为输入上拉
  pinMode(PWM, OUTPUT);
  pinMode(DIR1, OUTPUT); 
  pinMode(DIR2, OUTPUT);
  // 在每个上升信号上在霍尔传感器A处附加中断
  attachInterrupt(digitalPinToInterrupt(encoderA), updateEncoder, RISING);
}

void loop()
{
  digitalWrite(DIR1,HIGH);
  digitalWrite(DIR2,LOW);
  analogWrite(PWM,Val);
  //每秒更新RPM值
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    // 每分钟转数（RPM）= （编码器总脉冲数/电机编码器输出）x 60s 
    rpm = (float)(encoderValue * 60 / ENCODEROUTPUT);

    Serial.print(Val);
    Serial.print('\t');
    Serial.print(encoderValue);
    Serial.print(" pulse / ");
    Serial.print(ENCODEROUTPUT);
    Serial.print(" pulse per rotation x 60 seconds = ");
    Serial.print(rpm);
    Serial.println(" RPM");  //电机转速
    encoderValue = 0;
  }
}

void updateEncoder()
{
  encoderValue++;
}
