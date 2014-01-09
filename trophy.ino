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

void setup() {
  initScroller();
  scrollNextMessage();
}

void loop() {
  if (scroller.displayAnimate()) {
    //request next message from linino
  }
}

void initScroller() {
  scroller.begin();
  scroller.displayClear();
  scroller.displaySuspend(false);
}

void scrollNextMessage(char message[]) {
  scroller.displayScroll(message, MD_Parola::LEFT, scrollEffect, frameDelay);
}