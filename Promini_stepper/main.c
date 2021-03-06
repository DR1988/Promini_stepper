/*
 * ProMini_step_1er.c
 *
 * Created: 27.04.2017 0:56:01
 * Author : DR
 */ 

#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define CLOCKWISE PIND & (1<<PD2)
#define PRESSED_STOP PIND & (1<<PD5)
#define clockWise true
#define antiClockWise false


int _step_1 = 0;
int _step_2 = 0;
int rotation = 0;
bool dir = clockWise;
bool move = false;
bool setToZero = true;

void setInitialState() {
	_step_1 = 0;
	rotation = 0;
	dir = clockWise;
	move = false;
}

void setToZeroPoint() {
	dir=antiClockWise;
	move = true;
	rotation=0;
}

void Stop() {
	move = false;
	PORTB &=~(1<<PB1);
	PORTB &=~(1<<PB0);
	PORTD &=~(1<<PD7);
	PORTD &=~(1<<PD6);
}

int main(void)
{
	DDRC = (1<<DDC3)|(1<<DDC2)|(1<<DDC1)|(1<<DDC0);
	DDRD = (1<<DDD7)|(1<<DDD6);
	DDRB |= (1<<DDB1)|(1<<DDB0);
	sei();
	EICRA |= (1<<ISC00);
	//|(1<<ISC00)
	EIMSK |= (1<<INT0);
	//|(1<<INT0);
	setToZeroPoint();

    /* Replace with your application code */
    while (1) 
    {
		if(PRESSED_STOP && (dir==antiClockWise)){
			if(setToZero) {
				dir=clockWise;
				move = true;
				setToZero = false;
				rotation=0;
			} else {
				Stop();
				setInitialState();			
			}
		}

		/* switch (_step_2)
		{
			case 0:
				PORTC |= (1<<PC3);
				PORTC |= (1<<PC2);
				PORTC &= ~(1<<PC1);
				PORTC &= ~(1<<PC0);
			break;
			case 1:
				PORTC &= ~(1<<PC3);
				PORTC |= (1<<PC2);
				PORTC |= (1<<PC1);
				PORTC &= ~(1<<PC0);
			break;
			case 2:
				PORTC &= ~(1<<PC3);
				PORTC &= ~(1<<PC2);
				PORTC |= (1<<PC1);
				PORTC |= (1<<PC0);
			break;
			case 3:
				PORTC |= (1<<PC3);
				PORTC &= ~(1<<PC2);
				PORTC &= ~(1<<PC1);
				PORTC |= (1<<PC0);
			break;
		}
		_step_2++;
		if(_step_2 > 3) {
			_step_2 = 0;
		}*/
			if (move) {
				switch (_step_1){
					case 0:
						PORTD |= (1<<PD7);
						PORTD &= ~(1<<PD6);
						PORTB |= (1<<PB0);
						PORTB &= ~(1<<PB1);
						break;
						case 1:
						PORTD &= ~(1<<PD7);
						PORTD &= ~(1<<PD6);
						PORTB |= (1<<PB0);
						PORTB |= (1<<PB1);
						break;
						case 2:
						PORTD &= ~(1<<PD7);
						PORTD |= (1<<PD6);
						PORTB &= ~(1<<PB0);
						PORTB |= (1<<PB1);
					break;
					case 3:
						PORTD |= (1<<PD7);
						PORTD |= (1<<PD6);
						PORTB &= ~(1<<PB0);
						PORTB &= ~(1<<PB1);
					break;
				}
	
				if(dir) {
					_step_1--;
					rotation++;
					if(rotation > 490) {
						//rotation=0;
						Stop();
						}
					} else {
					rotation--;
					_step_1++;
				}
				if(_step_1 < 0) {
					_step_1 = 3;
					} else if (_step_1 > 3) {
					_step_1=0;
				}

			}

			_delay_ms(2);
    }
}
		
	ISR(INT0_vect) {
	_delay_ms(5);
		if(CLOCKWISE){
			dir=antiClockWise;
			//rotation=0;
			move = true;
		}
		else {
			dir=clockWise;
			//rotation=0;
			move = true;
		}
	}
	//ISR(INT0_vect) {
		//Stop();
		//setInitialState();
	//}
