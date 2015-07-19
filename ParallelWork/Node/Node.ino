#include <ESP8266WiFi.h>

#include <pt.h>
#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

const char* ssid     = "ak1103@#$/";
const char* password = "#0804521864*";
const char* host = "research27.ml";
WiFiClient client;

struct pt pt_taskRead;
struct pt pt_taskWrite;

bool letgo = false;
int countSuccess = 0;
String readVal = "";
String writeVal = "";

void writeRequest(String val) {
  String url = "/write/" + val;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
}

void readRequest() {

  String url = "/read";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  int countLine = 0;
  while(client.available()){
    String line = client.readStringUntil('\n');
    countLine++;
    if (countLine > 8) {
		  readVal = line;
    }
  }
}

PT_THREAD(taskRead(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
	while(Serial.available() > 0) {
		writeVal = Serial.readStringUntil('\r');
		writeRequest(writeVal);
	}
	PT_DELAY(pt, 50, ts);
  }

  PT_END(pt);
}

PT_THREAD(taskWrite(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
	  readRequest();
	  Serial.print("\t");
	  Serial.print(readVal);
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}

void setup() {
  Serial.begin(115200);
  delay(10);
//
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//
  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

  PT_INIT(&pt_taskRead);
  PT_INIT(&pt_taskWrite);
}

void loop() {

  taskRead(&pt_taskRead);
  taskWrite(&pt_taskWrite);
  const int httpPort = 8001;
  if (!client.connect(host, httpPort)) {
//    Serial.print("connection failed");
    return;
  }

 // if (letgo == true) {
 //   Serial.print("Hello\r");
 // }
 // if (countSuccess >= 3) {
 //   Serial.println("sentExit");
 //   writeRequest("exit");
 // }
 // readRequest();
}

