/*
 * Lan_v08.c
 *
 * Created: 2024-10-16 오후 3:26:19
 * Author : phm12
 */ 
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "seven_segment.h"

void initGPIO();
void initUART();
void initTIMER();
void cal_vin();
void cal_soc();
uint16_t uint8_to_uint16(uint8_t lsb, uint8_t msb);

#define LANSING_MODE	0x01
#define LANSING_S_MODE	0x02

#define START_BIT		0xAA
#define END_BIT			0xBB


volatile uint16_t first_seg = 0;//UART 첫번째 데이터(7-seg)
volatile uint16_t second_seg = 0;//UART 두번째 데이터(7-seg)
volatile uint8_t receive_data[10];//UART 데이터 저장
volatile int data_count = 0;//UART 받은 횟수
bool S_MODE_STRAP = true;//gpio mode read
int timer_2mcnt = 0;//2m timer count
int timer_4mcnt = 0;//4m timer count
int vin_h_place = 0;//첫번째 seg 100의 자리 (실제 1의자리)
int vin_t_place = 0;//첫번째 seg 10의 자리 (실제 0.1의 자리)
int vin_o_place = 0;//첫번째 seg 1의 자리 (실제 0.01의 자리)
int soc_h_place = 0;//두번째 seg 100의 자리 (실제 1의자리)
int soc_t_place = 0;//두번째 seg 10의 자리 (실제 0.1의 자리)
int soc_o_place = 0;//두번째 seg 1의 자리 (실제 0.01의 자리)
int data_Length = 20;//UART 데이터 길이(defalut : Lansing-s)


int main(void)
{
	initGPIO();
	initTIMER();
	initUART();
	
	sei();
	PORTC &= ~(0x03);
	//int ret = 0;
    /* Replace with your application code */
    while (1) 
    {
		//read_EC_GPIO();
    }
}

void initGPIO()
{
	DDRA = 0xFF; //LPCAMM_VIN_SEG_A,B,C,D,E,F,G,DOT
	DDRB = 0x00; 
	DDRC = 0x73; //MEM_PASS,MEM_FAIL,LPCOMM_VIN_SEG_SEL1,2,3
	DDRD = 0x00; 
	DDRE = 0x70; //SOCAMM_VDD2L_SEG_SEL1,2,3
	DDRF = 0xFF; //SOCAMM_VDD2L_SEG_A,B,C,D,E,F,G,DOT
	DDRG = 0x00; // EC_GPIO_0,1,2
	
	PORTA = 0xFF;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x70;
	PORTF = 0xFF;
	PORTG = 0x08;
}


void initUART() // 19200bps 
{
	//uint16_t ubrr = 8; //(F_CPU/(16*BaudRate)) -1 = (16M/(16*115200)) -1 = 7.68(반올림 해서 8)
	uint16_t ubrr = 51; //(F_CPU/(16*BaudRate)) -1 = (16M/(16*19200)) -1 = 51.08(반올림 해서 51)
	UBRR1H = (unsigned char)(ubrr>>8); 
	UBRR1L = (unsigned char)ubrr;
	//uint8_t tmp = UCSR1A; //UCSR1A의 레지스터를 읽었을경우 동작 가능
	//UCSR1A = tmp;
	(void)UCSR1A; //UCSR1A를 읽으므로써 sync 안정화 
	UCSR1B = (1<<RXEN1) | (1<<RXCIE1);  //RXEN1 UART1 RX 활성화, RXCIE1 RX inturrupt 활성화
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); //Data 크기를 8bit로 설정
}

void initTIMER()
{
	 //Timer/Counter 2 설정
	 TCCR2 |= (1 << CS01) | (1 << CS00);  // (ATMEGA1280) PRESCALER 64
	 //TCCR2 |= (1<<CS02) | (1<<CS00); // (ATMEGA128A) PRESCALER 64
	 TCNT2 = 6; // TCNT 는 초기값(카운트를 시작하는 초기값)
				// (256-6 =250(카운트 횟수), 8MHz)
				// 8MHz/Prescaler(64) = 125,000Hz
				// 1Tick 2ms  X 카운트 횟수  = 2ms 마다 반복(인터럽트)
	 TIMSK |= (1 << TOIE2);// 오버플로우 인터럽트를 활성화
}

ISR(USART1_RX_vect)
{
	uint8_t data = UDR1;
	if (data_count == 0)
	{
		if(data != START_BIT)
		return;
	}
	
	receive_data[data_count++] = data;
	if (data_count == data_Length)
	{
		if (receive_data[data_Length -1] == END_BIT)
		{
			uint8_t checksum = 0;
			for (uint8_t i = 1; i < data_Length -2; i++)
			{
				checksum ^= receive_data[i];
			}

			if (checksum == receive_data[data_Length -2])
			{
				
				if (receive_data[1] == LANSING_MODE)
				{
					S_MODE_STRAP = false;
					first_seg = uint8_to_uint16(receive_data[3], receive_data[4]);
				}
				else
				{
					S_MODE_STRAP = true;
					first_seg = uint8_to_uint16(receive_data[3], receive_data[4]);
					second_seg = uint8_to_uint16(receive_data[5], receive_data[6]);
				}
				if(receive_data[2] <3)
				{
					PORTC &= 0xFC;
					PORTC |= receive_data[2];
				}
				else 
				{
					PORTC &= 0xFC;
				}
			}
			
		}
		data_count = 0;
	}
}


ISR(TIMER2_OVF_vect)//2ms
{
	timer_2mcnt ++;
	if(timer_2mcnt == 2) //4ms
	{
		VIN_seg_all_off();
		SOCAMM_seg_all_off();
		if(S_MODE_STRAP)// Lansing-s
		{
			cal_vin(first_seg);
			cal_soc(second_seg);
			//VIN_seg_all_off();
			//SOCAMM_seg_all_off();
			switch(timer_4mcnt) //12ms
			{
				case 0:
					VIN_SEG_NUM(vin_h_place,timer_4mcnt,1);
					SOCAMM_SEG_NUM(soc_h_place,timer_4mcnt,1);
					break;
				case 1 :
					VIN_SEG_NUM(vin_t_place,timer_4mcnt,0);
					SOCAMM_SEG_NUM(soc_h_place,timer_4mcnt,0);
					break;
				case 2 :
					VIN_SEG_NUM(vin_o_place,timer_4mcnt,0);
					SOCAMM_SEG_NUM(soc_h_place,timer_4mcnt,0);
					break;
			}
		}
		else
		{
			cal_vin(first_seg);
			//VIN_seg_all_off();
			//VIN_SEG_NUM(1,1,0);
			switch(timer_4mcnt) //12ms
			{
				case 0:
					VIN_SEG_NUM(vin_h_place,timer_4mcnt,1);
					break;
				case 1 :
					VIN_SEG_NUM(vin_t_place,timer_4mcnt,0);
					break;
				case 2 :
					VIN_SEG_NUM(vin_o_place,timer_4mcnt,0);
					break;
			}
		}
		
		timer_4mcnt++;
		timer_2mcnt=0;
		
		
		if(timer_4mcnt >2)
		{
			timer_4mcnt =0;
		}
		
	}
}

void cal_vin(uint16_t vin_number)
{
	vin_h_place = vin_number/1000;
	vin_t_place = vin_number%1000/100;
	vin_o_place = vin_number%100/10;
}
void cal_soc(uint16_t soc_number)
{
	soc_h_place = soc_number/1000;
	soc_t_place = soc_number%1000/100;
	soc_o_place = soc_number%100/10;
}

uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb)
{
	//uint16_t buf;
	//buf = (msb<<8)|lsb;
	//return buf;
	return ((msb << 8) | lsb);
}