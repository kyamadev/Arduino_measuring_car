void test(){
  Serial.print("counter1=");
  Serial.print(counter1);
  Serial.print(" storedValue=");
  Serial.print(storedValue);
}
double nyuuryoku() {
  if (millis() - lastUpdateTime > 1000) {
    lastUpdateTime = millis();
    isBlinkOn = !isBlinkOn;
  }
  if (digitalRead(buttonAPin) == LOW) {
    delay(1);  // Debounce
    while(digitalRead(buttonAPin) == LOW);  // Wait for button release
    selectedDigit = (selectedDigit + 1) % 4;
    ledcWriteTone(1,C4);
    delay(500);
    ledcWriteTone(1,R0);
  }
  if (digitalRead(buttonBPin) == LOW) {
    delay(1);  // Debounce
    while(digitalRead(buttonBPin) == LOW);  // Wait for button release
    incrementDigit();
    ledcWriteTone(1,C4);
    delay(500);
    ledcWriteTone(1,R0);
  }
  if(digitalRead(buttonCPin) == LOW){
    delay(1);  // Debounce
    while(digitalRead(buttonCPin) == LOW);  // Wait for button release
    currentUnit = (Unit)((currentUnit + 1) % 3);
    ledcWriteTone(1,D4);
    delay(500);
    ledcWriteTone(1,R0);
  }
  if (digitalRead(buttonDPin) == LOW) {
    delay(1);
    while (digitalRead(buttonDPin) == LOW);  // Debounce
    switch(currentUnit){
      case CM:
        storedValue = counter1;
      break;
      case INCH:
        storedValue = counter1*2.54;
      break;
      case SUN:
        storedValue = counter1*3.03;
      break;
    }
    ledcWriteTone(1,E4);
    delay(200);
    ledcWriteTone(1,F4);
    delay(300);
    ledcWriteTone(1,R0);
    return(storedValue);
  }
  displayNumber(counter1);
  return(nyuuryoku());
}

void incrementDigit() {
  int tempcounter1 = counter1 * 10;  // Shift decimal one place
  int incrementAmount = pow(10, 3 - selectedDigit);
  tempcounter1 += incrementAmount;
  counter1 = tempcounter1 / 10.0;
  if (counter1 >= 1000.0) counter1 = counter1 - 1000.0;
}

void displayNumber(double value) {
  int integerPart = (int)value;
  int decimalPart = (value - integerPart) * 10;
  int displayDigits[] = {
    integerPart / 100,
    (integerPart % 100) / 10,
    integerPart % 10,
    decimalPart
  };
  for (int i = 0; i < 4; i++) {
    if (i == selectedDigit && !isBlinkOn) continue;
    mcp.digitalWrite(digitPins[i], HIGH);
    for (int j = 0; j < 8; j++) {
      mcp.digitalWrite(segmentPins[j], numbers[displayDigits[i]] & (1 << j));
    }
    if(i==2)mcp.digitalWrite(segmentPins[7],LOW);
    delay(1);
    // Turn on decimal point for 3rd digit
    for (int j = 0; j < 8; j++) {
      mcp.digitalWrite(segmentPins[j], HIGH);//全点灯させないためHIGHをデフォにする
    }
    mcp.digitalWrite(digitPins[i], LOW);
    
  }  
  mcp.digitalWrite(ledPins[0], currentUnit == CM ? HIGH : LOW);
  mcp.digitalWrite(ledPins[1], currentUnit == INCH ? HIGH : LOW);
  mcp.digitalWrite(ledPins[2], currentUnit == SUN ? HIGH : LOW);  
}
