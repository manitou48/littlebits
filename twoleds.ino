// littlebits arduino two yellow LEDs USB rx/tx PB0/PD5, no pin out
//  https://www.pjrc.com/teensy/pins.html
// hardware/arduino/variants/leonardo/pins_arduino.h

#define MS 2000

#define PRBYTE(x) Serial.print(#x" 0b"); Serial.println(x,BIN)
void setup() {
  #if 0
	Serial.begin(9600);
	while(!Serial); 
	PRBYTE(DDRB);    // mode
	PRBYTE(DDRD);
       Serial.end();  // no matter
  #endif
}

void loop() {
	RXLED1;
	delay(MS);
	RXLED0;
	delay(MS);
	TXLED1;
	delay(MS);
	TXLED0;
	delay(MS);
	RXLED1; TXLED1;
	delay(MS);
	RXLED0; TXLED0;
	delay(MS);
}
