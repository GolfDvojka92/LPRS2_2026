
// Test with:
// ./waf debug_uart --port=0


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "avr_io_bitfields.h"
#include "uart_stdio.hpp"


struct bf_c {
	//TODO
	u8 c0 : 1;
	u8 r0 : 1;
};
#define o_keybrd (*((volatile bf_c*)(&PORTC)))
#define i_keybrd (*((volatile bf_c*)(&PINC)))

struct bf_d {
	u8 rxd : 1;
	u8 txd : 1;
	u8 zc1 : 1;
	u8 zc2 : 1;
	u8 drv1 : 1;
	u8 drv2 : 1;
	u8 buzz : 1;
	u8 drv3 : 1;
};
#define o_buzz (*((volatile bf_d*)(&PORTD)))


int main(void) {
	// 0 - in
	// 1 - out
	DDRD = bf_to_u8<bf_d>({
		.rxd = 0,
		.txd = 1,
		.zc1 = 0,
		.zc2 = 0,
		.drv1 = 1,
		.drv2 = 1,
		.buzz = 1,
		.drv3 = 1
	});
	// For ins:
	// 1 - pull up
	PORTD = bf_to_u8<bf_d>({
		.rxd = 0,
		.txd = 1,
		.zc1 = 0,
		.zc2 = 0,
		.drv1 = 0,
		.drv2 = 0,
		.buzz = 0,
		.drv3 = 0
	});

	uart_stdio_non_blocking<100> _u;
	printf("Hello!\n\r");
	printf("Hello again!\n\r");
	printf("F_CPU = %ld\n\r", F_CPU);

	u8 s = 0;
	u8 cnt = 0;
	while(1){
		s = ~s;
		PORTA = s;
		PORTB = s;
		PORTC = s;
		printf("cnt = %d\n\r", cnt);
		cnt++;

		_delay_ms(1000); // Print on every 1 sec.
	}

}
