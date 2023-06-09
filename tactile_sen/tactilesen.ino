
// Pin for the LED
int LEDPin1 = 11;
int LEDPin2 = 12;
int LEDPin3 = 13;
// Pin to connect to your drawing
int capSensePin1 = 2;
int capSensePin2 = 3;
int capSensePin3 = 4;
// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
int touchedCutoff = 40;

void setup(){
  Serial.begin(9600);
  // Set up the LED
  pinMode(LEDPin1, OUTPUT);
  digitalWrite(LEDPin1, LOW);
  pinMode(LEDPin2, OUTPUT);
  digitalWrite(LEDPin2, LOW);
  pinMode(LEDPin3, OUTPUT);
  digitalWrite(LEDPin3, LOW);
}

void loop(){
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  if (readCapacitivePin(capSensePin1) < touchedCutoff) {
    digitalWrite(LEDPin1, HIGH);
  }
  else {
    digitalWrite(LEDPin1, LOW);
  }

  if (readCapacitivePin(capSensePin2) < touchedCutoff) {
    digitalWrite(LEDPin2, HIGH);
  }
  else {
    digitalWrite(LEDPin2, LOW);
  }

  if (readCapacitivePin(capSensePin3) < touchedCutoff) {
    digitalWrite(LEDPin3, HIGH);
  }
  else {
    digitalWrite(LEDPin3, LOW);
    
  }
  
  // Every 500 ms, print the value of the capacitive sensor
  if ( (millis() % 250) == 0){
    Serial.print("Capacitive Sensor on Pin 2 reads: ");
    Serial.print(readCapacitivePin(capSensePin1));
    Serial.print(" ");
    Serial.print(readCapacitivePin(capSensePin2));
    Serial.print(" ");
    Serial.print(readCapacitivePin(capSensePin3));
    Serial.print("\n");

  }
}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//          how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  In order for this to work now,
// The pin should have a 1+Megaohm resistor pulling
//  it up to +5v.
uint8_t readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}



