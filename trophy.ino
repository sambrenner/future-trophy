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
boolean firstRun = true;

/*	
linino-atmega communication
lininoChars is a buffer of the last 3 characters to come through Serial1.
we are listening for these "magic strings":
%%% signals that the linino has booted
%%! comes from a cron job on every hour, we will run the trophy at this point */
char lininoChars[4] = "000";

void setup() {
	Serial1.begin(250000);

	scroller.begin();
	scroller.displayClear();
	scroller.displaySuspend(false);
}

void loop() {
	//read serial buffer
	while (Serial1.available() > 0) {
		char c = (char)Serial1.read();
		bufferLininoCommunication(c);
	}
  
	//only run when scroller is ready
	if (scroller.displayAnimate()){
    
		//if we are still waiting for linino to boot:
		if (!contactWithLinino) {
			// check boot complete
			if(hasLininoSaid("%%%") && !contactWithLinino){
				contactWithLinino = true;
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
      
			if(firstRun || hasLininoSaid("%%!")) {
				bufferLininoCommunication('0');
				firstRun = false;
        
				scrollNextTrophyMessage();
			} else {
				// hang out
				delay(10000);
			}
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

void bufferLininoCommunication(char nextChar) {
	// store the last three chars we've received over serial from linino
	lininoChars[0] = lininoChars[1];
	lininoChars[1] = lininoChars[2];
	lininoChars[2] = nextChar;
}

// http://stackoverflow.com/questions/20169474/char-array-comparison-in-c
short hasLininoSaid(char sequence[]) {
	int i;
  
	for(i = 0; i < strlen(lininoChars); i++) {
		if(lininoChars[i] != sequence[i]) {
			return 0;
		}
	}
  
	return 1;
}
