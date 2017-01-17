#include <avr/io.h>

#define motorPin1 A0
#define motorPin2 A1
#define PWMPin 10

#define ledDuty 4

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(PWMPin, OUTPUT);
  setMotorMode(1);
}

void setMotorMode(int mode) {
  boolean mtr1;
  boolean mtr2;

  switch(mode) {
    case 1:
      mtr1 = HIGH;
      mtr2 = LOW;
      break;
    case 2:
      mtr1 = LOW;
      mtr2 = HIGH;
      break;
    case 3:
      mtr1 = HIGH;
      mtr2 = HIGH;
      break;
    default:
      mtr1 = LOW;
      mtr2 = LOW;
  }
  
  digitalWrite(motorPin1, mtr1);
  digitalWrite(motorPin2, mtr2);
}

long frq = 440;
//boolean flg = true;
int delayTime = 10;

int flg = 1;

unsigned long spd = 1; //min 1 max 100,000

void loop() {

  if (flg == 1) {
    spd++;
    if (spd == 100000 - 1) {
      flg = 0;
    }
  } else {
    spd--;
    if (spd == 1) {
      flg = 1;
    }
  }

  int spd2 = spd / 1000.0; //min 1 max 100

  /*
  // E209
  if (spd2 < 5) {
    frq = 114;
  } else if (spd2 < 9) {
    frq = spd * 57.0 / 1000.0;
  } else if (spd2 < 20) {
    frq = spd * 33.0 / 1000.0;
  } else if (spd2 < 30) {
    frq = spd * 21.0 / 1000.0;
  } else if (spd2 < 45) {
    frq = spd * 9.0 / 1000.0;
  } else if (spd2 < 70) {
    frq = spd * 3.0 / 1000.0;
  } else {
    frq = spd / 1000.0;
  }

  frq = frq * 1.8;
  //*/

  /*
  // E231 墜落インバータ
  if (spd2 < 25) {
    frq = 1050;
  } else if (spd2 < 50) {
    // 1050 -> 700
    frq = 1050 - (350 * (spd - 25000) / 25000);
  } else if (spd2 < 60) {
    // 700 -> 1800
    frq = 700 + (1100 * (spd - 50000) / 10000);
  } else if (spd2 < 65) {
    frq = spd * 3.0 / 1000.0;
  } else {
    frq = spd / 1000.0;
  }

  frq = frq * 2.0;
  //*/
  
  /*
  // E233
  if (spd2 < 10 && flg == 0) { //減速時のみ
    frq = 200;
  } else if (spd2 < 18) {
    frq = 525;
  } else if (spd2 < 38) {
    frq = 525 + (375 * (spd - 18000) / 20000);
  } else if (spd2 < 47) {
    frq = spd * 9.0 / 1000.0;
  } else if (spd2 < 50) {
    frq = spd * 3.0 / 1000.0;
  } else {
    frq = spd / 1000.0;
  }

  frq = frq * 2.5;
  //*/

  /*
  // 東急5177F
  if (spd2 < 15 && flg == 0) { //減速時のみ
    frq = 200;
  } else if (spd2 < 30) {
    frq = 560;
  } else if (spd2 < 65) {
    // 560 -> 700
    frq = 560 + (140 * (spd - 30000) / 35000);
  } else if (spd2 < 80) {
    // 700 -> 1800
    frq = 700 + (1100 * (spd - 65000) / 15000);
  } else if (spd2 < 90) {
    frq = spd * 3.0 / 1000.0;
  } else {
    frq = spd / 1000.0;
  }

  frq = frq * 1.4;
  //*/

  /*
  // 東急9000
  if (spd2 < 5) {
    frq = 200;
  } else if (spd2 < 7) {
    frq = spd * 45.0 / 1000.0;
  } else if (spd2 < 13) {
    frq = spd * 27.0 / 1000.0;
  } else if (spd2 < 24) {
    frq = spd * 15.0 / 1000.0;
  } else if (spd2 < 30) {
    frq = spd * 9.0 / 1000.0;
  } else if (spd2 < 36) {
    frq = spd * 5.0 / 1000.0;
  } else if (spd2 < 45) {
    frq = spd * 3.0 / 1000.0;
  } else {
    frq = spd / 1000.0;
  }

  frq = frq * 3.0;
  //*/

  //*
  // 京急1000 ドレミファインバーター
  // 本当はもっと同期モード数たくさんある
  if (spd2 < 4) {
    frq = 349;
  } else if (spd2 < 5) {
    frq = 392;
  } else if (spd2 < 6) {
    frq = 440;
  } else if (spd2 < 7) {
    frq = 466;
  } else if (spd2 < 8) {
    frq = 523;
  } else if (spd2 < 9) {
    frq = 587;
  } else if (spd2 < 10) {
    frq = 622;
  } else if (spd2 < 11) {
    frq = 698;
  } else if (spd2 < 30) {
    frq = 784;
  } else if (spd2 < 35) {
    frq = spd * 33.0 / 1000.0;
  } else if (spd2 < 40) {
    frq = spd * 21.0 / 1000.0;
  } else if (spd2 < 55) {
    frq = spd * 9.0 / 1000.0;
  } else if (spd2 < 65) {
    frq = spd * 3.0 / 1000.0;
  } else {
    frq = spd / 1000.0;
  }

  frq = frq * 2;
  //*/

  float dutyTest = 0.0;

  float spd3 = (spd2 + dutyTest) * 100.0 / (100.0 + dutyTest); 

  TCCR1A = B00110001;
  TCCR1B = B00010001;

  // TOP
  OCR1A = (word)(8000000 / frq);
  // duty
  float num = 100.0 / spd2;
  OCR1B = (word)(8000000 / frq - 8000000 / frq / num);
  delayMicroseconds(delayTime);
}
