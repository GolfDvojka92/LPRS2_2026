
#include "defs.hpp"

#define S_OE_N 3
#define S_CLK 4
#define S_LATCH A5
#define S_A 6
#define S_B A3
#define S_C 5
#define S_R0 10
#define S_G0 11
#define S_B0 9
#define S_R1 8
#define S_G1 A1
#define S_B1 7


#define LED 13

void sel_bitplane(int addr) {
	digitalWrite(S_A, addr>>0 & 1);
	digitalWrite(S_B, addr>>1 & 1);
	digitalWrite(S_C, addr>>2 & 1);
}

static inline void latch_pulse() { 
	digitalWrite(S_LATCH, 1);
	digitalWrite(S_LATCH, 0);
}

static inline void bit_pulse() {
	digitalWrite(S_CLK, 1);
	digitalWrite(S_CLK, 0); 
}


void set_color(color_t color) {
	digitalWrite(S_R0, color>>0 & 1);
	digitalWrite(S_G0, color>>1 & 1);
	digitalWrite(S_B0, color>>2 & 1);  
	digitalWrite(S_R1, color>>0 & 1);
	digitalWrite(S_G1, color>>1 & 1);
	digitalWrite(S_B1, color>>2 & 1); 
}

color_t bitchasing_pixel(int row, int col){
	return color_t( row + col );
}

void setup() {

	pinMode(LED, OUTPUT);
	digitalWrite(LED, 0);
	
	pinMode(S_A, OUTPUT);
	pinMode(S_B, OUTPUT);
	pinMode(S_C, OUTPUT);

	
 
	pinMode(S_R0, OUTPUT);
	pinMode(S_G0, OUTPUT);
	pinMode(S_B0, OUTPUT);
	pinMode(S_R1, OUTPUT);
	pinMode(S_G1, OUTPUT);
	pinMode(S_B1, OUTPUT);
	 
	pinMode(S_CLK, OUTPUT);
	digitalWrite(S_CLK, 0);
	 
	pinMode(S_LATCH, OUTPUT);
	digitalWrite(S_LATCH, 1);
	
	pinMode(S_OE_N, OUTPUT);
	digitalWrite(S_OE_N, 1);


	Serial.begin(115200);

	Serial.println(bitchasing_pixel(0,1));
}



void loop() {
	for(int bitplane = 0; bitplane < 8; bitplane++){
		digitalWrite(S_OE_N, 1);
			 
		sel_bitplane(bitplane);
		 
		for(int i = 0; i < 16; i++){
			set_color(color_t(i));
			for(int j = 0; j < 8; j++){
				bit_pulse();   
			} 
					
		}
		latch_pulse();


		digitalWrite(S_OE_N, 0);
		delay(1);
	}
}
