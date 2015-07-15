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
struct pt pt_taskSerial;

int led1, led2, led3, led4;
int sw, knob;
String val;

///////////////////////////////////////////////////////
PT_THREAD(taskSerial(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    val = Serial1.readStringUntil('\r');
    if (val.substring(0, 5) == "pinsw") {
      if (val.substring(5, 6) == "1") {
        sw = HIGH; 
      }
      else {
        sw = LOW;
      }
    }
    PT_DELAY(pt, 10, ts);
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
    //sw = digitalRead(SW);
	Serial1.print("pinsw");
	Serial1.print(digitalRead(SW));
	Serial1.print("\r");
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
    //knob = analogRead(KNOB);
	Serial1.print("pinkn");
	Serial1.print(analogRead(KNOB));
	Serial1.print("\r");
	//if (knob> 500) {
	//	led3 = HIGH;
	//}
	//else {
	//	led3 = LOW;
	//}
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
  PT_INIT(&pt_taskSerial);
	led1 = HIGH;
	led2 = HIGH;
	led3 = LOW;
	led4 = LOW;
  sw = HIGH;
	knob = 300;
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
  taskSerial(&pt_taskSerial);
}
