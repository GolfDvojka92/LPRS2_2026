
// Test with:
// ./waf debug_uart --port=0

#include "common.hpp"

void set_color(int screen_half, color_t color) {
	switch(screen_half){
		case 0:
			screen.rgb0 = color;
			break;
		case 1:
			screen.rgb1 = color;
			break;
	} 
}


int main(void) {
	screen.odd_0 = wide_bool(DDR_OUT);
	screen.odd_1 = wide_bool(DDR_OUT);
	screen.odd_2 = wide_bool(DDR_OUT);
	screen.odd_3 = wide_bool(DDR_OUT);



	//screen.rgb0 = screen.rgb1 = WHITE;
	
	while(1){

		for(int color = 0; color < 8; color++){
			for(int vsync = 0; vsync < 500; vsync++){
				for(int row_grp = 0; row_grp < 8; row_grp++){
					screen.n_oe = 1;
					
					// Set row addr.
					screen.addr = row_grp;

					for(int row_shift = 2; row_shift >= 0; row_shift--){
						for(int col = 0; col < 64; col++){
							//for(int screen_half = 0; screen_half < 2; screen_half++){
							//	int row = screen_half*16 + row_shift*8 + row_grp;
							//	
							//	set_color(screen_half, color_t(color));
							//}
							screen.rgb0 = screen.rgb1 = color;
							
							// Bit pulse.
							screen.clk = 1;
							screen.clk = 0;
						} 
						
					}

					// Latch pulse.
					screen.latch = 1;
					//_delay_us(1); // For scope
					screen.latch = 0;

					// Turn on LEDs.
					screen.n_oe = 0;

					_delay_us(100);
				}
			}
		}

	} // while

}
