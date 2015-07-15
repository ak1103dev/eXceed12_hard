#include <ESP8266WiFi.h>
#include <pt.h>

const char* ssid     = "ak1103@#$/";
const char* password = "#0804521864*";
const char* host = "research27.ml";
const int httpPort = 8001;
WiFiClient client;

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

int i = 0;
bool letgo = false;
int countSuccess = 0;

struct pt pt_taskSerial;
struct pt pt_taskClient;

///////////////////////////////////////////////////////
PT_THREAD(taskSerial(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    Serial.readStringUntil('\r');
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskClient(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    Serial.readStringUntil('\r');
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////
void writeRequest(String qry) {

  String url = "/write/" + qry;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);
}

///////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  PT_INIT(&pt_taskSerial);
  PT_INIT(&pt_taskClient);
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

  taskSerial(&pt_taskSerial);
  serialEvent();
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

