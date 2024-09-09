
//モーター部分

#define drate 1
byte dutycounter;
// ストップ
void stop(){
  digitalWrite(front, LOW);
  digitalWrite(back, LOW);
}
// 前進
void FrontRotate(){
  digitalWrite(front, HIGH);
  digitalWrite(back, LOW);
}
// 後進
void BackRotate(){
  digitalWrite(front, LOW);
  digitalWrite(back,HIGH);
}
void duty_front(){
  dutycounter++;
  if(dutycounter==dcycle_front){
    dutycounter=0;
  }
  if(dutycounter<drate){
    digitalWrite(front, HIGH);
  }else{
    digitalWrite(front, LOW);
  }
}
void duty_back(){
  dutycounter++;
  if(dutycounter == dcycle_back){
    dutycounter = 0;
  }
  if(dutycounter<drate){
    digitalWrite(back, HIGH);
  }else{
    digitalWrite(back, LOW);
  }
}
