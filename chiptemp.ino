// internal temp adc 8 for littlebits/leonardo
// we do raw  ADMUX or patch wiring_analog.c
//  http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1252144888
// calibration http://nyfiken.org/filer/kalibrering.ods

void setup() {
  Serial.begin(9600);
  ADMUX = _BV(REFS1) | _BV(REFS0) | 7;   // Set internal V reference, temperature reading
  ADCSRB = 0x20;    // ref  24.6
  delay(100);
  chipTempRaw(); // discard first sample
}

void loop() {
  int raw = chipTempRaw();
  float temp = chipTemp(raw);
  float ftemp = 1.8f * temp + 32;
  Serial.print(temp);
  Serial.print("  ");
  Serial.print(ftemp);
  Serial.print("   ");
  Serial.println(raw, DEC);
  analogWrite(5,int(ftemp*255/100)); // number module in values
  delay(1000);
}

float chipTemp(float raw) {
  const float chipTempOffset = -142.5;
  const float chipTempCoeff = .558;
  return((raw * chipTempCoeff) + chipTempOffset);
}

int chipTempRaw(void) {
//  while((ADCSRA & _BV(ADSC)));  // Wait for any ongoing conversion to complete
  ADCSRA &= ~(_BV(ADATE) |_BV(ADIE)); // Clear auto trigger and interrupt enable
  ADCSRA |= _BV(ADEN) | _BV(ADSC);        // Enable AD and start conversion
  while((ADCSRA & _BV(ADSC)));             // Wait until conversion is finished
  return(ADCL | (ADCH << 8));
}
