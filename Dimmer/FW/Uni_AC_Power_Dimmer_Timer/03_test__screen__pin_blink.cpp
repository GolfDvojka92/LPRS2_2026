
// Test with:
// ./waf debug_uart --port=0

#include "common.hpp"

#define BLINK(pin) \
	do{ \
		printf("%s\n", #pin); \
		for(u8 i = 0; i < 64; i++){ \
			bool even = i & 1; \
			pin = wide_bool(even); \
			_delay_ms(1000/8); \
		} \
	}while(0) \

int main(void) {
	//uart_stdio_non_blocking<100> _u;
	uart_stdio_blocking _u;
	printf("%s\n", __FILE__);

	screen.odd_0 = wide_bool(DDR_OUT);
	screen.odd_1 = wide_bool(DDR_OUT);
	screen.odd_2 = wide_bool(DDR_OUT);
	screen.odd_3 = wide_bool(DDR_OUT);

	//DDRB = 0xff;
	//printf("sizeof(screen) = %d\n", sizeof(screen));

	// Turn off LEDs.
	screen.rgb0 = 0;
	screen.rgb1 = 0;
	screen.addr = 0;
	screen.clk = 0;
	screen.latch = 0;
	screen.n_oe = 0;

	while(1){
		//BLINK(PORTA);
		
		// pin 1
		BLINK(screen.r0); // Work
		//BLINK(screen.b0); // Work
		//BLINK(screen.r1);
		//BLINK(screen.b1);
		//BLINK(screen.a); // Work
		//BLINK(screen.c); // Work
		//BLINK(screen.clk); // Work
		//BLINK(screen.n_oe); // Work
		// pin 16
		// GND
		//BLINK(screen.latch); // Work
		// D
		//BLINK(screen.b); // Work
		// NC
		//BLINK(screen.g1);
		// NC
		//BLINK(screen.g0); // Work
	}

}
