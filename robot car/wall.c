#define F_CPU 16000000UL
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void USART_INIT(void);
unsigned char USART_RECEIVE(void);
void USART_SEND(unsigned char character);
void send_string(char *s);
char wall_val[16];

static volatile int pulse = 0;
static volatile int i = 0;
int16_t wall_count = 0; 
char wall_val[16];
  
int main(void){
	USART_INIT();
	
	DDRD=0<<PD2|1<<PD3;
	_delay_ms(50);
	
	EIMSK |= 1<<INT0 ;
	EICRA |= 1<<ISC00 ;
	
	sei(); 
	
	while(1){
		PORTD |= 1 <<PD3;
		_delay_us(15);
		PORTD &= ~(1<<PD3);
		wall_count = pulse;
		sprintf(wall_val," distance= %d",wall_count);
		send_string(wall_val);
		if (wall_count<=10){
			send_string("  wall \n");
		}else{
			send_string("  no wall \n");
		}
		_delay_ms(500);
	}
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

void USART_INIT(void){
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);               //Enable Transmit, Receive
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);            //Select UCSRC, Select transmit/receive msg bit-size - 8bit
	UBRR0L = 103;                                 //Set baudrate (For 16 MHz 9600 bps)
}

unsigned char USART_RECEIVE(void){
	while(!(UCSR0A&(1<<RXC0)));         //Wait until byte received
	return(UDR0);                      //UDR contains received msg
}

void USART_SEND(unsigned char character){
	while(!(UCSR0A&(1<<UDRE0)));        //Wait until UDR empty
	UDR0 = character;                  //Load msg byte to UDR register to be sent
	while(!(UCSR0A&(1<<TXC0)));       //Wait until msg sent
}

void send_string(char *s){        //send string
	
	for(unsigned char i=0; s[i]!=0; i++){
		USART_SEND(s[i]);
	}
}
