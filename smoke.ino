#include<LiquidCrystal_I2C.h>
#include<Servo.h>
#include<wire.h>

servo myservo
LiquidCrystel_I2C lcd(0x27,16,2);
int redLED      = 13;   // pin set to 13 
int greenLED    = 12; // pin set to 12
int buzzer      = 8;    // pin set to 8
int smokeAO     = A0;  // pin set to A0
int angle       = 0;
int sensorvalue = 0;
float voltageoutput;
float Rs;
float Ro;
int b1,b2,b3,m1,m2,m3;  // b and m represent constant and slopes of gases(graph)
int threshold;

void setup()
{
    myservo.attach(3);  // servo attached to pin 3 of arduino
    lcd.init();
    lcd.backlight();
    lcd.print("Gas Detector");
    pinmode(redLED,OUTPUT);
    pinmode(greenLED,OUTPUT);
    pinmode(buzzer,OUTPUT);
    pinmode(smokeAO,INPUT);
    serial.begin(9600);  // serial communication begins at 9600 bits per second

    sensorvalue = analogRead(smokeAO);
    voltageoutput = sensorvalue*(5/1023); //converting analog to votage form
    Rs = (5 - voltageoutput)/voltageoutput ;
    Ro = Rs/9.8 // calculation of Ro
    serial.begin(9600)
}

double ppm(float a,float b,float m,float calc)
{   
    return pow(a,((calc-b)/m))
}

void loop()
{
    sensorvalue   = analogRead(smokeAO);
    voltageoutput = sensorvalue*(5/1023);
    Rs            = (5 - voltageoutput)/voltageoutput ;
    float ratio   = Rs/Ro;
    float calc    = log10(ratio);
    float a       = 10.0;
    ppm1          = ppm(a,b1,m1,calc);  // for CO
    ppm2          = ppm(a,b2,m2,calc);  // for smoke
    ppm3          = ppm(a,b3,m3,calc);  // for LPG
    lcd.clear();
    lcd.setcursor(0,0);
    lcd.print("CO");
    lcd.setcursor(3,0);
    lcd.print("smoke");
    lcd.setcursor(9,0);
    lcd.print("LPG");
    lcd.setcursor(0,1);
    lcd.print("ppm1");
    lcd.setcursor(4,1);
    lcd.print("ppm2");
    lcd.setcursor(9,1);
    lcd.print("ppm3");
    delay(100);

    if (ppm1 >= 200 || ppm2 >= 350 || ppm3 >=800)  //conditions
    {
        digitalWrite(redLED,HIGH); //red LED on
        digitalWrite(greenLED,LOW); //green LED off
        tone(buzzer,1000); // buzzer on

        if (angle == 0)
        {
            for(angle = 0; angle <= 180. angle++)
            {
                myservo.write(angle); // servo rotates
                delay(15);
            }
        }

    }
    delay(1000);

    else
    {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        notone(buzzer);
        if (angle != 0)
        {
            for(angle = 180; angle >= 0; angle--)
            {
                myservo.write(angle);  // servo rotates
                delay(15);
            }
        }

    }
    delay(1000);

    














}
