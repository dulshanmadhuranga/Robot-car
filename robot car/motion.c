#define F_CPU 16000000UL
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

void motion(unsigned char A0,unsigned char A1,unsigned char B0,unsigned char B1);
void motion_init(void);
	
int main(void){
	
	motion_init();
	
	while(1){
		/*motion(0,0,100,100);//backward
		_delay_ms(2000);*/
		
		/*motion(100,100,0,0);//forward
		_delay_ms(2000);*/
		
		motion(100,0,0,100);//right
		_delay_ms(2000);
		
		motion(100,100,0,0);//forward
		_delay_ms(2000);
		
		motion(0,100,100,0);//left
		_delay_ms(2000);
		
		motion(100,100,0,0);//forward
		_delay_ms(2000);
		
		motion(0,100,100,0);//left
		_delay_ms(2000);
		
		motion(100,100,0,0);//long_forward
		_delay_ms(4000);
		
		motion(0,100,100,0);//left
		_delay_ms(2000);
		
		motion(100,100,0,0);//forward
		_delay_ms(2000);
		
		motion(0,100,100,0);//left
		_delay_ms(2000);
		
		motion(100,100,0,0);//forward
		_delay_ms(2000);
		
		motion(100,0,0,100);//right
		_delay_ms(2000);
	}

}

void motion_init(void){
	TCCR0A= (1<<COM0A1)|(1<<WGM01)|(1<<WGM00)|(1<<COM0B1);
	TCCR0B= (1<<CS00);
	DDRD =0XFF;
	
	TCCR1A= (1<<COM1A1)|(1<<WGM10)|(1<<WGM12)|(1<<COM1B1);
	TCCR1B= (1<<CS10);
	DDRB =0XFF;
}

void motion(unsigned char A0,unsigned char A1,unsigned char B0,unsigned char B1){
	OCR0A=A0;
	OCR1A=A1;
	OCR0B=B0;
	OCR1B=B1;

}