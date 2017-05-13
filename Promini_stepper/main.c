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

#define CLOCKWISE PIND & (1<<PD3)
#define PRESSED_STOP PIND & (1<<PD2)
#define clockWise false
#define antiClockWise true

int _step_1 = 0;
int _step_2 = 0;
int rotation = 0;
bool dir = clockWise;
bool move = false;

void setInitialState() {
	_step_1 = 0;
	rotation = 0;
	dir = clockWise;
	move = false;
}

void setToZeroPoint() {
	dir=clockWise;
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
	EICRA |= (1<<ISC10);
	//|(1<<ISC00)|(1<<ISC01);
	EIMSK |= (1<<INT1);
	//|(1<<INT0);
	setToZeroPoint();

    /* Replace with your application code */
    while (1) 
    {
		if(PRESSED_STOP && (dir==clockWise)){
			Stop();
			setInitialState();
		}

		switch (_step_2)
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
					_step_1++;
					rotation++;
					} else {
					_step_1--;
				}
				if(_step_1 > 3) {
					_step_1 = 0;
					} else if (_step_1 < 0) {
					_step_1=3;
				}
				if(rotation > 530) {
					rotation=0;
					Stop();
				}
			}
			_step_2++;
			if(_step_2 > 3) {
				_step_2 = 0;
			}
			_delay_ms(2);
    }
}
		
	ISR(INT1_vect) {
	_delay_ms(5);
		if(CLOCKWISE){
			dir=antiClockWise;
			rotation=0;
			move = true;
		}
		else {
			dir=clockWise;
			rotation=0;
			move = true;
		}
	}
	//ISR(INT0_vect) {
		//Stop();
		//setInitialState();
	//}
