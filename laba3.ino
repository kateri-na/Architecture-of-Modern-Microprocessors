#include <Arduino.h>
#include <EEPROM.h>
#include "Keypad.h"
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2
#define TIMEDELAY 1000
#define ADDRESS_MEM_PACK 0

#define ADDRESS_FLAG 0
#define ADDRESS_NUMBER 1

int pin_rows[4] = {9, 8, 7, 6};
int pin_columns[4] = {5, 4, 3, 2};

keypad::Keyboard keyboard(pin_rows, pin_columns);


LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

volatile bool choose(false);
volatile int new_menu(-1);
int cur_menu(-1);

struct memory_cell
{
  bool used;
  int number;
  mem_pack() {
    used = false;
    number = 0;
  }
};

memory_cell using_memory_cell;

void handle_interrupt()
{ 
  char chr = keyboard.scanning_keypad();
  switch(chr)
  {
    case 'A': new_menu = 0; break;
    case 'B': new_menu = 1; break;
    case 'C': new_menu = 2; break;
    case 'D': choose = true; break;
    default:
      break;
  }
}

void setup(){
Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  interrupts();
  switch (EEPROM.read(ADDRESS_FLAG))
  {
   case 1: 
    using_memory_cell.used=true; 
    EEPROM.get(ADDRESS_NUMBER, using_memory_cell.number); 
   break;
   case 0: break;
   default:
    EEPROM.write(ADDRESS_FLAG, 0);
  }
  attachInterrupt(digitalPinToInterrupt(pin_columns[3]), handle_interrupt, FALLING);
}


String menu[3]={"Calculate", "Memory", "Clear"};


int get_number_from_screen(int col, int row, int max_len)
{
  int n = 0;  
  char chr = keyboard.scanning_keypad();
  int pos = col;
  lcd.setCursor(pos, row);
  while(pos<col+max_len&&chr!='#')
  {
    if(chr>='0'&&chr<='9')
    {
      n= n*10 + chr - '0';
      lcd.print(chr);
      pos+=1;
      lcd.setCursor(pos, row);
    }
    chr = keyboard.scanning_keypad();
  }
  
  return n;
}

void calc_menu()
{
  lcd.setCursor(0,0);
  lcd.clear();
  int n1, n2;
  if (using_memory_cell.used)
  {
    lcd.print("n1:from memory?");
    lcd.setCursor(0, 1);
    lcd.print("yes-#, no-any");
    char chr='\0';
    while(chr=='\0')
    {
    chr = keyboard.scanning_keypad();
    }
    if(chr == '#')
      {
          n1 = using_memory_cell.number;
          lcd.setCursor(0,0);
          lcd.clear();
          lcd.print("n1:"+String(n1));
      }
      else
        { 
          lcd.setCursor(0,0);
          lcd.clear();
          lcd.print("n1:");
          n1= get_number_from_screen(3, 0, LCD_COLUMNS - 3);
        }
    lcd.setCursor(0,1);
    lcd.print("n2:");
    n2 = get_number_from_screen(3, 1, LCD_COLUMNS - 3);
  }
  else
  {
    lcd.print("n1:");
    n1= get_number_from_screen(3, 0, LCD_COLUMNS - 3);
    lcd.setCursor(0,1);
    lcd.print("n2:");
    n2 = get_number_from_screen(3, 1, LCD_COLUMNS - 3);
  }

  
  int res = n1 | n2;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("res:"+ String(res));
  lcd.setCursor(0, 1);
  lcd.print("save?y-#,n-any");
  char chr='\0';
  
  while(chr=='\0')
  {
  chr = keyboard.scanning_keypad();
  }
  
  delay(TIMEDELAY);
  if(chr == '#')
  {
    using_memory_cell.used = true;
    using_memory_cell.number = res;
    EEPROM.update(ADDRESS_FLAG, 1);
    EEPROM.put(ADDRESS_NUMBER, res);
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("number saved");
  }

  choose = false;
}

void mem_menu()
{
  //lcd.clear();
  lcd.setCursor(0, 1);
  if(using_memory_cell.used)
    lcd.print("in memory:"+String(using_memory_cell.number));
  else
    lcd.print("empty memory!");

  delay(TIMEDELAY);
  choose = false;
}

void clear_menu()
{ 
  lcd.clear();
  lcd.setCursor(0, 0);
  if(using_memory_cell.used)
  {
    lcd.print("in memory:"+String(using_memory_cell.number));
    lcd.setCursor(0, 1);
    lcd.print("clear?yes-#,no-any");
    
    char chr='\0';
  
    while(chr=='\0')
    {
    chr = keyboard.scanning_keypad();
    }
    if(chr =='#')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Memory cleared");
      using_memory_cell.used=false;

      EEPROM.update(ADDRESS_FLAG, 0);
    }
  }
  else
    lcd.print("empty memory!");
  
  delay(TIMEDELAY);
  choose = false;
}

void loop()
{
  if (new_menu != cur_menu)
  {
    detachInterrupt(digitalPinToInterrupt(pin_columns[3]));
    lcd.setCursor(0,0);
    cur_menu=new_menu;
    lcd.clear();
    lcd.print(menu[cur_menu]);
    if(cur_menu == 1){
      mem_menu();
    }
    attachInterrupt(digitalPinToInterrupt(pin_columns[3]), handle_interrupt, FALLING);
  }
  else
  { 
     if (choose)
     {
       detachInterrupt(digitalPinToInterrupt(pin_columns[3]));
       switch (cur_menu)
       {
         case 0: calc_menu();break;
         case 2: clear_menu();break;
         default: break;
        }
       lcd.clear();
       lcd.print(menu[cur_menu]);
       attachInterrupt(digitalPinToInterrupt(pin_columns[3]), handle_interrupt, FALLING);
    }
  }
}
