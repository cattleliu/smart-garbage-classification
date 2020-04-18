#include <Servo.h>
Servo drivServo1;
Servo drivServo2;
Servo doorServo1;
Servo doorServo2;

int drivPos1 = 3600;
int drivPos2 = 3600;
int doorPos1 = 0;
int doorPos2 = 0;
const int buttonPin = 2;
const int limitSwitch = 3;
const int ledPin = 13;
const int buttonPin1 = 4;
int btCount = 0;
int buttonState = 0;
int buttonState1 =0;
int limitCount = 0;
int classState = 0;
int limitTemp = 0;
int i=0;
char predict = '4';
unsigned long time1;
unsigned long time2;
unsigned long time3;
//int servoState =0;
void setup() {
  // put your setup code here, to run once:
  drivServo1.attach(10);
  drivServo2.attach(11);
  doorServo1.attach(6);
  doorServo2.attach(9);
  pinMode(limitSwitch,INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin1,INPUT);
  //attachInterrupt(0,buttonCount,RISING);//计算定位传感器次数
  attachInterrupt(1,limit,FALLING);
  doorServo1.write(90);
  doorServo2.write(0);
  time3 = 0;
  Serial.begin(9600);
  Serial.println("Arduino");//
}

void loop() 
  {
    if ( Serial.available())//判断串口缓存区有没有数据
    {
       predict = Serial.read();//读出的预测值
    }

   switch (predict)
    {
      case '0':
            onePos();
            Serial.println("Arduino");//通知树莓派可以下个循环
            predict = '4';
             
            break;
      case '1':
           twoPos();
           Serial.println("Arduino");//
           predict = '4';
           
           break;
      case '2':
           threePos();
           Serial.println("Arduino");//
           predict = '4';
           
           break;
      case '3':
           fourPos();
           Serial.println("Arduino");
           predict = '4';
            
           break;
    }
    
     
  buttonState = digitalRead(buttonPin);
  buttonState1 = digitalRead(buttonPin1);
 
  if ((buttonState == LOW)||(buttonState1 == LOW))
  {
    digitalWrite(ledPin,HIGH);
    if (true)
     {
      Serial.println("cap");//按钮按下通知树莓派拍照
      delay(2000);
     }
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
   
 

  //buttonCount();
  }
void buttonCount()  //计数中断程序
  {

    delay(5);
   
    
    btCount++;
   
   
  }
void limit()
    {
    
    debounce();
    if (limitTemp == 20)
    {
    limitCount++;
    }
    
    if ((classState == 1)&&(limitCount ==1))
    {
     drivServo1.write(90);
     drivServo2.write(90);
     limitCount = 0; 
     
    }
    else if ((classState ==2)&&(limitCount == 2))
    {
     drivServo1.write(90);
     drivServo2.write(90); 
     limitCount = 0;
     
    }
    limitTemp=0;
    //Serial.print("limitCount");
    //Serial.println(limitCount);
    //Serial.print("classState");
    //Serial.println(classState);
  
    }
void onePos()
    {
            classState = 2; //走到最右边计数器为2
            limitCount = 0;
            drivServo1.write(180);
            drivServo2.write(0);
            delay(3500);            
            doorServo1.write(0); 
            doorServo2.write(90);            
            delay(2000);                                    
            doorServo1.write(90); 
            doorServo2.write(0);       
            delay(1000);       
            drivServo1.write(0);
            drivServo2.write(180);
            delay(3000);               
            //classState = 0;      
    } 


void twoPos()
    {
            classState = 1;
            limitCount = 0;        
            drivServo1.write(180);
            drivServo2.write(0);
            delay(2500);            
            doorServo1.write(0); 
            doorServo2.write(90);            
            delay(2000);                                    
            doorServo1.write(90); 
            doorServo2.write(0);       
            delay(1000);       
            drivServo1.write(0);
            drivServo2.write(180);
            delay(3000);               
           // classState = 0;                               
    }           
void threePos()
    {
            classState = 1;
            limitCount = 0;
            drivServo1.write(0);
            drivServo2.write(180);
            delay(2900);            
            doorServo1.write(0); 
            doorServo2.write(90);            
            delay(2000);                                    
            doorServo1.write(90); 
            doorServo2.write(0);       
            delay(1000);       
            drivServo1.write(180);
            drivServo2.write(0);
            delay(3000);               
            //classState = 0;       
    }            
void fourPos()
    {
            classState = 2;
            limitCount = 0;
            drivServo1.write(0);
            drivServo2.write(180);
            delay(3900);            
            doorServo1.write(0); 
            doorServo2.write(90);            
            delay(2000);                                    
            doorServo1.write(90); 
            doorServo2.write(0);       
            delay(1000);       
            drivServo1.write(180);
            drivServo2.write(0);
            delay(3000);               
            //classState = 0;       
    }    
void debounce()
    {
            for (i=0;i<20;i++)
             {
              delay(10);
              if (digitalRead(limitSwitch)==LOW)
                 {
                   limitTemp++;
                 }
              
             }
             //Serial.print("limitTemp");
             //Serial.println(limitTemp);
             
            
    }
   
