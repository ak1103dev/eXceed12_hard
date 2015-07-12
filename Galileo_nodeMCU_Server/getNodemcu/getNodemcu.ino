/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "WiFi name";
const char* password = "WiFi password";

const char* host = "research27.ml";

WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;
int i = 0;
bool letgo = false;
int countSuccess = 0;

void loop() {
  
  ++value;
  serialEvent();
  const int httpPort = 8001;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  
  if (letgo == true) {
    Serial.print("Hello\r");
  }
  if (countSuccess >= 3) {
    Serial.println("sentExit");
    writeRequest("exit"); 
  }
  readRequest();
  delay(1000);
}

void serialEvent() {
  if(Serial.available() > 0) {
    String inputStr = "";
    inputStr = Serial.readStringUntil('\r');
  
    if (inputStr == "Success") {
      countSuccess++;
      Serial.print("successX : ");
      Serial.println(countSuccess);
    }
    Serial.println("get");
  }
}

void readRequest() {
  
  String url = "/read";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10); 
  int countLine = 0;
  while(client.available()){
    String line = client.readStringUntil('\n');
    countLine++;
    if (countLine > 8) {
      if (line == "LetGo") {
        letgo = true;  
      }
      else if (line == "exit") {
        letgo = false; 
      }
      else {
        Serial.print("<no>");
        Serial.print(line);
        Serial.println("</no>");
      }
    }
  }
}

void writeRequest(String qry) {
  
  String url = "/write/" + qry;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
}
