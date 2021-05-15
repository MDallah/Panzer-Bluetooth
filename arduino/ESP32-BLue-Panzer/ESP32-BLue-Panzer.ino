//https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/examples/SerialToSerialBT/SerialToSerialBT.ino
//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <Servo.h>
#include <string>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define R1 27
#define R2 26

#define L1 33
#define L2 32

#define Leds 17
#define Fire 16

//static const int RSpeed = 22;
//static const int LSpeed = 5;

const int ledPin1 = 22;  // 16 corresponds to GPIO1
const int ledPin2 = 5;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000;
const int ledChannel = 2;
const int ledChannel1 = 3;
const int resolution = 8;


static const int servoRLpin = 19;
static const int servoUDpin = 18;

int minSpeed = 100;
int maxSpeed = 200; //maximal. 255
int noSpeed = 0;

int posRL = 60;
int posUD = 60;
Servo servoRL;
Servo servoUD;


BluetoothSerial SerialBT;

bool sup , sdn , slf , srt;

void setup() {
  Serial.begin(115200);

  pinMode(Leds, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  //  pinMode(RSpeed, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  //  pinMode(LSpeed, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(ledChannel1, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin1, ledChannel);
  ledcAttachPin(ledPin2, ledChannel1);

  servoRL.attach(servoRLpin);
  servoUD.attach(servoUDpin);

  servoRL.write(60);
  servoUD.write(60);

  slf = srt = sdn = sup = false;

  SerialBT.begin("Panzer48"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!\n");
}

void loop() {
  if (sup) ServoUp();
  else if (sdn) ServoDown();
  else ServoStop();

  if (slf) ServoLeft();
  else if (srt) ServoRight();
  else ServoStop();
  //SerialBT.flush();
  if (SerialBT.available()) {
    //  value = SerialBT.readString();
    //    Serial.write(value);

    String value = "";
    char c = SerialBT.read();
    while (c != '\n') {
      value += c;
      c = SerialBT.read();
    }

    Serial.println("V: " + value);
    //----------------------------------------------
    //----------------------------------------------
    if ( value.startsWith("L")) {
      if (digitalRead(Leds) == LOW)
      {
        digitalWrite(Leds, HIGH);
        Serial.println("LEDs ON");
      }
      else
      {
        digitalWrite(Leds, LOW);
        Serial.println("LEDs OFF");
      }
    }
    //----------------------------------------------
    //----------------------------------------------
    else if ( value.startsWith("F") ) {
      digitalWrite(Fire, HIGH);
      Serial.println("Fire ON");
      delay(250);
      digitalWrite(Fire, LOW);
      Serial.println("Fire OFF");
    }
    //----------------------------------------------
    //----------------------------------------------
    else if ( value.startsWith("M") ) {
      int x = value.indexOf('/');
      //Serial.println("x: " + x);
      int M_MoveValue = value.substring(1, x).toInt();
      int M_MovePull = value.substring(x + 1, value.length()).toInt();
      //Serial.println("M-MoveValue: " + M_MoveValue);
      //Serial.println("M-MovePull: " + M_MovePull);
      //----------------------------------------------
      if (M_MovePull > 6) {
        //----------------------------------------------
        if ((M_MoveValue >= 0 && M_MoveValue <= 45) ||
            (M_MoveValue >= 315 && M_MoveValue <= 360)) {
          forward();
          Serial.println("forward");
        }
        //----------------------------------------------
        if (M_MoveValue >= 135 && M_MoveValue <= 225) {
          backward();
          Serial.println("backward");
        }
        //----------------------------------------------
        if (M_MoveValue > 45 && M_MoveValue <= 90) {
          forwardRight();
          Serial.println("forwardRight");
        }
        //----------------------------------------------
        if (M_MoveValue >= 270 && M_MoveValue < 315) {
          forwardLeft();
          Serial.println("forwardLeft");
        }
        //----------------------------------------------
        if (M_MoveValue > 90 && M_MoveValue < 135) {
          backwardRight();
          Serial.println("backwardRight");
        }
        //----------------------------------------------
        if (M_MoveValue > 225 && M_MoveValue < 270) {
          backwardLeft();
          Serial.println("backwardLeft");
        }
        //----------------------------------------------
        
      } else {
        Stop();
        Serial.println("Stop");
      }
    }
    //----------------------------------------------
    //----------------------------------------------

    else if ( value.startsWith("S") ) {
      int y = value.indexOf('/');
      //Serial.println("y: " + String(y));
      int S_MoveValue = value.substring(1, y).toInt();
      int S_MovePull = value.substring(y + 1, value.length()).toInt();
      // Serial.println("S-MoveValue: " + String(S_MoveValue));
      // Serial.println("S-MovePull: " + String(S_MovePull));
      if (S_MovePull > 6) {
        //----------------------------------------------
        if ((S_MoveValue >= 0 && S_MoveValue <= 45) ||
            (S_MoveValue >= 315 && S_MoveValue <= 360)) {
          sup = true ;
        }
        //----------------------------------------------
        if (S_MoveValue >= 135 && S_MoveValue <= 225) {
          sdn = true ;
        }
        //----------------------------------------------
        if (S_MoveValue > 45 && S_MoveValue < 135) {
          srt = true;
        }
        //----------------------------------------------
        if (S_MoveValue > 225 && S_MoveValue < 315) {
          slf = true ;
        }
      }
      else {
        slf = srt = sdn = sup = false;
      }
      //----------------------------------------------
    }


    //SerialBT.flush();
    Serial.println("------------------");

    //delay(20);
  }

}
void forward()
{
  digitalWrite(R1, HIGH);
  digitalWrite(L1, HIGH);
  digitalWrite(R2, LOW);
  digitalWrite(L2, LOW);
  //  analogWrite(RSpeed, maxSpeed);
  //  analogWrite(LSpeed, maxSpeed);
  ledcWrite(ledChannel, maxSpeed);
  delay(50);
  ledcWrite(ledChannel1, maxSpeed);
  delay(50);
}

void backward()
{
  digitalWrite(R1, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(R2, HIGH);
  digitalWrite(L2, HIGH);
  //  analogWrite(RSpeed, maxSpeed);
  //  analogWrite(LSpeed, maxSpeed);
  ledcWrite(ledChannel, maxSpeed);
  delay(50);
  ledcWrite(ledChannel1, maxSpeed);
  delay(50);
}

void left()
{
  digitalWrite(R1, HIGH);
  digitalWrite(L1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L2, HIGH);
  //  analogWrite(RSpeed, minSpeed);
  //  analogWrite(LSpeed, minSpeed);
  ledcWrite(ledChannel, minSpeed);
  delay(50);
  ledcWrite(ledChannel1, minSpeed);
  delay(50);
}

void right()
{
  digitalWrite(R1, LOW);
  digitalWrite(L1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(L2, LOW);
  //  analogWrite(RSpeed, minSpeed);
  //  analogWrite(LSpeed, minSpeed);
  ledcWrite(ledChannel, minSpeed);
  delay(50);
  ledcWrite(ledChannel1, minSpeed);
  delay(50);
}

void Stop()
{
  digitalWrite(R1, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L2, LOW);
  //  analogWrite(RSpeed, noSpeed);
  //  analogWrite(LSpeed, noSpeed);
}

void forwardRight()
{
  digitalWrite(R1, HIGH);
  digitalWrite(L1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L2, LOW);
  //  analogWrite(RSpeed, maxSpeed);
  //  analogWrite(LSpeed, noSpeed);
  ledcWrite(ledChannel, maxSpeed);
  delay(50);
  ledcWrite(ledChannel1, noSpeed);
  delay(50);
}

void forwardLeft()
{
  digitalWrite(R1, LOW);
  digitalWrite(L1, HIGH);
  digitalWrite(R2, LOW);
  digitalWrite(L2, LOW);
  //  analogWrite(RSpeed, noSpeed);
  //  analogWrite(LSpeed, maxSpeed);
  ledcWrite(ledChannel, noSpeed);
  delay(50);
  ledcWrite(ledChannel1, maxSpeed);
  delay(50);
}

void backwardLeft()
{
  digitalWrite(R1, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(L2, HIGH);
  //  analogWrite(RSpeed, noSpeed);
  //  analogWrite(LSpeed, maxSpeed);
  ledcWrite(ledChannel, noSpeed);
  delay(50);
  ledcWrite(ledChannel1, maxSpeed);
  delay(50);
}

void backwardRight()
{
  digitalWrite(R1, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(R2, HIGH);
  digitalWrite(L2, LOW);
  //  analogWrite(RSpeed, maxSpeed);
  //  analogWrite(LSpeed, noSpeed);
  ledcWrite(ledChannel, maxSpeed);
  delay(50);
  ledcWrite(ledChannel1, noSpeed);
  delay(50);
}

void ServoDown()
{
  posUD--;
  if (posUD < 30) {
    posUD = 30;
  }
  servoUD.write(posUD);
  delay(25);
  Serial.println("UD: " + String(posUD));
  Serial.println("------------------");
}

void ServoUp()
{
  posUD++;
  if (posUD > 80) {
    posUD = 80;
  }
  servoUD.write(posUD);
  delay(25);
  Serial.println("UD: " + String(posUD));
  Serial.println("------------------");
}

void ServoLeft()
{
  posRL++;
  if (posRL > 120) {
    posRL = 120;
  }
  servoRL.write(posRL);
  delay(25);
  Serial.println("RL: " + String(posRL));
  Serial.println("------------------");
}

void ServoRight()
{
  posRL--;
  if (posRL < 0) {
    posRL = 0;
  }
  servoRL.write(posRL);
  delay(25);
  Serial.println("RL: " + String(posRL));
  Serial.println("------------------");

}

void ServoStop()
{
  servoUD.write(posUD);
  servoRL.write(posRL);
  delay(25);
}
