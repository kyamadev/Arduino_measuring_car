//decoder
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;

#define TPin 18 
#define EPin 23
#define RPin 35
#define BUZZER_PIN 25
const int front = 4; //GPIO4
const int back = 2; //GPIO0
int ct=0;
enum Unit{CM,INCH,SUN};
Unit currentUnit = CM;
//21,22はi2c通信

//music
//音を鳴らす時間
#define BEAT 500
//音階名と周波数の対応
#define B3 246.942//シ
#define C4 261.6//ド
#define C#4 277.18//#ド
#define D4 293.665//レ
#define D#4 311.127//#レ
#define E4 329.628//ミ
#define F4 349.228//ファ
#define F#4 369.994//#ファ
#define G4 391.995//ソ
#define G#4 415.305//#ソ
#define A4 440//ラ
#define A#4 466.164//#ラ
#define B4 493.883//シ
#define C5 523.251//ド
#define R0 0//給付

//inout
byte numbers[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000};
double counter1 = 100.0;
double storedValue;//センサに送る数値[cm]
int selectedDigit = 0;  // 0: hundreds, 1: tens, 2: ones, 3: tenths
unsigned long lastUpdateTime = 0;
bool isBlinkOn = false;

int buttonAPin = 26;
int buttonBPin = 27;
int buttonCPin = 14;
int buttonDPin = 12;
int segmentPins[] = {9,13,4,6,7,10,0,5};// A, B, C, D, E, F, G, DP
int digitPins[] ={8,11,12,14};// 1st, 2nd, 3rd, 4th digits
int ledPins[] = {3,2,1};//CM,INCH,SUN

//モータ設定
// #include <MsTimer2.h> 
const byte dutydata[]={1, 2, 4, 8, 16, 32, 0xff}; //スピードの候補
byte counter;
// 前進の際の速度
byte dcycle_front;
// 後進の際の速度
byte dcycle_back;

void setup() {
//センサピン
  Serial.begin(115200);
  pinMode(TPin, OUTPUT);
  pinMode(EPin, INPUT);
  pinMode(RPin, INPUT);
  digitalWrite(TPin, LOW);
//モータピン
  pinMode(front, OUTPUT);
  pinMode(back, OUTPUT);
  counter=0;
  //音楽
  ledcSetup(1,12000, 8);
  ledcAttachPin(BUZZER_PIN,1);
  //decoder
  Wire.begin();
  Wire.setClock(1700000);
  
  mcp.begin_I2C();
  for (int i = 0; i < 15; i++) {
    mcp.pinMode(i,OUTPUT) ;
    mcp.digitalWrite(segmentPins[i], LOW);
  }
  for (int i = 0; i < 4; i++) {
    mcp.digitalWrite(digitPins[i], LOW);
  }
  pinMode(12, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
}
/*
double UC(int u,double a){
  double an=a;
  switch(u){
    //インチ
    case 1:an/=0.39370;break;
    //寸
    case 2:an/=3.0303;break;
    //フィート
    case 3:an/=30.48;break;
    //スパン
    case 4:an/=15.24;break;    
    default:;
  }
  return(an);
}*/
void setduty(){
  //1,2,4,8,16,32,0xff 値が小さいほど回転スピードが速くなった
  dcycle_front = dutydata[2];// dutydata[x] xの値を変えることでスピードを変える。
  dcycle_back = dutydata[2];// dutydata[x] xの値を変えることでスピードを変える。
}

double move(double a) {
  setduty();
  double ans=Emes();
  double num=a;
  double b=num*0.05;
  if(ans>(num+b)){
    duty_back();
    if(ct>0)ct--;
  }
  else if(ans<(num-b)){
    duty_front();
    if(ct>0)ct--;
  }
  else{
    stop();
    if(ct<20)ct++;
    else if(ct==20){
      //seijya();
      ct=0;
      return(0);
    }
  }
  delay(1);
  return(move(num));
}

void loop(){
  double num1=nyuuryoku();
  move(num1);
  delay(1);
  if(currentUnit == CM){
    rondon();
  }
  else if(currentUnit == INCH){
    seijya ();
  }
  else if(currentUnit == SUN ){
    furusato();
  }
}
