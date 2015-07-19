#include <pt.h>

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

#define LED1 11
#define LED2 12
#define LED3 13
#define LED4 14
#define SW 2
#define KNOB A3

struct pt pt_taskLED1;
struct pt pt_taskLED2;
struct pt pt_taskLED3;
struct pt pt_taskLED4;
struct pt pt_taskSW;
struct pt pt_taskKNOB;
struct pt pt_taskRead;
struct pt pt_taskWrite;

int led1, led2, led3, led4;
int sw, knob;
String val;

void setValue() {
  switch(val.charAt(3)) {
    case '0' : sw = LOW; break;
    case '1' : sw = HIGH; break;
  }
//  switch(val.charAt(8)) {
//    case '0' : knob = LOW; break;
//    case '1' : knob = HIGH; break;
//  }
}

///////////////////////////////////////////////////////
PT_THREAD(taskRead(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    val = Serial1.readStringUntil('\r');
    Serial.print(val.charAt(3));
    setValue();
    PT_DELAY(pt, 50, ts);
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
    Serial1.print("sw");
    Serial1.print(digitalRead(SW));
    PT_DELAY(pt, 10, ts);
    Serial1.print("knob");
    Serial1.print(analogRead(KNOB));
    PT_DELAY(pt, 10, ts);
    Serial1.print(led1);
    Serial1.print(led2);
    Serial1.print(led3);
    Serial1.print(led4);

    Serial1.print("\r");
    PT_DELAY(pt, 100, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskSW(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
	  if (!sw) {
		  led4 = HIGH;
   	}
    else {
		  led4 = LOW;
	  }
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskKNOB(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
	  if (knob> 500) {
		  led3 = HIGH;
	  }
	  else {
		  led3 = LOW;
	  }
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED1(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED1, led1);
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED2(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED2, led2);
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED3(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED3, led3);
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED4(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED4, led4);
    PT_DELAY(pt, 10, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
void setup()
{
	Serial1.begin(115200);
  Serial.begin(9600);
  
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(SW, INPUT);

	PT_INIT(&pt_taskLED1);
	PT_INIT(&pt_taskLED2);
	PT_INIT(&pt_taskLED3);
	PT_INIT(&pt_taskLED4);
	PT_INIT(&pt_taskSW);
	PT_INIT(&pt_taskKNOB);
	PT_INIT(&pt_taskRead);
	PT_INIT(&pt_taskWrite);

	led1 = HIGH;
	led2 = HIGH;
	led3 = LOW;
	led4 = LOW;
	sw = 1;
}

///////////////////////////////////////////////////////
void loop()
{
  taskLED1(&pt_taskLED1);
  taskLED2(&pt_taskLED2);
  taskLED3(&pt_taskLED3);
  taskLED4(&pt_taskLED4);
  taskSW(&pt_taskSW);
  taskKNOB(&pt_taskKNOB);
  taskRead(&pt_taskRead);
  taskWrite(&pt_taskWrite);
}
