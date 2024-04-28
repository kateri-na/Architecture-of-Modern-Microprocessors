#include "Keypad.h"

char keypad::Keyboard::scanning_keypad()
{
  for(int r=0; r<4;++r)
    		digitalWrite(row_pins[r], HIGH);

    	char res = '\0';
    	for(int r=0; r<4&&res=='\0';++r)
    	{
    		digitalWrite(row_pins[r], LOW);
        delayMicroseconds(10);
    		for(int c = 0; c<4&&res=='\0';++c)
    			if(digitalRead(column_pins[c]) == LOW)
    				{
              delay(100);
              if(digitalRead(column_pins[c]) == LOW)
                {
                  res = symbols[r][c];
                  delay(100);
                }
            }
    		digitalWrite(row_pins[r], HIGH);
    	}
    	for(int r=0; r<4;++r)
    		digitalWrite(row_pins[r], LOW);
      return res;
};


