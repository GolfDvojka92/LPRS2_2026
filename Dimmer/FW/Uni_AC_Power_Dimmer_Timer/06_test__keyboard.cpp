
// Test with:
// ./waf debug_uart --port=0

#include "common.hpp"



int main(void) {
	screen.odd_0 = wide_bool(DDR_OUT);
	screen.odd_1 = wide_bool(DDR_OUT);
	screen.odd_2 = wide_bool(DDR_OUT);
	screen.odd_3 = wide_bool(DDR_OUT);

	//screen.rgb0 = screen.rgb1 = WHITE;
	
	while(1){


	} // while

}
