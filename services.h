/*
 * services.h
 *
 * Created: 5/8/2016 8:28:41 PM
 *  Author: danial
 */ 


#ifndef SERVICES_H_
#define SERVICES_H_









	typedef unsigned int word;
	typedef char byte;
	
	struct 
	{
		byte one_sec:1;
		byte one_minute:1;
		
	}flags;
	
	enum
	{
		OFF = 0, ON,
	};

void sys_init(void);
//word adc(byte ch_select);
//void uart(byte*buf );

#define FOSC 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void USART_Transmit_String(char* name);
void USART_Transmit_doubles(double input);


#define ADC_ENABLE ADCSRA|=(1<<ADEN)
#define ADC_DISABLE ADCSRA &=0x7F
#define ADC_START_CONVERSION ADCSRA |= (1<<ADSC)
#define const1 0.512
#define const2 0.256
#define const3 256

//double buffer[] = {0};
void ADC_init(void);
int showADCResult(void);
int showADC2Result(void);



#endif /* SERVICES_H_ */