   #define ANALOG_PIN A2
    #define RANGE 5000 // Depth measuring range 5000mm (for water)
    #define VREF 5000 // ADC's reference voltage on your Arduino,typical value:5000mV
    #define CURRENT_INIT 4.00 // Current @ 0mm (uint: mA)
    #define DENSITY_WATER 1  // Pure water density normalized to 1
    #define DENSITY_GASOLINE 0.74  // Gasoline density
    #define PRINT_INTERVAL 1000
    #define debug 
    #define SENSOR          3
    #define DEBOUNCE_TIME   100
    
   float precipitation = 0;
   volatile int pulse = 0;
   static int last_pulse = 0;

    int16_t dataVoltage;
    float dataCurrent, depth; //unit:mA
    unsigned long timepoint_measure;

    void setup()
    {
      Serial.begin(9600);
      pinMode(ANALOG_PIN, INPUT);
      timepoint_measure = millis();
       pinMode(SENSOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR), isr, CHANGE);
    }

    void loop()
    {
      int actual_pulse;
  noInterrupts();
  
  actual_pulse = pulse;
  interrupts();
      if (millis() - timepoint_measure > PRINT_INTERVAL) {
        timepoint_measure = millis();

        dataVoltage = analogRead(ANALOG_PIN)/ 1024.0 * VREF;
        dataCurrent = dataVoltage / 120.0; //Sense Resistor:120ohm
        depth = (dataCurrent - CURRENT_INIT) * (RANGE/ DENSITY_WATER / 16.0); //Calculate depth from current readings

        if (depth < 0) 
        {
          depth = 0.0;
        }
        if (last_pulse != actual_pulse) {

        Serial.println((float) actual_pulse / 100);

        last_pulse = actual_pulse;
        }

        //Serial print results
        Serial.print("depth:");
        Serial.print(depth);
        Serial.println("mm");
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
