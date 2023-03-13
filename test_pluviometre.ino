//#define debug 
#define SENSOR          3
#define DEBOUNCE_TIME   100
float precipitation = 0;
volatile int pulse = 0;
static int last_pulse = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(SENSOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR), isr, CHANGE);
}

void loop()
{
  int actual_pulse;
  noInterrupts();
  
  actual_pulse = pulse;
  interrupts();
  if (last_pulse != actual_pulse) {
#ifdef debug
    Serial.println((float) actual_pulse / 100);
#endif
    last_pulse = actual_pulse;
  }
}

int pinState, previousPinState = HIGH;

void isr()
{
  static unsigned long last_interrupt_time;
  unsigned long interrupt_time = millis();

  pinState = digitalRead(SENSOR);
  if (pinState != previousPinState) {
    if (pinState == LOW) {
      if (interrupt_time - last_interrupt_time >= DEBOUNCE_TIME) {
        pulse += 0.28 * 100;
        
        
      }
    }
    last_interrupt_time = interrupt_time;
    previousPinState = pinState;
  }
}
