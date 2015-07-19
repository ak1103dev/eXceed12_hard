#include <ESP8266WiFi.h>

const char* ssid     = "your-wifi-name";
const char* password = "wifi-password";
const char* host = "research27.ml";
WiFiClient client;

String val = "";

void writeRequest(String value) {
  String url = "/write/" + value;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
}

void readRequest() {
  int countLine = 0;
  while(client.available()){
    String line = client.readStringUntil('\n');
    countLine++;
    if (countLine > 8) {
		  val = line;
    }
  }
}

void serialEvent() {
	while (Serial.available() > 0) {
		val = Serial.readStringUntil('\r');
    delay(10);
    writeRequest(val);
    delay(10);
    readRequest();
    delay(10);
    Serial.print("\t");
    Serial.print(val);
	}
}

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, password);
}

void loop() {
	const int httpPort = 8001;
	if (!client.connect(host, httpPort)) {
		return;
	}
	serialEvent();
	delay(500);
}

