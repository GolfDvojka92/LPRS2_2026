
// Test with:
// ./waf debug_uart --port=0

#include "common.hpp"

#define EN_POLL 0

ISR(INT2_vect)
{
	static bool b;
	b = !b;
	portc.b7 = b;
}

int main(void) {

	// Just for debug.
	ddrc.b7 = DDR_OUT;

	//TODO Move to struct.

	// Set PB2 (INT2) as input
	ddrb.b2 = DDR_IN;

	#if !EN_POLL
		// Set INT2 to trigger on FALLING edge
		MCUCSR &= ~(1 << ISC2);   // ISC2 = 0 → falling edge

		// Enable INT2 interrupt
		GICR |= (1 << INT2);

		// Clear any pending interrupt flag (optional but good practice)
		GIFR |= (1 << INTF2);

		// Enable global interrupts
		sei();
	#endif
	
	while(1){
		#if EN_POLL
			portc.b7 = pinb.b2;
		#endif
	} // while

}
