// These tutorials helped: 
// https://www.allaboutcircuits.com/technical-articles/how-to-a-162-lcd-module-with-an-mcu/
// https://openlabpro.com/guide/custom-character-lcd-pic/


#include <msp430g2553.h>


const unsigned char RS_Pin = 0x01;
const unsigned char RW_Pin = 0x02;
const unsigned char E_Pin = 0x04;

unsigned char dino1[] = {0x06, 0x05, 0x07, 0x06, 0x0F, 0x16, 0x15, 0x05};
unsigned char dino2[] = {0x06, 0x05, 0x07, 0x06, 0x0F, 0x16, 0x12, 0x02}; 
unsigned char cactus[] = {0x04, 0x14, 0x15, 0x1D, 0x07, 0x04, 0x04, 0x04};


int score = 0;


void stupidDelay();
void writeInstruction(unsigned char insCode);
void writeData(unsigned char dataCode);
void initializeScreen();
void clear();
void customChar(unsigned char *charArray, const char loc);
void demoAnimation();
void newFrame();
void printScore();
void initGame();


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD; 
	initializeScreen();
	customChar(dino1, 0);
	customChar(dino2, 1);
	customChar(cactus, 2);
	writeInstruction(0xC8);
	writeData(2);
	score = 246;
	printScore();

}

void initGame(){

}

void printScore(){
	int ones = score % 10;
	int tens = ((score % 100) - ones) / 10;
	int hundreads = (score - (score % 100)) / 100; 
	writeInstruction(0x80);
	writeData(0x30 + hundreads);
	writeInstruction(0x81);
	writeData(0x30 + tens);
	writeInstruction(0x82);
	writeData(0x30 + ones);
}

void newFrame(){
	for(int j = 0; j <20000; j++);
	writeInstruction(0x01);
	stupidDelay();
}

void stupidDelay(){
	for(int i = 0; i < 100; i++);
}

void writeInstruction(unsigned char insCode){
	stupidDelay();
	P1OUT &= ~RS_Pin;
	P1OUT &= ~RW_Pin;
	P1OUT &= ~E_Pin;
	P2OUT = insCode;
	P1OUT |= E_Pin;
	P1OUT &= ~E_Pin;
}

void writeData(unsigned char dataCode){
	stupidDelay();
	P1OUT |= RS_Pin;
	P1OUT &= ~RW_Pin;
	P1OUT &= ~E_Pin;
	P2OUT = dataCode;
	P1OUT |= E_Pin;
	P1OUT &= ~E_Pin;

	
}

void initializeScreen(){
	stupidDelay();
	P1SEL &= 0x00;
	P2SEL &= 0x00;
	P1DIR |= 0xFF;
	P2DIR |= 0xFF;
	writeInstruction(0x01);
	writeInstruction(0x02);
	writeInstruction(0x0C);
	writeInstruction(0x38);
}

void clear(){
	stupidDelay();
	writeInstruction(0x01);
	writeInstruction(0x02);
}

void customChar(unsigned char *charArray, const char loc){
	writeInstruction(0x40 + loc * 8);
	for(int i = 0; i < 8; i++){
		writeData(charArray[i]);
	}
}

void demoAnimation() {
	unsigned char dino1[] = {0x06, 0x05, 0x07, 0x06, 0x0F, 0x16, 0x15, 0x05};
	unsigned char dino2[] = {0x06, 0x05, 0x07, 0x06, 0x0F, 0x16, 0x12, 0x02}; 
	customChar(dino1, 0);
	customChar(dino2, 1);
	for(int i = 0; i < 17; i++){
		newFrame();
		writeInstruction(0xC0 + i);
		if(i % 2 == 0){
			writeData(0);
		} else{
			writeData(1);
		}
	}
	
}





