#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Process.h>

//arduino pins
#define	MAX_DEVICES 5
#define	CLK_PIN 8
#define	DATA_PIN 3
#define	CS_PIN 12

//parola
#define	BUF_SIZE 75
MD_Parola scroller = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
uint8_t	frameDelay = 40;
char nextMessage[BUF_SIZE];

//arduino
boolean contactWithLinino = false;
boolean bridgeBegan = false;
int countPercentSigns = 0;
int expectedPercentSigns = 3;

void setup() {
  Serial1.begin(250000);

  scroller.begin();
  scroller.displayClear();
  scroller.displaySuspend(false);
}

void loop() {
  //only run when scroller is ready
  if (scroller.displayAnimate()){
    
    //if we are still waiting for linino to boot:
    if (!contactWithLinino) {
      if (Serial1.available()) {
        char c = (char)Serial1.read();
      
        if (c=='%') {
          countPercentSigns++;
          if (countPercentSigns == expectedPercentSigns){
            contactWithLinino = true;
          }
        }
      } else {
        displayMessage("Loading...");
      }
    } 
    
    //linino has booted.
    else {
      //have we initialized contact with Bridge?
      if (!bridgeBegan) {
        Bridge.begin();
        bridgeBegan = true;
      }
    
      //request the trophy message
      scrollNextTrophyMessage();
    }
  }
}

void scrollNextTrophyMessage() {
  Process process;
  String pyMessage = "";

  process.runShellCommand("python /root/trophy/trophy.py nextmsg");
  
  while(process.running()) {}

  while(process.available()) {
    char next = process.read();

    if (next != '\n' && next != '\r') {
      pyMessage += next;
    }
  }

  char *msg = new char[pyMessage.length() + 1];
  strcpy(msg, pyMessage.c_str());
  
  displayMessage(msg);
  
  delete [] msg;
}

void displayMessage(char* msg) {
  strcpy(nextMessage, msg);
  scroller.displayScroll(nextMessage, MD_Parola::LEFT, MD_Parola::SCROLL_LEFT, frameDelay);
}
