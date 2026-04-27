
#include "defs.hpp"
#include "avr_io_bitfields.h"
#include "lcd5x8_font.h"

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

///////////////////////////////////////////////////////////////////////////////

#define DEBUG(x) \
	do{ \
		Serial.print(#x" = "); Serial.println(x); \
	}while(0)
#define DEBUG_HEX(x) \
	do{ \
		Serial.print(#x" = 0x"); Serial.println(x, HEX); \
	}while(0)
#define DEBUG_BYTES(x) \
	do{ \
		Serial.print("0x"); \
		for(size_t i = 0; i < sizeof(x); i++){ \
			u8 b = reinterpret_cast<const u8*>(&x)[i]; \
			u8 nl = b & 0xf; \
			u8 nu = b >> 4; \
			Serial.print(int(nu), HEX); \
			Serial.print(int(nl), HEX); \
		} \
		Serial.println(); \
	}while(0)

///////////////////////////////////////////////////////////////////////////////

void sel_addr(int addr) {
	digitalWrite(S_A, addr>>0 & 1);
	digitalWrite(S_B, addr>>1 & 1);
	digitalWrite(S_C, addr>>2 & 1);
}

static inline void latch_pulse() { 
	digitalWrite(S_LATCH, 1);
	digitalWrite(S_LATCH, 0);
}

static inline void bit_pulse() {
	//TODO This is also critical and need to use bitfields.
	digitalWrite(S_CLK, 1);
	digitalWrite(S_CLK, 0); 
}


void set_color(int screen_half, color_t color) {
	switch(screen_half){
		case 0:
			//TODO Use bitfields for all colors.
#if 1
			digitalWrite(S_R0, color>>0 & 1);
#else
			portb.b2 = color>>0 & 1;
			portb.b2 = color>>1 & 1;
			portb.b2 = color>>2 & 1;
#endif
			
			//digitalWrite(S_G0, color>>1 & 1);
			//digitalWrite(S_B0, color>>2 & 1); 
			break;
		case 1:
			//digitalWrite(S_R1, color>>0 & 1);
			//digitalWrite(S_G1, color>>1 & 1);
			//digitalWrite(S_B1, color>>2 & 1); 
			break;
	} 
}

color_t bitchasing_pixel(int row, int col){
	return color_t( row + col );
}





#define TEXT_ROWS 4
#define TEXT_COLS 10
char text_buf[TEXT_ROWS][TEXT_COLS];

bool bitchase_text(u8 pix_row, u8 pix_col) {
	u8 glyph_col = pix_col % 6;
	if(glyph_col == 5){
		// Empty column.
		return false;
	}
	u8 glyph_row = pix_row % 9;
	if(glyph_row == 8){
		// Empty row.
		return false;
	}
	
	u8 text_col = pix_col / 6;
	if(text_col >= TEXT_COLS) {
		return false;
	}
	
	u8 text_row = pix_row / 9;
	if(text_row >= TEXT_ROWS) {
		return false;
	}
	
	
	char c = text_buf[text_row][text_col];
	if(c < 0x20 || c > 0x7e){
		return false;
	}
DEBUG_HEX(c);
	
	u8 glyph_idx = (c - 32) * 5;
	
	u8 line = pgm_read_byte(&lcd5x8_font[glyph_idx + glyph_col]);
	
	return line & (1 << glyph_row);
}

void setup() {
	Serial.begin(115200);

	strcpy(&text_buf[0][0], "JEJA");

	
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
	digitalWrite(S_R0, 0);
	digitalWrite(S_G0, 0);
	digitalWrite(S_B0, 0);
	digitalWrite(S_R1, 0);
	digitalWrite(S_G1, 0);
	digitalWrite(S_B1, 0);
	 
	pinMode(S_CLK, OUTPUT);
	digitalWrite(S_CLK, 0);
	 
	pinMode(S_LATCH, OUTPUT);
	digitalWrite(S_LATCH, 1);
	
	pinMode(S_OE_N, OUTPUT);
	digitalWrite(S_OE_N, 1);

}


/**
 * 64 cols x 32 rows, 1/8 scan
 * 8 row groups with 8 addrs
 *    addr = 0:
 *    	RGB0:
 *    		rows: 0, 8,
 *    	RGB1:
 *    		rows: 16, 24
 *    addr = 1:
 *    	RGB0:
 *    		rows: 1, 9,
 *    	RGB1:
 *    		rows: 17, 25
 *    		
 * Shift 64 cols x 2 rows
 * 	
 * 	bitplane = 64 cols x 4 rows = 256 LEDs
 */


void loop() {
	static int target_row = 1;
	static int target_col = 0;

	for(int row_grp = 0; row_grp < 8; row_grp++){
		digitalWrite(S_OE_N, 1);
			 
		sel_addr(row_grp);
		 
		for(int row_shift = 2; row_shift >= 0; row_shift--){
			for(int col = 0; col < 64; col++){
				for(int screen_half = 0; screen_half < 2; screen_half++){
					int row = screen_half*16 + row_shift*8 + row_grp;

					bool pix = true;
					pix = bitchase_text(row, col);
					set_color(screen_half, pix ? RED : BLACK);
				}
				
				bit_pulse();   
			} 
					
		}
		latch_pulse();


		digitalWrite(S_OE_N, 0);
		delayMicroseconds(100);
	}

#if 1
	target_col++;
	if(target_col == 64){
		target_col = 0;
		target_row++;
		if(target_row == 32){
			target_row = 0;
		}
	}
#endif
	
}
