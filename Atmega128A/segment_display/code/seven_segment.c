/*
 * seven_segment.c
 *
 * Created: 2024-10-16 오후 3:39:35
 *  Author: phm12
 */ 
#include <avr/io.h>
#include "seven_segment.h"

void VIN_seg_all_off()
{
	PORTC &= ~(0x70);
}
void SOCAMM_seg_all_off()
{
	PORTE &= ~(0x70);
}

void VIN_SEG_NUM(int number, int sel, int dot)
{
	if(sel == 0)
		PORTC |= 0x10;
	else if(sel == 1)
		PORTC |= 0x20;
	else if (sel == 2)
		PORTC |= 0x40;
	
	switch (number)
	{
		case 0:	PORTA = 0x3F; break;
		case 1:	PORTA = 0x06; break;
		case 2: PORTA = 0x5B; break;
		case 3:	PORTA = 0x4F; break;
		case 4: PORTA = 0x66; break;
		case 5: PORTA = 0x6D; break;
		case 6: PORTA = 0x7D; break;
		case 7:	PORTA = 0x07; break;
		case 8: PORTA = 0x7F; break;
		case 9: PORTA = 0x6F; break;
		default:PORTA = 0x00; break;
	}
	if (dot == 1)
		PORTA |= 0x80;
}
void SOCAMM_SEG_NUM(int number, int sel, int dot)
{
	if(sel == 0)
		PORTE |= 0x20;
	else if(sel == 1)
		PORTE |= 0x40;
	else if (sel == 2)
		PORTE |= 0x10;
		
	switch (number)
	{
		case 0: PORTF = 0x3F; break;
		case 1: PORTF = 0x06; break;
		case 2:	PORTF = 0x5B; break;
		case 3: PORTF = 0x4F; break;
		case 4: PORTF = 0x66; break;
		case 5: PORTF = 0x6D; break;
		case 6: PORTF = 0x7D; break;
		case 7: PORTF = 0x07; break;
		case 8: PORTF = 0x7F; break;
		case 9: PORTF = 0x6F; break;
		default:PORTF = 0x00; break;
	}
	
	if (dot == 1)
	PORTF |= 0x80;
}