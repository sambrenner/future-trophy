#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

//arduino pins
#define	MAX_DEVICES 5
#define	CLK_PIN 10
#define	DATA_PIN 8
#define	CS_PIN 9

//parola
MD_Parola scroller = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
MD_Parola::textEffect_t	scrollEffect = MD_Parola::SCROLL_LEFT;
uint8_t	frameDelay = 100;

#define MSGS_LENGTH 10
char *messages[] = {
  "WESTFIELD ATHLETIC CLUB",
  "FOOTBALL OF CHAMPIONS",
  "","","","","","","",""
};

void setup() {
  Serial.begin(57600);
 
  initScroller();
  scrollNextMessage();
}

void loop() {
  if (scroller.displayAnimate()) {
    if (strcmp(messages[1], "") == 0) {
      scroller.displayReset();
      shiftMessageArrayLeft();
      scrollNextMessage();
    } else {
      //message stack empty
    }
  }
}

void initScroller() {
  scroller.begin();
  scroller.displayClear();
  scroller.displaySuspend(false);
}

void scrollNextMessage() {
  scroller.displayScroll(messages[0], MD_Parola::LEFT, scrollEffect, frameDelay);
}

void shiftMessageArrayLeft() {
  for(int i=0; i<MSGS_LENGTH; i++) {
    if(i+1 == MSGS_LENGTH) {
      strcpy(messages[i], "");
    } else {
      strcpy(messages[i], messages[i+1]);
    }
  }
}
