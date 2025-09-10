#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char units_sec_num = 0;
unsigned char tens_sec_num = 0;
unsigned char units_min_num = 0;
unsigned char tens_min_num = 0;
unsigned char units_hours_num = 0;
unsigned char tens_hours_num = 0;
unsigned char flag = 0;
unsigned char FLAG_HOUR_INC = 0;
unsigned char FLAG_MIN_INC = 0;
unsigned char FLAG_SEC_INC = 0;
unsigned char FLAG_HOUR_DEC = 0;
unsigned char FLAG_MIN_DEC = 0;
unsigned char FLAG_SEC_DEC = 0;
unsigned char toggle = 1;
unsigned char toggle_flag = 0;

void display(void)
{
	PORTA = 0x20;
	PORTC = (PORTC & 0xF0) | (units_sec_num & 0x0F);
	_delay_ms(2);
	PORTA = 0x10;
	PORTC = (PORTC & 0xF0) | (tens_sec_num & 0x0F);
	_delay_ms(2);
	PORTA = 0x08;
	PORTC = (PORTC & 0xF0) | (units_min_num & 0x0F);
	_delay_ms(2);
	PORTA = 0x04;
	PORTC = (PORTC & 0xF0) | (tens_min_num & 0x0F);
	_delay_ms(2);
	PORTA = 0x02;
	PORTC = (PORTC & 0xF0) | (units_hours_num & 0x0F);
	_delay_ms(2);
	PORTA = 0x01;
	PORTC = (PORTC & 0xF0) | (tens_hours_num & 0x0F);
	_delay_ms(2);
}

void pause(void)
{
	if (PIND & (1 << PD3))
	{
		if (flag == 1)
		{
			TCCR1B &= ~(1 << CS11);
			TCCR1B &= ~(1 << CS12);
			TCCR1B &= ~(1 << CS10);
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	if (toggle == 1)
	{
		PORTD |= (1 << PD4);
		PORTD &= ~(1 << PD5);
		PORTD &= ~(1 << PD0);

		if (units_sec_num == 9) {
			units_sec_num = 0;
			if (tens_sec_num == 5)
			{
				tens_sec_num = 0;
				if (units_min_num == 9)
				{
					units_min_num = 0;
					if (tens_min_num == 5)
					{
						tens_min_num = 0;
						if (units_hours_num == 9)
						{
							units_hours_num = 0;
							if (tens_hours_num == 5)
							{
								tens_hours_num = 0;
							}
							else
							{
								tens_hours_num++;
							}
						}
						else
						{
							units_hours_num++;
						}
					}
					else
					{
						tens_min_num++;
					}
				}
				else
				{
					units_min_num++;
				}
			}
			else
			{
				tens_sec_num++;
			}
		}
		else
		{
			units_sec_num++;
		}
	}
	else if (toggle == 0)
	{
		PORTD |= (1 << PD5);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);
		if (units_sec_num == 0)
		{
			units_sec_num = 9;
			if (tens_sec_num == 0)
			{
				tens_sec_num = 5;
				if (units_min_num == 0)
				{
					units_min_num = 9;
					if (tens_min_num == 0)
					{
						tens_min_num = 5;
						if (units_hours_num == 0)
						{
							units_hours_num = 9;
							if (tens_hours_num == 0)
							{
								tens_hours_num = 0;
								units_hours_num = 0;
								units_min_num = 0;
								tens_min_num = 0;
								tens_sec_num = 0;
								units_sec_num = 0;
								PORTD |= (1 << PD0);
							}
							else
							{
								tens_hours_num--;
							}
						}
						else
						{
							units_hours_num--;
						}
					}
					else
					{
						tens_min_num--;
					}
				}
				else
				{
					units_min_num--;
				}
			}
			else
			{
				tens_sec_num--;
			}
		}
		else
		{
			units_sec_num--;
		}
	}
}

void toggle_button(void)
{
	if (!(PINB & (1 << PB7)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB7)))
		{
			if (toggle_flag == 0)
			{
				toggle = !toggle;
				toggle_flag = 1;
			}
		}
	}
	else
	{
		toggle_flag = 0;
	}
}

ISR(INT0_vect)
{
	TCNT1 = 0;
	units_sec_num = 0;
	tens_sec_num = 0;
	units_min_num = 0;
	tens_min_num = 0;
	units_hours_num = 0;
	tens_hours_num = 0;
	PORTD &= ~(1 << PD0);
}

ISR(INT1_vect)
{
	flag = 1;
}

ISR(INT2_vect)
{
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void INT_0(void)
{
	GICR |= (1 << INT0);
	MCUCR |= (1 << ISC01);
}

void INT_1(void)
{
	GICR |= (1 << INT1);
	MCUCR |= (1 << ISC11) | (1 << ISC10);
}

void INT_2(void)
{
	GICR |= (1 << INT2);
	MCUCSR &= ~(1 << ISC2);
}

void TIMER_1(void)
{
	TCNT1 = 0;
	OCR1A = 15624;
	TIMSK = (1 << OCIE1A);
	TCCR1A = (1 << FOC1A) | (1 << FOC1B);
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}

void TIME_INCREMENT(void)
{
	if (!(PINB & (1 << PB1)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB1)))
		{
			if (FLAG_HOUR_INC == 0)
			{
				if (units_hours_num == 9)
				{
					units_hours_num = 0;
					if (tens_hours_num == 5)
					{
						tens_hours_num = 5;
						units_hours_num = 9;
					}
					else
					{
						tens_hours_num++;
					}
				}
				else
				{
					units_hours_num++;
				}
				FLAG_HOUR_INC = 1;
			}
		}
	}
	else
	{
		FLAG_HOUR_INC = 0;
	}

	if (!(PINB & (1 << PB4)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB4)))
		{
			if (FLAG_MIN_INC == 0)
			{
				if (units_min_num == 9)
				{
					units_min_num = 0;
					if (tens_min_num == 5)
					{
						tens_min_num = 5;
						units_min_num = 9;
					}
					else
					{
						tens_min_num++;
					}
				}
				else
				{
					units_min_num++;
				}
				FLAG_MIN_INC = 1;
			}
		}
	}
	else
	{
		FLAG_MIN_INC = 0;
	}

	if (!(PINB & (1 << PB6)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB6)))
		{
			if (FLAG_SEC_INC == 0)
			{
				if (units_sec_num == 9)
				{
					units_sec_num = 0;
					if (tens_sec_num == 5)
					{
						tens_sec_num = 5;
						units_sec_num = 9;
					}
					else
					{
						tens_sec_num++;
					}
				}
				else
				{
					units_sec_num++;
				}
				FLAG_SEC_INC = 1;
			}
		}
	}
	else
	{
		FLAG_SEC_INC = 0;
	}
}

void TIME_DECCREMENT(void)
{
	if (!(PINB & (1 << PB0)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB0)))
		{
			if (FLAG_HOUR_DEC == 0)
			{
				if (units_hours_num == 0)
				{
					units_hours_num = 9;
					if (tens_hours_num == 0)
					{
						tens_hours_num = 0;
						units_hours_num = 0;
					}
					else
					{
						tens_hours_num--;
					}
				}
				else
				{
					units_hours_num--;
				}
				FLAG_HOUR_DEC = 1;
			}
		}
	}
	else
	{
		FLAG_HOUR_DEC = 0;
	}

	if (!(PINB & (1 << PB3)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB3)))
		{
			if (FLAG_MIN_DEC == 0)
			{
				if (units_min_num == 0)
				{
					units_min_num = 9;
					if (tens_min_num == 0)
					{
						tens_min_num = 0;
						units_min_num = 0;
					}
					else
					{
						tens_min_num--;
					}
				}
				else
				{
					units_min_num--;
				}
				FLAG_MIN_DEC = 1;
			}
		}
	}
	else
	{
		FLAG_MIN_DEC = 0;
	}

	if (!(PINB & (1 << PB5)))
	{
		_delay_ms(30);
		if (!(PINB & (1 << PB5)))
		{
			if (FLAG_SEC_DEC == 0)
			{
				if (units_sec_num == 0)
				{
					units_sec_num = 9;
					if (tens_sec_num == 0)
					{
						tens_sec_num = 0;
						units_sec_num = 0;
					}
					else
					{
						tens_sec_num--;
					}
				}
				else
				{
					units_sec_num--;
				}
				FLAG_SEC_DEC = 1;
			}
		}
	}
	else
	{
		FLAG_SEC_DEC = 0;
	}
}

int main(void)
{
	SREG |= (1 << 7);

	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);

	DDRD &= ~(1 << PD3);

	DDRB &= ~(1 << PB2);
	PORTB |= (1 << PB2);

	DDRB &= ~(1 << PB7);
	PORTB |= (1 << PB7);

	DDRB &= ~(1 << PB1);
	PORTB |= (1 << PB1);

	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0);

	DDRB &= ~(1 << PB4);
	PORTB |= (1 << PB4);

	DDRB &= ~(1 << PB3);
	PORTB |= (1 << PB3);

	DDRB &= ~(1 << PB6);
	PORTB |= (1 << PB6);

	DDRB &= ~(1 << PB5);
	PORTB |= (1 << PB5);

	DDRD |= (1 << PD0);
	PORTD &= ~(1 << PD0);

	DDRD |= (1 << PD4);
	PORTD &= ~(1 << PD4);

	DDRD |= (1 << PD5);
	PORTD &= ~(1 << PD5);

	DDRC |= 0x0F;
	PORTC &= 0xF0;

	DDRA &= 0xC0;

	TIMER_1();
	INT_1();
	INT_0();
	INT_2();

	while (1)
	{
		display();
		pause();
		TIME_INCREMENT();
		TIME_DECCREMENT();
		toggle_button();
	}
}
