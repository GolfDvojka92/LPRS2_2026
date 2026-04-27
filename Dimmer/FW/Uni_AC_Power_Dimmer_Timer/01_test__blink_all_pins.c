
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	// Make all pins outputs.
	DDRA = 0xff;
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;
	uint8_t s = 0;
	while(1){ //infinite loop
		s = ~s;
		PORTA = s;
		PORTB = s;
		PORTC = s;
		PORTD = s;
		_delay_ms(1000/2/5); // For 5Hz flashing.
		//_delay_ms(1000/2); // For 1Hz flashing.
		//_delay_ms(4000/2); // For 0.25Hz flashing.
	}
}
