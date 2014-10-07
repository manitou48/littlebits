// http://hacking.majenko.co.uk/node/57
//  check 5v level by using 1.1v ref bandgap (how accurate is that?)
//   uses 1.1v input to ADC, calculate what Vcc must be
long readVcc() {
  long result;
  // Read 1.1V reference (MUX1 2 3 4) against AVcc (REFS0)  table 24-4
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV   1.1*1024
  return result;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println( readVcc(), DEC );
  delay(1000);
}
