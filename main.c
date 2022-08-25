// These tutorials helped: 
// https://www.allaboutcircuits.com/technical-articles/how-to-a-162-lcd-module-with-an-mcu/
// https://openlabpro.com/guide/custom-character-lcd-pic/
// https://svijaykoushik.github.io/blog/2019/10/04/three-awesome-ways-to-generate-random-number-in-javascript/


#include <msp430g2553.h>



//constants to control the three control pins
const unsigned char RS_Pin = 0x01;
const unsigned char RW_Pin = 0x02;
const unsigned char E_Pin = 0x04;

unsigned char board[16][20];

long int X;
long int a = 1664525;
long int c = 1013904223;
long long int m = 4294967296;

int random(){
	X = (a * X + c) % m;
    return X;
}

void setBoardStartPos(){
	
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 20; j++){
			int rand = random();
			if(rand % 3 == 0){
				board[i][j] = 'A';
			} else{
				board[i][j] = 'D';
			}
		}
	}
	
}

void delayUpdate(){
	__delay_cycles(16000);
}

void writeInstruction(unsigned char instruction){
	delayUpdate();
	P1OUT &= ~RS_Pin;	//Set register select to low for instruction
	P1OUT &= ~E_Pin; //Set enable pin low;
	P2OUT = instruction;
	P1OUT |= E_Pin; //Set enable pin high the low to generate falling edge
	P1OUT &= ~E_Pin;
}

void initialize(){
	WDTCTL = WDTPW + WDTHOLD;
	P1SEL = 0x00;
	P2SEL = 0x00;
	P1DIR = 0xFF;
	P2DIR = 0xFF;
	delayUpdate();
	P1OUT &= ~RW_Pin; //Hold read/write pin low, since will always be writing 
	writeInstruction(0x01);
	writeInstruction(0x02);
	writeInstruction(0x0C);
	writeInstruction(0x38);
}

void writeData(unsigned char data){
	delayUpdate();
	P1OUT |= RS_Pin;
	P1OUT &= ~E_Pin;
	P2OUT = data;
	P1OUT |= E_Pin;
	P1OUT &= ~E_Pin;
}

//function to determine if each cell will be alive or dead after the next generation
void aliveOrDead(int y, int x){
    int count = 0;
    if (y > 0 && x > 0 && board[y-1][x-1] == 'A' || board[y-1][x-1] == 'B'){
        count ++;
    }
    if (y > 0 && board[y-1][x] == 'A' || board[y-1][x] == 'B'){
        count ++;
    }
    if (y > 0 && x < 19 && board[y-1][x+1] == 'A' || board[y-1][x+1] == 'B'){
        count ++;
    }
    if (x > 0 && board[y][x-1] == 'A' || board[y][x-1] == 'B'){
        count ++;
    }
    if (x < 19 && board[y][x+1] == 'A' || board[y][x+1] == 'B'){
        count ++;
    }
    if (y < 15 && x > 0 && board[y+1][x-1] == 'A' || board[y+1][x-1] == 'B'){
        count ++;
    }
    if (y < 15 && board[y+1][x] == 'A' || board[y+1][x] == 'B'){
        count ++;
    }
    if (y < 15 && x < 19 && board[y+1][x+1] == 'A' || board[y+1][x+1] == 'B'){
        count ++;
    }

    if (board[y][x] == 'A' && (count < 2 || count > 3)){
        board[y][x] = 'A' + 0x01;
    } else if (board[y][x] == 'D' && count == 3){
        board[y][x] = 'D' + 0x01;
    }
}

void next_gen(){
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 20; j++){
            aliveOrDead(i,j);
        }
    }
}

void customChar(unsigned char *charArray, const char loc){
	writeInstruction(0x40 + loc * 8);
	for(int i = 0; i < 8; i++){
		writeData(charArray[i]);
	}
}

int twoPower(int power){
	int sum = 1;
	for(int i = 0; i < power; i++){
		sum = sum * 2;
	}
	return sum;
}

void drawBoard(){

	for(int g = 0; g < 2; g++){
		for(int h = 0; h < 4; h++){
			unsigned char toDraw[8];
			for(int i = g * 8; i < 8 + g * 8; i++){
				unsigned char sum = 0;
				for(int j = h * 5; j < 5 + h * 5; j++){
					if(board[i][j] == 'B'){
						board[i][j] = 'D';
					}
					if(board[i][j] == 'E'){
						board[i][j] = 'A';
					}
					if(board[i][j] == 'A'){
						sum += twoPower(4 - j % 5);
					}
				}
				toDraw[i % 8] = sum;
			}

			customChar(toDraw, h + g * 4);
			if(g == 0){
				writeInstruction(0x86 + h);
			} else{
				writeInstruction(0xC6 + h);
			}
			writeData(h + g * 4);
		}
	}
}



int main(){
	initialize();
	setBoardStartPos();
	while(1){
		drawBoard();
		next_gen();
	}


	
	return 0;
}





