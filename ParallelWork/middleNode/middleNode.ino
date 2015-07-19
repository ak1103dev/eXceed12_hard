#include <ESP8266WiFi.h>
#include <pt.h>

const char* ssid     = "ak1103@#$/";
const char* password = "#0804521864*";
const char* host = "research27.ml";

WiFiClient client;

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

int i = 0;
bool letgo = false;
int countSuccess = 0;

struct pt pt_taskRead;
struct pt pt_taskWrite;

///////////////////////////////////////////////////////
PT_THREAD(taskRead(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
	  readRequest();
    PT_DELAY(pt, 1000, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskWrite(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
	  writeRequest(serialEvent());
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
String serialEvent() {
  String inputStr = "";
  if(Serial.available() > 0) {
    inputStr = Serial.readStringUntil('\r');
  }
  return inputStr;
}

///////////////////////////////////////////////////////
void readRequest() {

  String url = "/read";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
//  delay(10);
  int countLine = 0;
  while(client.available()){
    String line = "";
    line = client.readStringUntil('\n');
//	  countLine++;
//	  if (countLine > 8) {
		  Serial.print(line);
//	  }
  }
}

///////////////////////////////////////////////////////
void writeRequest(String dataBody) {

  String url = "/write/" + dataBody;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
//  delay(10);
}

///////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  PT_INIT(&pt_taskRead);
  PT_INIT(&pt_taskWrite);
  delay(10);

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


///////////////////////////////////////////////////////
void loop() {

  const int httpPort = 8001;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  taskRead(&pt_taskRead);
  //taskWrite(&pt_taskWrite);
//  delay(1000);
}

