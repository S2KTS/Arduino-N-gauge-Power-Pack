#include <avr/io.h>

#define motorPin1 A0
#define motorPin2 A1
#define PWMPin 10

#define ledPin 0

#define startDuty 0.0 // モーターの始動Duty比 1 - 100,000 で指定
#define stopDuty 0.0 // モーターの停止Duty比 1 - 100,000 で指定 startDutyよりは低いはず
#define ledDuty 0.0 // LED点灯Duty比 startDuty比より小さい方が良い(じゃないと走りだす) 1 - 100,000 で指定

boolean useLED = false; // LEDを使うときはtrue

/***
startDuty: 発車するするギリギリのDuty比
stopDuty: 停車するDuty比
ledDuty: 要見直し
***/

long frq = 440;
int delayTime = 10;

int flg = 1;

unsigned long spd = 1; // min 1 max 100,000

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(PWMPin, OUTPUT);
  setMotorMode(1);

  pinMode(ledPin, INPUT);
}

void setMotorMode(int mode) {
  boolean mtr1;
  boolean mtr2;

  /**
  1: 正転, 2: 逆転, 3:ブレーキ, それ以外: 惰行
  **/
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

void loop() {

  /** 加速して減速を繰り返すだけのテストプログラム **/
  if (flg == 1) {
    // 加速
    spd++;
    if (spd == 100000 - 1) {
      flg = 0;
    }
  } else {
    // 減速
    spd--;
    if (spd == 1) {
      flg = 1;
    }
  }

  frq = getFreq(spd);
  if (frq < 15) frq = 15; // 誤動作防止

  // 解説とかは頑張ってまとめた http://blog.kts.jp.net/archives/18

  // モード指定
  TCCR1A = B00100001;
  TCCR1B = B00010010;
  
  // TOP値
  OCR1A = (unsigned int) (1000000 / frq); // 10^6

  // モーターの起動Duty比
  float adjustDuty = (flg == 1) ? startDuty : stopDuty;
  /** 三項演算子 下記と同義
    float adjustDuty;
    if (flg == 1) {
      adjustDuty = startDuty;
    } else {
      adjustDuty = stopDuty;
    }
  **/
  float duty = adjustDuty + (100000.0 - adjustDuty) * spd / 100000.0; // 10^5

  if (useLED && duty < ledDuty && digitalRead(ledPin) == HIGH) {
    // ledPin が HIGHの場合 でかつ duty が ledDuty 以下の場合
    duty = ledDuty;
  }

  // Duty比 ≒ 速度
  OCR1B = (unsigned int) (1000000 / frq * (duty / 100000.0));

  delayMicroseconds(delayTime);
}

long getFreq(int int_spd) {

 /*** 解説的な
 引数 int_spd を基に周波数をreturnする関数
 非同期モードはよく考えて周波数を求める式を作る
 同期モードはint_spd * 同期パルス数 / 1000.0 で簡単に求められる(".0"をつけないと正しく計算されないので注意)
 ***/

  /*
  // E209
  if (int_spd < 5 * 1000) {
    return 114;
  } else if (int_spd < 9 * 1000) {
    return int_spd * 57.0 / 1000.0;
  } else if (int_spd < 20 * 1000) {
    return int_spd * 33.0 / 1000.0;
  } else if (int_spd < 30 * 1000) {
    return int_spd * 21.0 / 1000.0;
  } else if (int_spd < 45 * 1000) {
    return int_spd * 9.0 / 1000.0;
  } else if (int_spd < 70 * 1000) {
    return int_spd * 3.0 / 1000.0;
  } else {
    return int_spd / 1000.0;
  }
  //*/

  /*
  // E231 墜落インバータ
  if (int_spd < 25 * 1000) {
    return 1050;
  } else if (int_spd < 50 * 1000) {
    // 1050 -> 700
    return 1050 - (350 * (int_spd - 25000) / 25000);
  } else if (int_spd < 60 * 1000) {
    // 700 -> 1800
    return 700 + (1100 * (int_spd - 50000) / 10000);
  } else if (int_spd < 65 * 1000) {
    return int_spd * 3.0 / 1000.0;
  } else {
    return int_spd / 1000.0;
  }
  //*/
  
  /*
  // E233
  if (int_spd < 10 * 1000 && flg == 0) { //減速時のみ
    return 200;
  } else if (int_spd * 1000 < 18) {
    return 525;
  } else if (int_spd < 38 * 1000) {
    return 525 + (375 * (int_spd - 18000) / 20000);
  } else if (int_spd < 47 * 1000) {
    return int_spd * 9.0 / 1000.0;
  } else if (int_spd < 50 * 1000) {
    return int_spd * 3.0 / 1000.0;
  } else {
    return int_spd / 1000.0;
  }
  //*/

  /*
  // 東急5177F
  if (int_spd < 15 * 1000 && flg == 0) { //減速時のみ
    return 200;
  } else if (int_spd < 30 * 1000) {
    return 560;
  } else if (int_spd < 65 * 1000) {
    // 560 -> 700
    return 560 + (140 * (int_spd - 30000) / 35000);
  } else if (int_spd < 80 * 1000) {
    // 700 -> 1800
    return 700 + (1100 * (int_spd - 65000) / 15000);
  } else if (int_spd < 90 * 1000) {
    return int_spd * 3.0 / 1000.0;
  } else {
    return int_spd / 1000.0;
  }
  //*/

  /*
  // 東急9000
  if (int_spd < 5 * 1000) {
    return 200;
  } else if (int_spd < 7 * 1000) {
    return int_spd * 45.0 / 1000.0;
  } else if (int_spd < 13 * 1000) {
    return int_spd * 27.0 / 1000.0;
  } else if (int_spd < 24 * 1000) {
    return int_spd * 15.0 / 1000.0;
  } else if (int_spd < 30 * 1000) {
    return int_spd * 9.0 / 1000.0;
  } else if (int_spd < 36 * 1000) {
    return int_spd * 5.0 / 1000.0;
  } else if (int_spd < 45 * 1000) {
    return int_spd * 3.0 / 1000.0;
  } else {
    return int_spd / 1000.0;
  }
  //*/

  //*
  // 京急1000 ドレミファインバーター
  // 本当はもっと同期モード数たくさんある
  if (int_spd < 4 * 1000) {
    return 349;
  } else if (int_spd < 5 * 1000) {
    return 392;
  } else if (int_spd < 6 * 1000) {
    return 440;
  } else if (int_spd < 7 * 1000) {
    return 466;
  } else if (int_spd < 8 * 1000) {
    return 523;
  } else if (int_spd < 9 * 1000) {
    return 587;
  } else if (int_spd < 10 * 1000) {
    return 622;
  } else if (int_spd < 11 * 1000) {
    return 698;
  } else if (int_spd < 30 * 1000) {
    return 784;
  } else if (int_spd < 35 * 1000) {
    return int_spd * 33.0 / 1000.0;
  } else if (int_spd < 40 * 1000) {
    return int_spd * 21.0 / 1000.0;
  } else if (int_spd < 55 * 1000) {
    return int_spd * 9.0 / 1000.0;
  } else if (int_spd < 65 * 1000) {
    return int_spd * 3.0 / 1000.0;
  } else {
    return int_spd / 1000.0;
  }
  //*/
}
