
#include "uart_stdio.hpp"

#include "avr_io_bitfields.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

//TODO rx aka stdin

void common_uart_init() {
//#define BAUD 115200 //FIXME Cannot have it?
#define BAUD 9600
#include <util/setbaud.h>

	uart.r.ubrrh = UBRRH_VALUE;
	uart.r.ubrrl = UBRRL_VALUE;
	#if USE_2X
	uart.r.ucsra |= (1 << U2X);
	#else
	uart.r.ucsra &= ~(1 << U2X);
	#endif

	// Enable Tx.
	uart.f.txen = 1;

	// Set the USART to asynchronous at 8 bits, no parity and 1 stop bit.
	uart.f.ursel = 1;
	uart.f.umsel = 0;
	uart.f.upm = 0;
	uart.f.usbs = 0;
	uart.f.ucsz = 3;
}

static bool uart_tx_busy() {
	return !uart.f.udre;
}

namespace _priv_uart_stdio_non_blocking {
	class queue_view {
	public:
		char* volatile p_begin; // First buf.
		char* volatile p_end; // After last in buf.
		char* volatile p_pop; // First data to pop.
		char* volatile p_push; // First empty to push.
		bool empty() {
			return p_pop == p_push;
		}
		bool full() {
			char* p_next_push = p_push + 1;
			if(p_push == p_end){
				p_next_push = p_begin;
			}
			return p_pop == p_next_push;
		}
		// push() block until some not full again.
		//TODO This is try_push(), non blocking
		bool push(const char& c) {
			//TODO 
			//if(full()){ // Not working good.
			if(false){
				// Overflow. Dropping data.
				return false;
			}else{
				*p_push = c;
				p_push++;
				if(p_push == p_end){
					p_push = p_begin;
				}
				return true;
			}
		}
		bool try_pop(char& c) {
			if(empty()){
				return false;
			}else{
				c = *p_pop;
				p_pop++;
				if(p_pop == p_end){
					p_pop = p_begin;
				}
				return true;
			}
		}
	};
}
static _priv_uart_stdio_non_blocking::queue_view qv;

ISR(USART_UDRE_vect) {
	char c;
	if(qv.try_pop(c)){
		// Putting data into buffer will sends data.
		uart.r.udr = c;
	}else{
		uart.f.udrie = 0; // Dissable IRQ so this ISR is not called non-stop.
	}
}

static int uart_putchar_blocking(char c, FILE *stream) {
	// Wait for empty transmit bufer.
	while(uart_tx_busy()){}
	
	// Putting data into bufer will sends data.
	uart.r.udr = c;
	
	return 0;
}
static int uart_putchar_non_blocking(char c, FILE *stream) {
	// If was not empty, then sending is in progress.
	bool was_empty = qv.empty();
	qv.push(c);
	// If it is empty, it will have its IRQ.
	if(was_empty && !uart_tx_busy()){
		// Start sending.
		uart.f.udrie = 1; // Enable IRQ.
	}
	return 0;
}


static FILE uart_fd;

uart_stdio_blocking::uart_stdio_blocking() {
	common_uart_init();
	
	fdev_setup_stream(
		&uart_fd,
		uart_putchar_blocking,
		NULL,
		_FDEV_SETUP_WRITE
	);

	stdout = stdin = &uart_fd;
}

namespace _priv_uart_stdio_non_blocking {
	void init(char* buf_begin, char* buf_end) {
		qv.p_begin = buf_begin;
		qv.p_end = buf_end;
		qv.p_push = qv.p_pop = qv.p_begin;
		
		common_uart_init();
		//uart.f.udrie = 1; // Enable IRQ.
		sei();
		
		fdev_setup_stream(
			&uart_fd,
			uart_putchar_non_blocking,
			NULL,
			_FDEV_SETUP_WRITE
		);
		
		stdout = stdin = &uart_fd;
	}
}
