/*
Date: 21/11/2023

Microproccessors game project

Game name: The Key

By: Emmanuel Awode (C22320583) & Emmanuel Adeogun (C22395881)

Program Details:

The game is a mix of adventure and challenge, with the player navigating through levels, 
avoiding enemies, and collecting keys. 

The game will keep a scoreboard for the player through each iteration of the game, and will reset once the
game starts over. 
*/

#include <stm32f031x6.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define KEY_WIDTH 12
#define KEY_HEIGHT 16

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void setupIO(void);

void levelOne(void);
void levelTwo(void);
void levelThree(void);

// Displays the start screen menu
void startGame(void);

// Displays game over screen
void gameOver(void);

volatile uint32_t milliseconds;

//Clears screen
void displayBlack(void);

//Siganture to check if the player inside the enemy.
void playerInsideEnemy(uint16_t,uint16_t,uint16_t,uint16_t);

//Signature to check if enemy hit player
void enemyInsidePlayer(uint16_t,uint16_t,uint16_t,uint16_t);

// Player character
const uint16_t link1[] = 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,39178,39178,39178,39178,0,0,0,0,0,0,0,4118,4118,4118,7996,4118,4118,39178,39178,39178,39178,39178,39178,0,0,0,0,0,4118,4118,4118,4118,7996,7996,39178,39178,39178,39178,39178,39178,0,0,0,0,0,0,4118,0,4118,4118,7996,7996,7996,39178,7996,7996,4118,7996,0,0,39178,0,0,0,0,0,4118,39178,39178,7996,7996,39178,7996,7996,39178,7996,7996,7996,39178,0,0,0,0,0,0,39178,39178,39178,7996,7996,7996,7996,7996,7996,0,0,39178,0,0,0,0,0,0,0,4118,4118,4118,4118,7996,7996,7996,7996,0,0,39178,0,0,0,0,0,39178,4118,4118,4118,4118,4118,4118,4118,39178,39178,39178,7996,39178,0,0,0,0,39178,39178,39178,4118,7996,7996,7996,4118,4118,4118,39178,39178,7996,39178,0,0,0,0,39178,39178,39178,39178,7996,7996,7996,4118,4118,4118,39178,39178,0,39178,0,0,0,0,39178,39178,39178,39178,7996,7996,4118,4118,4118,39178,0,0,0,39178,0,0,0,0,0,4118,39178,39178,4118,4118,39178,39178,39178,39178,0,0,0,39178,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,39178,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39178,39178,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t link2[] = 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,39178,39178,39178,39178,0,0,0,0,0,0,0,4118,4118,4118,7996,4118,4118,39178,39178,39178,39178,39178,39178,0,0,0,0,0,4118,4118,4118,4118,7996,7996,39178,39178,39178,39178,39178,39178,0,0,0,0,0,0,4118,0,4118,4118,7996,7996,7996,39178,7996,7996,4118,7996,0,0,0,0,0,0,0,0,4118,39178,39178,7996,7996,39178,7996,7996,39178,7996,7996,7996,0,0,0,0,0,0,0,39178,39178,39178,7996,7996,7996,7996,7996,7996,0,39178,0,0,0,0,0,0,0,0,4118,4118,4118,4118,7996,7996,7996,7996,0,39178,0,0,0,0,0,0,4118,39178,39178,4118,4118,7996,7996,7996,39178,39178,7996,39178,0,0,0,0,0,0,39178,39178,39178,39178,39178,7996,7996,7996,4118,39178,7996,39178,0,0,0,0,0,4118,39178,39178,39178,39178,39178,7996,7996,4118,4118,39178,0,39178,0,0,0,0,0,4118,4118,39178,39178,39178,39178,4118,4118,4118,39178,0,0,39178,0,0,0,0,39178,39178,4118,4118,4118,4118,4118,39178,39178,39178,39178,4118,0,39178,0,0,0,0,39178,39178,39178,4118,4118,4118,4118,4118,4118,4118,4118,39178,39178,0,0,0,0,0,0,39178,39178,39178,0,0,0,0,0,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t link3[] = 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,7996,0,4118,39178,39178,39178,39178,39178,39178,4118,0,7996,0,0,0,0,0,0,7996,0,39178,39178,39178,39178,39178,39178,39178,39178,0,7996,0,0,0,0,0,0,7996,7996,39178,7996,4118,7996,7996,4118,7996,39178,7996,7996,0,0,0,0,0,0,7996,7996,39178,7996,39178,7996,7996,39178,7996,39178,7996,7996,0,0,0,0,0,0,0,7996,7996,7996,7996,7996,7996,7996,7996,7996,7996,39178,0,0,0,0,0,0,0,4118,4118,7996,7996,39178,39178,7996,7996,4118,4118,39178,0,0,0,0,0,39178,39178,39178,39178,39178,7996,7996,7996,7996,4118,4118,39178,39178,39178,0,0,0,39178,39178,7996,39178,39178,39178,39178,4118,4118,4118,4118,4118,7996,39178,39178,0,0,0,39178,7996,7996,7996,39178,39178,7996,39178,39178,4118,4118,7996,7996,7996,39178,0,0,0,39178,39178,7996,39178,39178,39178,7996,4118,39178,39178,39178,39178,7996,7996,7996,0,0,0,39178,39178,7996,39178,39178,39178,7996,39178,39178,4118,4118,4118,4118,7996,0,0,0,0,39178,39178,39178,39178,39178,39178,7996,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,7996,7996,7996,7996,7996,39178,0,0,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t link4[] = 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,7996,0,4118,4118,4118,4118,4118,4118,4118,4118,0,7996,0,0,0,0,0,0,7996,4118,4118,4118,4118,4118,4118,4118,4118,4118,4118,7996,0,0,0,0,0,0,7996,39178,4118,4118,4118,4118,4118,4118,4118,4118,39178,7996,0,0,0,0,0,0,7996,7996,39178,39178,4118,4118,4118,4118,39178,39178,7996,7996,0,0,0,0,0,0,0,7996,39178,39178,39178,4118,4118,39178,39178,39178,7996,0,0,0,0,0,0,0,0,39178,4118,39178,39178,39178,39178,39178,39178,4118,39178,0,0,0,0,0,0,0,0,39178,39178,4118,4118,4118,4118,4118,4118,4118,39178,39178,0,0,0,0,0,0,7996,39178,39178,4118,4118,4118,4118,4118,4118,4118,39178,39178,0,0,0,0,0,0,7996,39178,39178,4118,4118,4118,4118,4118,4118,39178,39178,0,0,0,0,0,0,0,7996,7996,4118,39178,39178,39178,39178,39178,39178,4118,4118,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,39178,4118,4118,4118,4118,4118,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,39178,39178,0,0,39178,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t link5[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,7996,0,4118,4118,4118,4118,4118,4118,4118,4118,0,7996,0,0,0,0,0,0,7996,4118,4118,4118,4118,4118,4118,4118,4118,4118,4118,7996,0,0,0,0,0,0,7996,39178,4118,4118,4118,4118,4118,4118,4118,4118,39178,7996,0,0,0,0,0,0,7996,7996,39178,39178,4118,4118,4118,4118,39178,39178,7996,7996,0,0,0,0,0,0,0,7996,39178,39178,39178,4118,4118,39178,39178,39178,7996,0,0,0,0,0,0,0,0,39178,4118,39178,39178,39178,39178,39178,39178,4118,39178,0,0,0,0,0,0,0,39178,39178,4118,4118,4118,4118,4118,4118,4118,39178,39178,0,0,0,0,0,0,0,39178,39178,4118,4118,4118,4118,4118,4118,4118,39178,39178,7996,0,0,0,0,0,0,0,39178,39178,4118,4118,4118,4118,4118,4118,39178,39178,7996,0,0,0,0,0,0,0,4118,4118,39178,39178,39178,39178,39178,39178,4118,7996,7996,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,39178,39178,39178,4118,4118,4118,4118,4118,39178,0,0,0,0,0,0,0,0,0,39178,39178,39178,39178,0,0,39178,39178,0,0,0,0,0,0,0,0,0,0,0,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t link6[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,0,0,0,4118,4118,4118,4118,4118,4118,4118,4118,0,0,0,0,0,0,0,0,7996,0,4118,39178,39178,39178,39178,39178,39178,4118,0,7996,0,0,0,0,0,0,7996,0,39178,39178,39178,39178,39178,39178,39178,39178,0,7996,0,0,0,0,0,0,7996,7996,39178,7996,4118,7996,7996,4118,7996,39178,7996,7996,0,0,0,0,0,0,7996,7996,39178,7996,39178,7996,7996,39178,7996,39178,7996,7996,0,0,0,0,0,0,0,7996,7996,7996,7996,7996,7996,7996,7996,7996,7996,39178,0,0,0,0,0,0,0,0,4118,7996,7996,39178,39178,7996,7996,4118,39178,39178,0,0,0,0,0,0,39178,39178,39178,39178,39178,7996,7996,7996,4118,4118,4118,7996,0,0,0,0,0,39178,39178,7996,39178,39178,39178,39178,4118,4118,4118,4118,4118,7996,0,0,0,0,0,39178,7996,7996,7996,39178,39178,7996,39178,39178,4118,4118,39178,0,0,0,0,0,0,39178,39178,7996,39178,39178,39178,7996,4118,39178,39178,39178,4118,0,0,0,0,0,0,39178,39178,7996,39178,39178,39178,7996,39178,39178,4118,4118,4118,0,0,0,0,0,0,39178,39178,39178,39178,39178,39178,7996,4118,4118,4118,39178,0,0,0,0,0,0,0,0,7996,7996,7996,7996,7996,0,0,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39178,39178,39178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t darknut1[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40959,168,168,168,168,168,0,0,0,0,0,0,0,0,0,168,40959,40959,35836,35836,35836,168,0,0,0,0,0,0,0,0,0,35836,40959,40959,35836,35836,35836,35836,0,0,0,0,0,0,0,0,0,35836,35836,35836,35836,168,168,168,0,0,0,0,0,0,0,0,35836,35836,35836,35836,35836,35836,35836,168,35836,168,0,0,0,0,0,0,35836,35836,40959,40959,35836,35836,35836,168,35836,168,0,0,0,0,0,0,168,40959,40959,40959,40959,35836,168,168,35836,168,0,0,0,0,0,168,35836,168,168,35836,168,168,168,168,35836,168,0,0,0,0,168,168,35836,168,35836,35836,168,168,40959,40959,40959,40959,40959,0,0,0,168,168,168,168,35836,35836,168,168,40959,40959,40959,40959,40959,40959,0,168,168,168,35836,35836,168,168,168,168,40959,40959,40959,40959,40959,0,168,168,168,168,35836,35836,35836,35836,35836,168,35836,0,35836,168,0,0,0,0,168,168,168,35836,35836,35836,35836,35836,168,168,168,168,0,0,0,0,0,168,168,168,0,0,168,168,168,168,0,0,0,0,0,0,0,168,168,168,168,0,0,168,168,0,0,0,0,0,
};

const uint16_t darknut2[]=
{
0,0,0,0,0,0,40959,168,168,168,168,168,0,0,0,0,0,0,0,0,0,168,40959,40959,35836,35836,35836,168,0,0,0,0,0,0,0,0,0,35836,40959,40959,35836,35836,35836,35836,0,0,0,0,0,0,0,0,0,35836,35836,35836,35836,168,168,168,0,0,0,0,0,0,0,0,35836,35836,35836,35836,35836,35836,35836,168,0,0,0,0,0,0,0,0,35836,35836,40959,40959,35836,35836,35836,168,35836,168,0,0,0,0,0,0,168,40959,40959,40959,40959,35836,168,168,35836,168,0,0,0,0,0,168,35836,168,168,35836,168,168,168,168,35836,168,0,0,0,0,0,168,35836,168,35836,35836,168,168,40959,40959,40959,40959,40959,0,0,0,168,168,168,168,35836,35836,168,168,40959,40959,40959,40959,40959,40959,0,0,168,168,35836,35836,168,168,168,168,40959,40959,40959,40959,40959,0,0,168,168,168,35836,35836,35836,35836,35836,168,35836,0,35836,168,0,0,0,168,168,168,168,35836,35836,35836,35836,35836,0,0,35836,168,0,0,168,168,168,168,168,168,168,168,168,168,0,0,35836,168,0,0,0,0,0,0,0,0,168,168,168,168,0,0,0,0,0,0,0,0,0,0,0,0,168,168,168,168,168,0,0,0,0,0,
};

const uint16_t key1[]=
{
0,0,0,0,39972,39972,39972,13857,0,0,0,0,0,0,0,65535,39972,39972,39972,39972,13857,0,0,0,0,0,65535,39972,13857,0,0,39972,39972,13857,0,0,0,0,65535,13857,0,0,0,0,39972,13857,0,0,0,0,65535,13857,0,0,0,0,39972,13857,0,0,0,0,39972,39972,39972,39972,39972,39972,39972,13857,0,0,0,0,65535,39972,39972,39972,39972,13857,13857,13857,0,0,0,0,0,0,0,39972,13857,0,0,0,0,0,0,0,0,0,0,39972,13857,0,0,0,0,0,0,0,0,0,0,39972,13857,0,0,0,0,0,0,0,0,0,0,39972,13857,0,0,0,0,0,0,0,0,65535,39972,39972,13857,0,0,0,0,0,0,0,0,39972,39972,39972,13857,0,0,0,0,0,0,0,0,0,39972,39972,13857,0,0,0,0,0,0,0,0,39972,39972,39972,13857,0,0,0,0,0,0,0,0,0,0,39972,13857,0,0,0,0,0,
};

uint16_t x = 50;
uint16_t y = 50;
uint16_t oldx = 0;	
uint16_t oldy = 0;
int score = 0;
int hinverted = 0;
int vinverted = 0;
int toggle = 0;
int hmoved = 0;
int vmoved = 0;
int xmoved = 0;
int keycount1 = 0;
int keycount2 = 0;
int keycount3 = 0;
int key_x = 0;
int key_y = 0;
int i = 0;
int start;
uint16_t enemyX = 80;
uint16_t enemyY = 10;
uint16_t enemy2X = 140;
uint16_t enemy2Y = 64;
//Enemy Movement
int deltaX = 1;
int delta2X = -1;
	
int main()
{
	initClock();
	initSysTick();
	setupIO();
	initSound();
	uint16_t oldx = x;	
	uint16_t oldy = y;
	while(1)
	{
		startGame();
	}

	return 0;
}


void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}

void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}

void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}

int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}

	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	pinMode(GPIOB, 1, 1);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}

void enemyInsidePlayer(uint16_t enemyX,uint16_t enemyY,uint16_t x ,uint16_t y)
{
	// check for an overlap by checking to see if ANY of the 4 corners of Link are within the enemy area
	if (isInside(x,y,12,16,enemyX,enemyY) || isInside(x,y,12,16,enemyX+12,enemyY) || isInside(x,y,12,16,enemyX,enemyY+16) || isInside(x,y,12,16,enemyX+12,enemyY+16) )
	{
		gameOver();
	}
}

void playerInsideEnemy(uint16_t x,uint16_t y,uint16_t enemyX ,uint16_t enemyY)
{
	// check for an overlap by checking to see if ANY of the 4 corners of Link are within the enemy area
	if (isInside(enemyX,enemyY,12,16,x,y) || isInside(enemyX,enemyY,12,16,x+12,y) || isInside(enemyX,enemyY,12,16,x,y+16) || isInside(enemyX,enemyY,12,16,x+12,y+16) )
	{
		gameOver();
	}
}

void displayBlack(void)
{
	// clears whole screen
	fillRectangle(0,0,128,160,0);
	fillRectangle(40,0,45,179,0);
	fillRectangle(0,65,128,20,0);
}

void startGame()
{	
	uint16_t x = 50;
	uint16_t y = 50;

	// When this function is recalled, this will reset the score
	score = 0;
	printText(" WELCOME TO...  ", 15, 20, RGBToWord(0,0xff,0), 0);
	printText("   THE KEY  ", 15, 30, RGBToWord(0,0xff,0), 0);

    // Opening melody
    playNote(C4); delay(250);
    playNote(D4); delay(250);
    playNote(E4); delay(250);
    playNote(F4); delay(250);
    playNote(G4); delay(250);

    playNote(E4); delay(250);
    playNote(F4); delay(250);
    playNote(G4); delay(500);

    playNote(G4); delay(250);
    playNote(A4); delay(250);
    playNote(B4); delay(250);
    playNote(C5); delay(250);
    playNote(D5); delay(250);

    playNote(C5); delay(250);
    playNote(B4); delay(250);
    playNote(A4); delay(250);
    playNote(G4); delay(500);

	// Start game menu
	while (1)
	{
		playNote(0);
		// Displays a constantly appearing and reappearing arrow, along with a auto moving link
		if (toggle){
			putImage(x,y,18,18,link1,hinverted,0);
			printText("Press Up To Start", 0, 100, RGBToWord(0xff,0xff,0), 0);
			printText("^^^", 50, 120, RGBToWord(0,0xff,0), 0);
			printText("|||", 50, 124, RGBToWord(0,0xff,0), 0);
		}
		else{
			putImage(x,y,18,18,link2,hinverted,0);
			printText("                      ", 0, 100, RGBToWord(0xff,0xff,0), 0);
		}
		toggle = toggle ^ 1;

		// Commences to level one once up is pressed
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{
			displayBlack();
			levelOne();
		}
	}
}

void levelOne()
{
	start = 0;
	// Generates random x and y from the screen width/height to the key width/height
	key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
	key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
	while(1)
	{	
		// Displays the level, the start variable is used to display each level in every function
		if (start == 0)
		{
			printText("Level 1", 0, 0, RGBToWord(0xff,0xff,0xff), 0);

			// Level 1 melody
			playNote(C4); delay(250);
			playNote(D4); delay(250);
			playNote(E4); delay(500);

			playNote(E4); delay(250);
			playNote(F4); delay(250);
			playNote(G4); delay(500);

			playNote(G4); delay(250);
			playNote(F4); delay(250);
			playNote(E4); delay(250);
			playNote(D4); delay(250);
			playNote(C4); delay(500);

			displayBlack();
			playNote(0);

			start++; // incremented in order to not display it again in the while loop
		}
			
		hmoved = vmoved = xmoved = 0;
		hinverted = vinverted = 0;

		// Different notes are played per movement
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 110)
			{
				playNote(C4);
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}						
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			if (x > 10)
			{
				playNote(D4);
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}	
		}
		if ((GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{

			if (y < 140)
			{
				playNote(E4);
				y = y + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}
		if ((GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{
			if (y > 16)
			{
				playNote(F4);
				y = y - 1;
				xmoved = 1;
				vinverted = 0;
			}
		}

		if ((vmoved) || (hmoved) || (xmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;	

			if (hmoved)
			{
				if (toggle)
					putImage(x,y,18,18,link1,hinverted,0);
				else
					putImage(x,y,18,18,link2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else if (vmoved)
			{

				if (toggle)
					putImage(x,y,18,18,link3,hinverted,0);
				else
					putImage(x,y,18,18,link6,hinverted,0);

				toggle = toggle ^ 1;
			}
			else
			{
				if (toggle)
					putImage(x,y,18,18,link4,hinverted,0);
				else
					putImage(x,y,18,18,link5,hinverted,0);

				toggle = toggle ^ 1;
			}
			
			playNote(0); // stop sound when no button is pressed
			// Won't move on to next kwy until the first has been obtained
			if (keycount1 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
				
				// Now check for an overlap by checking to see if ANY of the 4 corners of link are within the target area
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("Keys Obtained: 1", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount1++;
					score = score + 15; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}
			
			if (keycount1 != 0 && keycount2 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
				
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Keys Obtained: 2", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount2++;
					score = score + 15; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}

			if (keycount2 != 0 && keycount3 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
					
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Keys Obtained: 3", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount3++;
					score = score + 20; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}

			if ((keycount1 != 0) && (keycount2 != 0) && (keycount3 != 0))
			{
				// resets keycount and moves onto next level
				keycount1 = keycount2 = keycount3 = 0;
				levelTwo();

			}
			
		}		
		delay(30);

	}
}

void levelTwo()
{
	start = 0;
	// Generates random x and y from the screen width/height to the key width/height
	key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
	key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
	
	while(1)
	{	
		if (start == 0)
		{	
			printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
			printText("Level 2", 0, 0, RGBToWord(0xff,0xff,0xff), 0);

		    // Level 2 melody
			playNote(G4); delay(200);
			playNote(A4); delay(200);
			playNote(B4); delay(200);
			playNote(C5); delay(400);

			playNote(C5); delay(200);
			playNote(B4); delay(200);
			playNote(A4); delay(200);
			playNote(G4); delay(400);

			playNote(E4); delay(200);
			playNote(F4); delay(200);
			playNote(G4); delay(600);
			
			printText("              ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
			playNote(0);

			start++;
		}

		//Enemy Movement
		if (enemyX > 100)
		{
			deltaX = -1;
		}
		
		if (enemyX < 1)
		{
			deltaX = 1;
		}

		fillRectangle(enemyX,enemyY,16,16,RGBToWord(0,0,0));
		
		// increments enemy movement
		enemyX = enemyX + deltaX;

		putImage(enemyX,enemyY,16,16,darknut1,0,0);

		// checks if enemy is touching player
		enemyInsidePlayer(enemyX,enemyY,x,y);

		hmoved = vmoved = xmoved = 0;
		hinverted = vinverted = 0;

		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 110)
			{
				playNote(C4);
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}	
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			if (x > 10)
			{
				playNote(D4);
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}	
		}
		if ((GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{

			if (y < 140)
			{
				playNote(E4);
				y = y + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}
		if ((GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{
			if (y > 16)
			{
				playNote(F4);
				y = y - 1;
				xmoved = 1;
				vinverted = 0;
			}
		}

		if ((vmoved) || (hmoved) || (xmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;	

			if (hmoved)
			{
				if (toggle)
					putImage(x,y,18,18,link1,hinverted,0);
				else
					putImage(x,y,18,18,link2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else if (vmoved)
			{

				//putImage(x,y,18,18,link3,vinverted,0);
				if (toggle)
					putImage(x,y,18,18,link3,hinverted,0);
				else
					putImage(x,y,18,18,link6,hinverted,0);
				toggle = toggle ^ 1;
			}
			else
			{
				if (toggle)
					putImage(x,y,18,18,link4,hinverted,0);
				else
					putImage(x,y,18,18,link5,hinverted,0);
				toggle = toggle ^ 1;
			}

			playNote(0); // stop sound when no button is pressed

			// checks if player is touching enemy
			playerInsideEnemy(x,y,enemyX,enemyY);
			
			// Won't move on to next kwy until the first has been obtained
			if (keycount1 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
				
				// Now check for an overlap by checking to see if ANY of the 4 corners of link are within the target area
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,14,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("Keys Obtained: 1", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount1++;
					score = score + 15; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}
			
			if (keycount1 != 0 && keycount2 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
				
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Keys Obtained: 2", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount2++;
					score = score + 15; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}

			if (keycount2 != 0 && keycount3 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
					
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Keys Obtained: 3", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount3++;
					score = score + 20; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}

			if ((keycount1 != 0) && (keycount2 != 0) && (keycount3 != 0))
			{
				// resets keycount
				keycount1 = keycount2 = keycount3 = 0;
				// Next level
				levelThree();
			}
			
		}		
		delay(25);
	}
}

void levelThree()
{
	start = 0;
	// Generates random x and y from the screen width/height to the key width/height
	key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
	key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);	

	while(1)
	{	
		if (start == 0)
		{	
			printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
			printText("Level 3", 0, 0, RGBToWord(0xff,0xff,0xff), 0);

		    // Level 3 melody (Final level)
			playNote(E4); delay(150);
			playNote(G4); delay(150);
			playNote(B4); delay(300);

			playNote(B4); delay(150);
			playNote(A4); delay(150);
			playNote(G4); delay(300);

			playNote(G4); delay(150);
			playNote(F4); delay(150);
			playNote(E4); delay(300);

			playNote(C5); delay(150);
			playNote(B4); delay(150);
			playNote(A4); delay(300);

			playNote(G4); delay(300);
			playNote(F4); delay(300);
			playNote(E4); delay(600);

			printText("              ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
			playNote(0);

			start++;
		}

		//Enemy Movement
		if (enemyX > 100)
		{
			deltaX = -1;
		}
		if (enemy2X > 100)
		{
			delta2X = 1;
		}
		
		// 2nd Enemy Movement
		if (enemyX < 1)
		{
			deltaX = 1;
		}
		if (enemy2X < 1)
		{
			delta2X = -1;
		}

		fillRectangle(enemyX,enemyY,16,16,RGBToWord(0,0,0));
		fillRectangle(enemy2X,enemy2Y,16,16,RGBToWord(0,0,0));
		
		// increments enemies movements
		enemyX = enemyX + deltaX;
		enemy2X = enemy2X - delta2X;

		putImage(enemyX,enemyY,16,16,darknut1,0,0);
		putImage(enemy2X,enemy2Y,16,16,darknut1,0,0);

		// checks if enemy is touching player
		enemyInsidePlayer(enemyX,enemyY,x,y);
		enemyInsidePlayer(enemy2X,enemy2Y,x,y);

		hmoved = vmoved = xmoved = 0;
		hinverted = vinverted = 0;

		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					
			if (x < 110)
			{
				playNote(C4);
				x = x + 1;
				hmoved = 1;
				hinverted=0;
			}	
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			if (x > 10)
			{
				playNote(D4);
				x = x - 1;
				hmoved = 1;
				hinverted=1;
			}	
		}
		if ((GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{

			if (y < 140)
			{
				playNote(E4);
				y = y + 1;			
				vmoved = 1;
				vinverted = 0;
			}
		}
		if ((GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{
			if (y > 16)
			{
				playNote(F4);
				y = y - 1;
				xmoved = 1;
				vinverted = 0;
			}
		}
		
		if ((vmoved) || (hmoved) || (xmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;	

			if (hmoved)
			{
				if (toggle)
					putImage(x,y,18,18,link1,hinverted,0);
				else
					putImage(x,y,18,18,link2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else if (vmoved)
			{

				//putImage(x,y,18,18,link3,vinverted,0);
				if (toggle)
					putImage(x,y,18,18,link3,hinverted,0);
				else
					putImage(x,y,18,18,link6,hinverted,0);
				toggle = toggle ^ 1;
			}
			else
			{
				if (toggle)
					putImage(x,y,18,18,link4,hinverted,0);
				else
					putImage(x,y,18,18,link5,hinverted,0);
				toggle = toggle ^ 1;
			}

			playNote(0); // stop sound when no button is pressed

			// checks if player is touching enemy
			playerInsideEnemy(x,y,enemyX,enemyY);
			
			// Won't move on to next kwy until the first has been obtained
			if (keycount1 == 0)
			{
				putImage(key_x,key_y,12,14,key1,0,0);
				
				// Now check for an overlap by checking to see if ANY of the 4 corners of link are within the target area
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+14) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("Keys Obtained: 1", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount1++;
					score = score + 15; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}
			
			if (keycount1 != 0 && keycount2 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
				
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Keys Obtained: 2", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount2++;
					score = score + 15; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}

			if (keycount2 != 0 && keycount3 == 0)
			{
				putImage(key_x,key_y,12,16,key1,0,0);
					
				if (isInside(key_x,key_y,12,16,x,y) || isInside(key_x,key_y,12,16,x+12,y) || isInside(key_x,key_y,12,16,x,y+16) || isInside(key_x,key_y,12,16,x+12,y+16))
				{
					printText("                                                                             ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Keys Obtained: 3", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(300);
					fillRectangle(key_x,key_y,12,16, 0);
					keycount3++;
					score = score + 20; // add to score
					printText("                 ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					printText("Score: ", 0, 0, RGBToWord(0xff,0xff,0xff), 0);
					// Prints score out
					printNumber(score, 90, 0, RGBToWord(0xff,0xff,0xff), 0);
					delay(500);
					// Generates random x and y from the screen width/height to the key width/height
					key_x = rand() % (SCREEN_WIDTH - KEY_WIDTH);
					key_y = rand() % (SCREEN_HEIGHT - KEY_HEIGHT);
				}
			}

			if ((keycount1 != 0) && (keycount2 != 0) && (keycount3 != 0))
			{
				keycount1 = keycount2 = keycount3 = 0;
				// Game complete
				fillRectangle(0,0,128,160,0);
				printText("COMPLETE!", 0, 0, RGBToWord(0xff,0xff,0), 0);

			    // Game Completion melody
				playNote(C4); delay(200);
				playNote(D4); delay(200);
				playNote(E4); delay(200);
				playNote(G4); delay(400);

				playNote(E4); delay(200);
				playNote(G4); delay(200);
				playNote(A4); delay(400);

				playNote(G4); delay(200);
				playNote(E4); delay(200);
				playNote(C4); delay(200);
				playNote(G4); delay(400);

				playNote(C5); delay(200);
				playNote(B4); delay(200);
				playNote(G4); delay(200);
				playNote(E4); delay(200);
				playNote(C4); delay(400);

				fillRectangle(0,0,128,160,0);
				playNote(0);
				printText("Restarting Game...", 0, 0, RGBToWord(0xff,0xff,0), 0);
				delay(500);

				// Restarts game
				fillRectangle(0,0,128,160,0);
				startGame();
			}
			
		}		
		delay(20);
	}

}

// Displays game over and prompts the player to restart
void gameOver()
{
	fillRectangle(0,0,128,160,0);
	while ( (GPIOA->IDR & (1 << 8)) != 0) // up not pressed
	{
		printText("Game Over... ", 30, 20, RGBToWord(0xff,0xff,0), 0);
		printText("Better Luck", 0, 140, RGBToWord(0xff,0xff,0), 0);
		printText("Next Time...", 0, 150, RGBToWord(0xff,0xff,0), 0);
		printText("Press up to redo", 10, 100, RGBToWord(0xff,0xff,0), 0);

		// Game Over music
		playNote(C5); delay(500);
		playNote(A2); delay(500);
		playNote(G2); delay(500);
		playNote(C3); delay(500);

		// Restarts game if up is pressed
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{
			playNote(0);
			displayBlack();
			startGame();
		}
	}
	
}