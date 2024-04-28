#pragma once
#include <Arduino.h>

namespace keypad{
class Keyboard
{
	char symbols[4][4]{ '1','2','3','A','4', '5', '6', 'B', '7', '8', '9','C', '*', '0', '#', 'D'};
	int row_pins[4];
	int column_pins[4];

  public:
	Keyboard(int* row_pin, int* column_pin)
	{
		for(int i=0; i<4;++i)
		{
			row_pins[i]=row_pin[i];
			column_pins[i]=column_pin[i];
			pinMode(row_pins[i], OUTPUT);
    	digitalWrite(row_pins[i], LOW);
			pinMode(column_pins[i], INPUT_PULLUP);			
		}
	}
	char scanning_keypad();
};
}