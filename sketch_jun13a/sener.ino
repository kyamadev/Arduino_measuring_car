int V = 340;//音速

//Send Trigger pulse
void sendTrigger() {
  digitalWrite(TPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TPin, LOW);
}
double Emes(){
  sendTrigger();
  while (!digitalRead(EPin)) {
  }
  double t1 = micros();
  while (digitalRead(EPin)) {
  }
  double t2 = micros();
  double t = t2 - t1;
  //Serial.print("E:");
  Serial.print(V * t / 20000);
  Serial.print("  ");
  //Serial.print("cm ");
  return(V * t / 20000);
}

double Rmes() {
  int ans;
  double volt;
  double distance;
  ans = analogRead(RPin);
  volt = ans * 5.0 / 4096;
  distance = ( 9.5974 / volt ) - 3.0086 ;
  //Serial.print("R:");
  Serial.println(distance * 100);
  //Serial.println("cm");
  return(distance * 100);
  }
  /*
double kyori(){
  double a=Emes();
  double b=Rmes();
  if(abs(a-b)<10){
    return((a+b)/2);
  }
  else return(a);
}*/
