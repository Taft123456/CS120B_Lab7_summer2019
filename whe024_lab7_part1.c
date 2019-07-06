#include <avr/io.h>
#include "io.h"

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 

void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff()
{
	TCCR1B = 0x00;
}
void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States {INIT, START, INCR, DECR, WAIT, RESET} state;
unsigned char display = 0x00;

void Tick()
{
	switch(state) 
	{
		case INIT:		
			LCD_Cursor(1);
			LCD_WriteData('0');
			state = START;
			break;		
		case START:		
			if((~PINA & 0x03) == 0x01)
			{
				state = INCR; 
				break;
			}
			else if((~PINA & 0x03) == 0x02)
			{
				state = DECR; 
				break;
			}
			else if((~PINA & 0x03) == 0x03)
			{
				state = RESET; 
				break;
			}
			else
			{
				state = START; 
				break;
			}		
		case INCR:	
			state = WAIT; 
			break;
		case DECR:		
			state = WAIT; 
			break;		
		case WAIT:	
			if((~PINA & 0x03) == 0x01)
			{
				state = INCR; 
				break;
			}
			else if((~PINA & 0x03) == 0x02)
			{
				state = DECR; 
				break;
			}
			else if((~PINA & 0x03) == 0x03)
			{
				state = RESET; 
				break;
			}
			else if((~PINA & 0x03) == 0x00)
			{
				state = START; 
				break;
			}
			else
			{
				state = WAIT; 
				break;
			}
		case RESET:
			if((~PINA & 0x03) == 0x00)
			{
				state = START; 
				break;
			}
			else
			{
				state = RESET; 
				break;
			}		
		default:
			break;
	}	
	
	switch(state) 
	{
		case INIT:
			break;		
		case START:
			break;		
		case INCR:		
			if(display >= 9)
			{
				display = 9;
			}
			else
			{
				++display;
			}
			LCD_Cursor(1);
			LCD_WriteData(display + '0');
			break;		
		case DECR:		
			if(display <= 0)
			{
				display = 0;
			}
			else
			{
				--display;
			}
			LCD_Cursor(1);
			LCD_WriteData(display + '0');
			break;		
		case WAIT:
			break;		
		case RESET:	
			display = 0;
			LCD_Cursor(1);
			LCD_WriteData(display + '0');
			break;
		
	}
}

int main(void)
{
	DDRA = 0x00; 
	DDRC = 0xFF;  
	DDRD = 0xFF;  
	PORTA = 0xFF;
	PORTC = 0x00;
	PORTD = 0x00;
	
	LCD_init();
	
	TimerSet(500);
	TimerOn();
	
	while(1)
	{
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}



