#include <Servo.h>

#define maxStrCnt   4
#define maxPos      115
#define minPos      50

// Initialise Pin
#define servoPin    9

// Initialise Servo
Servo myservo;              // create servo object to control a servo
int servoPos    = maxPos;   // variable to store the servo position

// Global Variable
String readStr  = "";
char readChar[maxStrCnt];

int readCnt     = 0;
int targetPos   = maxPos;
int steps       = 1;
char LF_Byte     = '\n';
bool isStepDone = false;

void setup() 
{
    // Serial
    Serial.begin(115200);
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
            readStr.trim();
            memset(readChar, 0, sizeof(readChar));
            if (readStr == "SM0" || readStr == "SM1")
            {
                targetPos = (readStr == "SM0") ? maxPos : minPos;
                steps = (targetPos > servoPos) ? 5 : -5;
                Serial.println("OK");
            }
            else
            {
                Serial.println("ERR: " + readStr);
                readStr = "";
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
                // Let Reading Bypass
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
    if (servoPos != targetPos && isStepsWithinBoundary())
    {
        servoPos += steps;
        myservo.write(servoPos);
        isStepDone = (servoPos == targetPos || !isStepsWithinBoundary());
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

bool isStepsWithinBoundary()
{
    return (servoPos + steps) < maxPos && (servoPos + steps) > minPos;
}
