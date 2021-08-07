#define F_CPU 16000000UL
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void motion(unsigned char A0,unsigned char A1,unsigned char B0,unsigned char B1);
//void motion_init(void);

void usart_init(void);
unsigned char usart_receive(void);
void usart_send(unsigned char character);
void send_string(char *s);
char wall_val[16];
void wall_init(void);
void wall_detect(void);

static volatile int pulse = 0;
static volatile int i = 0;
int16_t wall_count = 0; 
char wall_val[16];

int readADC(void);
void color_detect(void);
int red_val,green_val,blue_val;
int red_val=0,green_val=0,blue_val=0;
char clr_string[10];
	
	
int main(void){
	
	usart_init();
	wall_init();
	int readADC();
	
	sei();
	
	while(1){
		_delay_ms(2000);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<2;i++){
			wall_detect();
			color_detect();
		}
		
		motion(125,110,0,0);//right
		_delay_ms(1500);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<2;i++){
			wall_detect();
			color_detect();
		}
		
		motion(0,0,110,125);//left
		_delay_ms(1500);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<2;i++){
			wall_detect();
			color_detect();
		}
		
		motion(0,0,110,125);//left
		_delay_ms(1500);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<4;i++){
			wall_detect();
			color_detect();
		}
		
		motion(0,0,110,125);//left
		_delay_ms(1500);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<2;i++){
			wall_detect();
			color_detect();
		}
		
		motion(0,0,110,125);//left
		_delay_ms(1500);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<2;i++){
			wall_detect();
			color_detect();
		}
		
		motion(125,110,0,0);//right
		_delay_ms(1500);
		
		motion(0,110,125,0);//forward
		for(int i=0;i<2;i++){
			wall_detect();
			color_detect();
		}
		
		/*motion(0,0,100,100);//left
		_delay_ms(2000);
		
		motion(0,100,100,0);//forward
		_delay_ms(2000);
		
		motion(100,0,0,100);//back
		_delay_ms(2000);
		
		motion(100,100,0,0);//right
		_delay_ms(2000);
		
		*/
	}

}


void color_detect(void){
		DDRC=DDRC|(1<<3)|(1<<4)|(1<<5);
		PORTC=PORTC|(1<<2);
		//_delay_ms(100);
		//usart_send('\n');
		
		for (int i=0; i<20; i+=1){
		
		PORTC=PORTC|(1<<3);
		_delay_ms(30);
		red_val= readADC();
		red_val = ADCL|ADCH;  //range 0-255
		itoa(red_val,clr_string,10);
		//send_string(clr_string);
		PORTC=(0<<2);

		PORTC=PORTC|(1<<4);
		_delay_ms(30);
		green_val= readADC();
		green_val = ADCL|ADCH;  //range 0-255
		itoa(green_val,clr_string,10);
		//usart_send('	');
		//send_string(clr_string);
		PORTC=(0<<3);

		PORTC=PORTC|(1<<5);
		_delay_ms(30);
		blue_val= readADC();
		blue_val = ADCL|ADCH;  //range 0-255
		itoa(blue_val,clr_string,10);
		//usart_send('	');
		//send_string(clr_string);
		PORTC=(0<<4);
		
		//usart_send('\n');
		}
		
		//send_string("finish");
		//usart_send('\n');	
		
		/////////// color detect ///////////
		/*if(red_val>190 && green_val>190 && blue_val>190){
			send_string("black");
		}
		else if(red_val<100 && green_val<100 && blue_val<100){
			send_string("white");
		}*/
		
		if(red_val<green_val && red_val<blue_val){
			send_string("color - red");
		}
		
		else if(green_val<red_val && green_val<blue_val){
			send_string("color - green");
		}

		else if(blue_val<red_val && blue_val<green_val){
			send_string("color - blue");
		}
		
		else{
			send_string("no specific color");
		}	
		
		usart_send('\n');
		send_string("///////////////////////////");
		usart_send('\n');
}


int readADC(void){
	ADMUX = ADMUX|1<<REFS0|(1<<MUX0);
	ADCSRA =ADCSRA| 1<<ADEN|1<<ADPS1|1<<ADPS0|1<<ADSC;
	
	while(ADCSRA & (1<<ADSC)){
	}
	return (ADC);
}
	


void wall_detect(void){
	PORTD |= 1 <<PD3;
	_delay_us(15);
	PORTD &= ~(1<<PD3);
	wall_count = pulse;
	sprintf(wall_val," distance= %d",wall_count);
	send_string(wall_val);
	if (wall_count<=15){
		send_string("  wall \n");
	}else{
		send_string("  no wall \n");
	}
	//_delay_ms(10);
}

void wall_init(void){
	DDRD=0<<PD2|1<<PD3;
	//_delay_ms(50);
	
	EIMSK |= 1<<INT0 ;
	EICRA |= 1<<ISC00 ;

}

ISR(INT0_vect)
{
  if(i == 0)
  {
    TCCR2B |= 1<<CS20|1<<CS21|1<<CS22;
    i = 1;
  }
  else
  {
    TCCR2B = 0;
    pulse = TCNT2;
    TCNT2 = 0;
    i = 0;
  }
}

void usart_init(void){
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);               //Enable Transmit, Receive
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);            //Select UCSRC, Select transmit/receive msg bit-size - 8bit
	UBRR0L = 103;                                 //Set baudrate (For 16 MHz 9600 bps)
}

unsigned char usart_receive(void){
	while(!(UCSR0A&(1<<RXC0)));         //Wait until byte received
	return(UDR0);                      //UDR contains received msg
}

void usart_send(unsigned char character){
	while(!(UCSR0A&(1<<UDRE0)));        //Wait until UDR empty
	UDR0 = character;                  //Load msg byte to UDR register to be sent
	while(!(UCSR0A&(1<<TXC0)));       //Wait until msg sent
}

void send_string(char *s){        //send string
	
	for(unsigned char i=0; s[i]!=0; i++){
		usart_send(s[i]);
	}
}

void motion(unsigned char A0,unsigned char A1,unsigned char B0,unsigned char B1){
	TCCR0A= (1<<COM0A1)|(1<<WGM01)|(1<<WGM00)|(1<<COM0B1);
	TCCR0B= (1<<CS00);
	DDRD =1<<PD5|1<<PD6;
	
	TCCR1A= (1<<COM1A1)|(1<<WGM10)|(1<<WGM12)|(1<<COM1B1);
	TCCR1B= (1<<CS10);
	DDRB =1<<PB1|1<<PB2;
	
	OCR0A=A0;
	OCR1A=A1;
	OCR0B=B0;
	OCR1B=B1;
}