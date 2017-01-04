/*
 * services.c
 *
 * Created: 5/8/2016 8:29:01 PM
 *  Author: danial
 */ 

#include <avr/io.h>
#include "services.h"
#include <math.h>
#include <string.h>


void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0B |= (1<<RXCIE0);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}




void USART_Transmit_String(char* name)
{
	unsigned int length;
	length = (unsigned int)(strlen(name));
	//length2 = (unsigned int)(strlen(dec));
	for (int i=0; i<length; i++)
	{
		USART_Transmit(name[i]);       
		if (name[i]=='\r')
		{
			USART_Transmit('\n');     //send String to BT. Terminate all sent data with a \n
		}
	}
}
void USART_Transmit_doubles(double input)
{
	
		double values = input;
		char arr[10] = { 0 };

		sprintf(arr, "%f", values);

		USART_Transmit(arr);
}


unsigned int i=0;
char rcvd_data[255];

 ISR(USART_RX_vect)
{

	char rcv_data = UDR0;
	
	if ( rcv_data == '\r')                    // incoming data (BT Device) should terminate with a carriage return
	{
		USART_Transmit_String( rcvd_data );
		//USART_Transmit(rcv_data);
		rcvd_data[255]=0;
		i=0;
	}
	else
	{
		rcvd_data[i] = rcv_data;
		i++;
	}
}

double Vref=5.00;

//*****************************************************************
//Purpose : Initialize the ADC
//*****************************************************************
void ADC_init(void)
{
	ADCSRA = 0x00; //disable adc
	ADMUX = 0x40;
	ADCSRA = 0x86;	//enable ADC and set the prescaler
}

int showADCResult()
{
	//double actualValue;
	int ADC_value0=0;
	//int ADC_value1=0;
	double Analog_voltage1 = 0;
	double Analog_voltage2 = 0;
	
	char result_buffer[5];
	char i;
	int ADC_temp, ADCH_temp;
	char string_buff[20]={0};

	/*Read Channel 0 */
	ADMUX &= 0xe0;
	ADMUX |= 0x00; 			//select ADC channel

	ADC_ENABLE;
	ADC_START_CONVERSION; 		//do a dummy readout first
	while(!(ADCSRA & 0x10)); 	// wait for conversion done, ADIF flag active
	ADCSRA|=(1<<ADIF);

	for(i=0;i<8;i++) 			// do the ADC conversion 8 times for better accuracy
	{
		ADC_START_CONVERSION;
		while(!(ADCSRA & 0x10)); 	// wait for conversion done, ADIF flag set
		ADCSRA|=(1<<ADIF);

		ADC_temp = ADCL; 			// read out ADCL register
		ADCH_temp = ADCH; 			// read out ADCH register
		ADC_temp +=(ADCH_temp << 8);
		ADC_value0 += ADC_temp; 		// accumulate result (8 samples) for later averaging
	}

	ADC_value0 = ADC_value0 >> 3; // average the 8 samples
	ADC_DISABLE;

/*Conversion method into */
//Analog_voltage1 = (3*ADC_value1)/1024;

return ADC_value0; //
}

int showADC2Result(void)
{
	//double actualValue;
	int ADC_value1=0;
	double Analog_voltage2 = 0;
	char result_buffer[5];
	char i;
	int ADC_temp, ADCH_temp;
	char string_buff[20]={0};

	/*Read Channel 1 */
	//ADMUX &= 0xe0;
	//ADMUX |= 0x00; 
  ADMUX = 0x01;			//select ADC channel

	ADC_ENABLE;
	ADC_START_CONVERSION; 		//do a dummy readout first
	while(!(ADCSRA & 0x10)); 	// wait for conversion done, ADIF flag active
	ADCSRA|=(1<<ADIF);

	for(i=0;i<8;i++) 			// do the ADC conversion 8 times for better accuracy
	{
		ADC_START_CONVERSION;
		while(!(ADCSRA & 0x10)); 	// wait for conversion done, ADIF flag set
		ADCSRA|=(1<<ADIF);

		ADC_temp = ADCL; 			// read out ADCL register
		ADCH_temp = ADCH; 			// read out ADCH register
		ADC_temp +=(ADCH_temp << 8);
		ADC_value1 += ADC_temp; 		// accumulate result (8 samples) for later averaging
	}

	ADC_value1 = ADC_value1 >> 3; // average the 8 samples
	ADC_DISABLE;

	//Conversion method into */
	//Analog_voltage2 = (3*ADC_value)/1024;

	return ADC_value1;
}




