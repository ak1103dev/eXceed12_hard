int countHello = 0;
bool printSuccess = false;

void setup() {
//  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
  delay(500);
  Serial1.flush();
  serialEvent();
  if (countHello % 5 == 0 && countHello > 0 && !printSuccess) {
    Serial1.print("Success\r");
    printSuccess = true;
  }
}

void serialEvent() {
  if (Serial1.available() > 0) {
    String inputString = "";
    inputString = Serial1.readStringUntil('\r');

    if (inputString == "Hello") {
      countHello++;
      printSuccess = false;
    }
  }
}

