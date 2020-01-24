/*
 * lab09.c
 *
 * Created: 11/1/2020 09:55:31
 *  Author: Student
 */

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
void adc(void);
void read_adc(void);

int main(void)
{
	DDRA = 0b00000000;
	DDRC = 0b00000000;
	DDRE = 0xFF;
	DDRB = 0b00101111;																  //PB0 = clock pb1 = ocr pb2 = ale pb3 = start pb4 = eoc
																					  //	DDRB = 0x10011111;
	DDRD = 0xFF;																	  //led display
	TCCR2 |= (1 << WGM21) | (1 << WGM20) | (1 << COM21) | (1 << COM20) | (1 << CS21); //Oscillator pinb
	TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS00);								  //clock pin
	OCR0 = 2;
	PORTB = 0b00010010;
	while (1)
	{
		adc();
	}
}
void adc(void)
{
	PORTE = 0x00;
	read_adc();
}
void read_adc(void)
{
	char num = 0;
	PORTB = PORTB | 0b00001110; //pb1 pb2 pb3
	_delay_ms(100);
	PORTB = PORTB & 0b10010011;//comparing port B with the read/write state of ADC // remove any incoming garbage value
	while ((PINB & 0b00010010) == 0b00010010)
		;
	while ((PINB & 0b00010010) == 0b00000010)
		;
	PORTB = PORTB | (0b00000010); //output osc pin
	num = PINC; 
	_delay_ms(200);
	PORTB = PORTB & (0b11011111);//reset port B
	PORTD = num;
	//pwm logic starting with minimum value
	if (num == 0b00000000)
	{
		OCR2 = 255;
	}
	else if ((num > 0b00000000) & (num <= 0b00110011)) // 0-51
	{
		OCR2 = 200;
	}
	else if ((num > 0b00110011) & (num <= 0b01100110)) //52-102
	{
		OCR2 = 160;
	}
	else if ((num > 0b01100110) & (num <= 0b10011001)) //102-153
	{
		OCR2 = 127;
	}
	else if ((num > 0b10011001) & (num <= 0b11001100)) //153-204
	{
		OCR2 = 80;
	}
	else if ((num > 0b11001100) & (num < 0b11111111)) //204-255
	{
		OCR2 = 30;
	}
	else if (num == 0b11111111)
	{
		OCR2 = 0;
	}
}