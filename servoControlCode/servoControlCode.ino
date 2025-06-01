#include <Servo.h>

#define maxStrCnt   4
#define maxPos      110
#define minPos      30

// Initialise Pin
#define servoPin    9

// Initialise Servo
Servo myservo;              // create servo object to control a servo
int servoPos    = maxPos;   // variable to store the servo position

// Global Variable
String readStr  = "";
char readChar[maxStrCnt];

int readCnt     = 0;
int LF_Byte     = 10;
int targetPos   = maxPos;
int steps       = 1;
bool isStepDone = false;

void setup() 
{
    // Serial
    Serial.begin(9600);
    // Servo
    myservo.attach(servoPin);
    myservo.write(servoPos);
    // Others
    memset(readChar, 0, sizeof(readChar));
    Serial.println("===============Device Raiser===============");
    Serial.println("Enter SM0 or SM1 to control the servo motor");
    Serial.println("===========================================");
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
            if (readStr == "SM0" || readStr == "SM1")
            {
                targetPos = (readStr == "SM0") ? maxPos : minPos;
                steps = (targetPos > servoPos) ? 1 : -1;
                Serial.println("OK");
            }
            else
            {
                readStr = "";
                Serial.println("ERR");
            }
        }
        else
        {
            if (readCnt < maxStrCnt-1)
            {
                readChar[readCnt++] = (char) readByte;
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
    if (servoPos != targetPos)
    {
        servoPos += steps;
        myservo.write(servoPos);
        isStepDone = (servoPos == targetPos);
        delay(15);
    }
    else
    {
        if (isStepDone)
        {
            Serial.println("DONE");
            isStepDone = false;
        }
        else
        {
            // Nothing to be processed
        }
    }
}