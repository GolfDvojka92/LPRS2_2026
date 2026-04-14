
#include <stdint.h>

// .sdata
uint64_t end_of_program; // Need to be at addr 0

// .text
void main() {

	*(volatile uint64_t*)0 = 1;

	while(1){
	}
}
