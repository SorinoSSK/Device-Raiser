#include <Servo.h>

#define maxStrCnt   4

// Initialise Pin
#define servoPin    9

// Initialise Servo
Servo myservo;          // create servo object to control a servo
int servoPos    = 0;    // variable to store the servo position

// Global Variable
String readStr  = "";
char readChar[maxStrCnt];

int readCnt     = 0;
int LF_Byte     = 10;
int maxPos      = 90;
int minPos      = 0;

void setup() 
{
    // Serial
    Serial.begin(9600);
    // Servo
    myservo.attach(servoPin);
    // Others
    memset(readChar, 0, sizeof(readChar));
    Serial.println("Device Raiser");
    Serial.println("Enter SM0 or SM1 to control the servo motor:");
}

void loop() 
{
    readSerialInput();
    rotateServoMotor();
}

void readSerialInput()
{
    if (Serial.available() > 0)
    {
        int readByte = Serial.read();
        if (readByte == LF_Byte)
        {
            readCnt = 0;
            readStr = String(readChar);
            memset(readChar, 0, sizeof(readChar));
            Serial.println("Value Read: " + readStr);

            if (readStr == "SM0" || readStr == "SM1")
            {
                Serial.println("SYS: " + readStr);
            }
            else
            {
                readStr = "";
                Serial.println("ERR: Invalid Input");
            }
        }
        else
        {
            if (readCnt < maxStrCnt-1)
            {
                readChar[readCnt] = (char) readByte;
                readCnt += 1;
            }
            else
            {
                readCnt = 0;
                memset(readChar, 0, sizeof(readChar));
            }
        }
    }
    else
    {
        // Nothing to be processed
    }
}

void rotateServoMotor()
{
    if (readStr == "SM0" && servoPos < maxPos)
    {
        servoPos+=1;
        myservo.write(servoPos);
        Serial.println(servoPos);
        delay(100);
    }
    else if (readStr == "SM1" && servoPos > minPos)
    {
        servoPos-=1;
        myservo.write(servoPos);
        Serial.println(servoPos);
        delay(100);
    }
    else
    {
        // Nothing to be processed
    }
}