#include <pt.h>
 
#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

#define green 2
#define red 3
 
struct pt pt_taskRed;
struct pt pt_taskGreen;
 
///////////////////////////////////////////////////////
PT_THREAD(taskGreen(struct pt* pt))
{
  static uint32_t ts;
 
  PT_BEGIN(pt);
 
  while (1)
  {
    digitalWrite(green, HIGH);
    PT_DELAY(pt, 1000, ts);
    digitalWrite(green, LOW);
    PT_DELAY(pt, 500, ts);
  }
 
  PT_END(pt);
}
 
///////////////////////////////////////////////////////
PT_THREAD(taskRed(struct pt* pt))
{
  static uint32_t ts;
 
  PT_BEGIN(pt);
 
  while (1)
  {
    digitalWrite(red, HIGH);
    PT_DELAY(pt, 700, ts);
    digitalWrite(red, LOW);
    PT_DELAY(pt, 300, ts);
  }
 
  PT_END(pt);
}
 
///////////////////////////////////////////////////////
void setup()
{
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  PT_INIT(&pt_taskGreen);
  PT_INIT(&pt_taskRed);
}
 
///////////////////////////////////////////////////////
void loop()
{
  taskGreen(&pt_taskGreen);
  taskRed(&pt_taskRed);
}
