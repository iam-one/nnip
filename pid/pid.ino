#include <SoftwareSerial.h>
#include "Wire.h"
#include "MPU6050_tockn.h"

MPU6050 mpu(Wire);

const int dcPin[4] = {5, 6, 9, 10};

const double Kp = 21.0;
const double Ki = 0.8;
const double Kd = 140.0;

const double desire = 1.0;

unsigned long int currentTime, preTime = 0;

double error, preError, valP, valI, valD, output = 0.0;

double avgAngleX = 0.0;

double getOutput(double input, float time){
  error = desire - input;

  valP = Kp * error;
  valI += Ki * error * time;
  valD = Kd * error / time;

  return valP + valI + valD;
}

long map(long x, long in_min, long in_max, long out_min, long out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup(){
  Serial.begin(115200);

  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);

  Serial.println("...Start.");

  pinMode(dcPin[0], OUTPUT);
  pinMode(dcPin[1], OUTPUT);
  pinMode(dcPin[2], OUTPUT);
  pinMode(dcPin[3], OUTPUT);

  analogWrite(dcPin[0], 0);
  analogWrite(dcPin[1], 0);
  analogWrite(dcPin[2], 0);
  analogWrite(dcPin[3], 0);

  preTime = 0;
  currentTime = millis();
}

void loop(){
  for (int x; x < 10; x++){
    mpu.update();
    avgAngleX += mpu.getAngleX();
  }
  avgAngleX = avgAngleX / 10.0;

  output = getOutput(-1 * avgAngleX, float(currentTime - preTime) / 1000) / 392 * 7;
  Serial.println(output);
  if (avgAngleX > 2 || avgAngleX < -2){
    if (output > 0){
      analogWrite(dcPin[0], output);
      analogWrite(dcPin[1], 0);
      analogWrite(dcPin[2], output);
      analogWrite(dcPin[3], 0);
    } else{
      analogWrite(dcPin[0], 0);
      analogWrite(dcPin[1], -1 * output);
      analogWrite(dcPin[2], 0);
      analogWrite(dcPin[3], -1 * output);
    }
  } else {
    analogWrite(dcPin[0], 0);
    analogWrite(dcPin[1], 0);
    analogWrite(dcPin[2], 0);
    analogWrite(dcPin[3], 0);
  }

  preTime = currentTime;
  currentTime = millis();
}