#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 2
#define DIO 3

#define LIT_SPACE 0b00000000
#define LIT_T SEG_D | SEG_E | SEG_F | SEG_G
#define LIT_R SEG_E | SEG_G
#define LIT_A SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G
#define LIT_I SEG_A | SEG_C
#define LIT_N SEG_C | SEG_E | SEG_G
#define LIT_L SEG_D | SEG_E | SEG_F
#define LIT_E SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
#define LIT_F SEG_A | SEG_E | SEG_F | SEG_G

#define TEST_DELAY_1 250
#define TEST_DELAY_2 500
#define TEST_DELAY_3 400

const uint8_t SEG_train_left[]={ 
  LIT_SPACE, LIT_SPACE, LIT_SPACE, LIT_SPACE, LIT_T, LIT_R, LIT_A, LIT_I, LIT_N, LIT_SPACE, //train
  LIT_L, LIT_E, LIT_F, LIT_T, LIT_SPACE, LIT_SPACE, LIT_SPACE, LIT_SPACE              //left
};

const uint8_t Space[]={
  LIT_SPACE
};

const uint8_t Minus[] ={
  SEG_G
};

TM1637Display display(CLK, DIO);

void setup() {
  display.setBrightness(0x0f);
  task_1();
  display.clear();
  task_2();
  display.clear();
}

void loop() {
  task_3();
}

void task_1(){
  for(int i=100; i>=-100; i-=2){
    if(i>=0)
      display.showNumberDec(i,true);
    else{
      display.setSegments(Minus,1,0);
      display.showNumberDec(i*-1,true,3,1);
    }

    delay(TEST_DELAY_1);
  }
}

void task_2() {
  for (int i = 300; i >= -300; i -= 30) {
    if(i<0) {
      display.setSegments(Minus,1,0);
      display.showNumberDecEx(i*-1,0x80,true,3,1);
    }
    else{
      display.setSegments(Space,1,0);
      display.showNumberDecEx(i,0x80, true, 3, 1);
    }
    delay(TEST_DELAY_2);
  }
}
void task_3() {
  while(1){
    for(int i=0; i<= (sizeof(SEG_train_left)/sizeof(SEG_train_left[0])-4); i++){
      display.setSegments(SEG_train_left+i);  
      delay(TEST_DELAY_3);
    }
  }
}
