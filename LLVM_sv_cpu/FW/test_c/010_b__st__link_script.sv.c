
#include <stdint.h>

// .sdata
uint64_t end_of_program; // Need to be at addr 0

// .text
void main() {

	end_of_program = 1;

	while(1){
	}
}
