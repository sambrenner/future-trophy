#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Process.h>

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
  Bridge.begin();
  scrollNextMessage();
}

void loop() {
  if (scroller.displayAnimate()) {
    scrollNextMessage();
  }
}

void initScroller() {
  scroller.begin();
  scroller.displayClear();
  scroller.displaySuspend(false);
  
  scroller.displayScroll("loading...", MD_Parola::LEFT, scrollEffect, frameDelay);
}

void scrollNextMessage() {
  Process process;
  String nextMessage = "";

  process.runShellCommand("python /root/trophy/trophy.py nextmsg");
  while(process.running());

  while(process.available()) {
    nextMessage += (char)process.read();
  }
  
  char msg[nextMessage.length()];
  nextMessage.toCharArray(msg, nextMessage.length() - 1);
  
  scroller.displayScroll(msg, MD_Parola::LEFT, scrollEffect, frameDelay);
}
