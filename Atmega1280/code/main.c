/*
 * test1.c
 *
 * Created: 2024-03-18 오전 8:46:07
 * Author : phm12
 */ 
#define F_CPU 8000000UL

#define FW_MAJOR_VERSION 		0x1
#define FW_MINOR_VERSION 		0x00

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>


void initGPIO();
void initTimer();
void initI2CSlave();

uint16_t uint8_to_uint16(uint8_t lsb, uint8_t msb);
void seg_all_off();
void seg1_number(int number,int pin,int dot);
void seg2_number(int number,int pin,int dot);
void seg3_number(int number,int pin,int dot);
void seg4_number(int number,int pin,int dot);
void seg5_number(int number,int pin,int dot);
void seg6_number(int number,int pin,int dot);

volatile uint16_t timer_1mcnt = 0;
volatile uint16_t timer_4mcnt = 0; 
volatile uint16_t dataCount = 0;
uint8_t i2c_buf[16] = {FW_MAJOR_VERSION, FW_MINOR_VERSION, 0x0,};
int reg_index =0;
uint16_t segnumber[6] ={0,};
int h_place = 0;
int t_place = 0;
int o_place = 0;


int main(void)
{
	initGPIO();
	initTimer();
	initI2CSlave();
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}


void initTimer()
{
	//TCCR0B |= (1 << CS01);  // PRESCALER 8
	//TCNT0 = 246; //(256 - 236 + 1 = 7, 8MHz)
	//TIMSK0 |= (1 << TOIE0);// 오버플로우 인터럽트를 활성화
	
	 //Timer/Counter 0 설정 
	TCCR0B |= (1 << CS01) | (1 << CS00);  // PRESCALER 256
	TCNT0 = 6; //(256 - 250 + 1 = 7, 8MHz)
	TIMSK0 |= (1 << TOIE0);// 오버플로우 인터럽트를 활성화
}

void initI2CSlave()
{
	TWSR = 0x00;
	TWBR = 0x20;
	// I2C 주소 설정
	TWAR = (0x60<<1);
	//I2C 슬레이브 주소를 0x60으로 설정하는것

	
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) ;
	//TWCr은 슬레이브 rec,tran 모드일경우 TWEA = 1, TWEN = 1, 밑에 인터럽트를 발생할것이기 때문에 인터럽트를 클리어하기위해 TWINT를 1로
	// TWI 슬레이브 수신 완료 인터럽트 활성화
	TWCR |= (1 << TWIE);
	//TWIE = 1 로 사용하여 TWI 인터럽트 활성화
	
}
void initGPIO()
{
	DDRA = 0xFF; //DUT1_SEG_A,B,C,D,E,F,G,DOT
	DDRB = 0xFC; //DUT1_SEG0,1,2 DUT2_SEG0,1,2
	DDRC = 0xFF; //DUT3_SEG_A,B,C,D,E,F,G,DOT
	DDRD = 0x70; //CPU0_MGMT_SEG0,1,2
	DDRE = 0xE0; //DUT4_SEG0,1,2 
	DDRF = 0x07; //CPU1_MGMT_SEG0,1,2 
	DDRG = 0x07; // DUT3_SEG0,1,2
	DDRH = 0xFF; //DUT4_SEG_A,B,C,D,E,F,G,DOT
	DDRJ = 0xFF; //CPU0_MGMT_SEG_A,B,C,D,E,F,G,DOT
	DDRK = 0xFF; //CPU1_MGMT_SEG_A,B,C,D,E,F,G,DOT
	DDRL = 0xFF; //DUT2_SEG_A,B,C,D,E,F,G,DOT
	
	//DDRA = 0xFF
	
	//DDRA = 0xF8;
	
	PORTA = 0xFF;
	PORTB = 0xFC;
	PORTC = 0xFF;
	PORTD = 0x70;
	PORTE = 0xE0;
	PORTF = 0x07;
	PORTG = 0x07;
	PORTH = 0xFF;
	PORTJ = 0xFF;
	PORTK = 0xFF;
	PORTL = 0xFF;
	
}
//------------------------------------------------------------------------------------------------
//발생할수 있는 TW_STATUS 의 case문에 작성
//	TW -> TWI = i2C
//	SR -> Slave Receiver
//	ST -> Slave Transmiiter
//	TW_SR_SLA_ACK Atmega가 Slave로 slave address 를 받고 write 명령을 받고 ack를 받았을경우 0x60이 뜬다.
//	TW_SR_DATA_ACK Atmega에 Slave로 설정된 address로 data가 들어온 후 ack를 받았을경우 0x80이 뜬다.
//	TW_ST_SLA_ACK Atemga가 Slave로 slave address를 받고 read 명령을 받고 ack를 받았을경우 0xA8이 뜬다.
//	TW_ST_DATA_ACK Atmega가 Slave로 설정된 address로 data를 보내주고 ack를 받았을경우 0xB8이 뜬다.
//	TW stop or nack(not ack)일 경우 오류가 났거나 데이터를 끝냈을 경우 처리
// -------------------------------------------------------------------------------------------------
ISR(TWI_vect) {
	
	switch (TW_STATUS) {
		//case TW_SR_SLA_ACK: // 0x60
							//TWCR |= (1 << TWEA);
							//	break;
		case TW_SR_DATA_ACK: //0x80
						if (dataCount++ == 0)
						{
							reg_index = TWDR;
						}
						else
						{
							if (reg_index == 0 || reg_index == 1)
							{
								break;
							}
							i2c_buf[reg_index]=TWDR;
						}
						switch (reg_index)
						{
							case 3 :	//12v
									segnumber[0] = uint8_to_uint16(i2c_buf[2],i2c_buf[3]);//seg1
									break;
							case 5 :	//12v
									segnumber[1] = uint8_to_uint16(i2c_buf[4],i2c_buf[5]);//seg2
									break;
							case 7 :	//12v
									segnumber[2] = uint8_to_uint16(i2c_buf[6],i2c_buf[7]);//seg5
									break;
							case 9 :	//12v
									segnumber[3] = uint8_to_uint16(i2c_buf[8],i2c_buf[9]);//seg3
									break;
							case 11 :	//3.3v
									segnumber[4] = uint8_to_uint16(i2c_buf[10],i2c_buf[11]);//seg4
									break;
							case 13 :	//3.3v
									segnumber[5] = uint8_to_uint16(i2c_buf[12],i2c_buf[13]);//seg6
									break;
						}
						if(reg_index >= 14)
						reg_index =0;
						break;
		case TW_ST_SLA_ACK: // A8
		case TW_ST_DATA_ACK: // B8
							if (reg_index == 0 || reg_index == 1)
							{
								TWDR = i2c_buf[reg_index];
							}
							break;
		case TW_ST_DATA_NACK: //C0
		case TW_SR_STOP: // A0
							dataCount = 0;
							break;
		default:
							TWCR |= (1 << TWINT) | (1 << TWEA);
							break;
	}
	TWCR |= (1 << TWINT);
	//TWCR = (1 << TWIE) | (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}
ISR(TIMER0_OVF_vect) //2ms
{
	timer_1mcnt++;
	if(timer_1mcnt == 1) //4ms
	{
		switch (timer_4mcnt)  //12ms 마다 반복
		{	
			case 0://100
					seg_all_off();
					h_place = (int)(segnumber[0]/1000);
					seg1_number(h_place,timer_4mcnt,0);
					h_place = (int)(segnumber[1]/1000);
					seg2_number(h_place,timer_4mcnt,0);
					h_place = (int)(segnumber[2]/1000);
					seg3_number(h_place,timer_4mcnt,0);
					h_place = (int)(segnumber[3]/1000);
					seg4_number(h_place,timer_4mcnt,0);
					h_place = (int)(segnumber[4]/100);
					//seg5_number(h_place,timer_4mcnt,1);
					seg5_number(h_place,timer_4mcnt,0);
					h_place = (int)(segnumber[5]/100);
					//seg6_number(h_place,timer_4mcnt,1);
					seg6_number(h_place,timer_4mcnt,0);
					break;
			case 1://10		
					seg_all_off();
					t_place = (int)(segnumber[0]%1000)/100;
					seg1_number(t_place,timer_4mcnt,1);
					t_place = (int)(segnumber[1]%1000)/100;
					seg2_number(t_place,timer_4mcnt,1);
					t_place = (int)(segnumber[2]%1000)/100;
					seg3_number(t_place,timer_4mcnt,1);
					t_place = (int)(segnumber[3]%1000)/100;
					seg4_number(t_place,timer_4mcnt,1);
					t_place = (int)(segnumber[4]%100)/10;
					seg5_number(t_place,timer_4mcnt,0);
					t_place = (int)(segnumber[5]%100)/10;
					seg6_number(t_place,timer_4mcnt,0);
					break;
			case 2://1  
					 seg_all_off();
					 o_place = (int)(segnumber[0]%100)/10;
					 seg1_number(o_place,timer_4mcnt,0);
					 o_place = (int)(segnumber[1]%100)/10;
					 seg2_number(o_place,timer_4mcnt,0);
					 o_place = (int)(segnumber[2]%100)/10;
					 seg3_number(o_place,timer_4mcnt,0);
					 o_place = (int)(segnumber[3]%100)/10;
					 seg4_number(o_place,timer_4mcnt,0);
					 o_place = (int)(segnumber[4]%10);
					 //seg5_number(o_place,timer_4mcnt,0);
					 seg5_number(o_place,timer_4mcnt,1);
					 o_place = (int)(segnumber[5]%10);
					 //seg6_number(o_place,timer_4mcnt,1);
					 seg6_number(o_place,timer_4mcnt,0);
					 break;
		}
		//seg_all_off();
		timer_1mcnt = 0;
		timer_4mcnt++;
		if(timer_4mcnt>2)
		timer_4mcnt =0;
	}
	
}
uint16_t uint8_to_uint16(uint8_t lsb, uint8_t msb)
{
	uint16_t buf;
	buf = (msb<<8)|lsb;
	return buf;
}
void seg_all_off()
{
	/*
	PORTB |= 0x1C; // seg 1
	PORTB |= 0xE0; // seg 2
	PORTG |= 0x07; // seg 3
	PORTE |= 0xE0; // seg 4
	PORTD |= 0x70; // seg 5
	PORTF |= 0x07; // seg 6
	
	*/
	PORTB &= ~(0x1C); // seg 1
	PORTB &= ~(0xE0); // seg 2
	PORTG &= ~(0x07); // seg 3
	PORTE &= ~(0xE0); // seg 4
	PORTD &= ~(0x70); // seg 5
	PORTF &= ~(0x07); // seg 6
	
}
void seg1_number(int number,int pin,int dot)//DUT1_SEG
{

		
	switch(pin)
	{
		case 0:
				//PORTB &=0xFB;
				//PORTB |= 0x04;
				PORTB |= 0x10;
				break;
		case 1:
				//PORTB &=0xF7;
				PORTB |= 0x08;
				break;
		case 2:
				//PORTB &=0xEF;
				//PORTB |= 0x10;
				PORTB |= 0x04;
				break;
	}
	switch (number)
	{
			case 0: 
					PORTA = 0x3F;
					break;
			case 1: 
					PORTA = 0x06;
					break;
			case 2:
					PORTA = 0x5B;
					break;
			case 3:
					PORTA = 0x4F;
					break;
			case 4:
					PORTA = 0x66;
					break;
			case 5:
					PORTA = 0x6D;
					break;
			case 6:
					PORTA = 0x7D;
					break;
			case 7:
					PORTA = 0x07;	
					break;
			case 8:
					PORTA = 0x7F;
					break;
			case 9:
					PORTA = 0x6F;
					break;
			
				
	}
	if(dot == 1)
	PORTA |= 0x80;
	else
	PORTA &= 0x7F;
}
void seg2_number(int number,int pin,int dot) //SUT2_SEG
{
	
	
	switch(pin)
	{
		case 0:
				//PORTB &=0xDF;
				//PORTB |= 0x20;
				PORTB |= 0x80;
				break;
		case 1:
				//PORTB &=0xBF;
				PORTB |= 0x40;
				break;
		case 2:
				//PORTB &=0x7F;
				//PORTB |= 0x80;
				PORTB |= 0x20;
				break;
	}
	switch (number)
	{
		case 0:
				PORTL = 0x3F;
				break;
		case 1:
				PORTL = 0x06;
				break;
		case 2:
				PORTL = 0x5B;
				break;
		case 3:
				PORTL = 0x4F;
				break;
		case 4:
				PORTL = 0x66;
				break;
		case 5:
				PORTL = 0x6D;
				break;
		case 6:
				PORTL = 0x7D;
				break;
		case 7:
				PORTL = 0x07;
				break;
		case 8:
				PORTL = 0x7F;
				break;
		case 9:
				PORTL = 0x6F;
				break;
		
	}
	if(dot == 1)
	PORTL |= 0x80;
	else
	PORTL &= 0x7F;
}
void seg3_number(int number,int pin,int dot)//DUT3_SEG
{
	
	
	switch (pin)
	{
		case 0:
				//PORTG &= 0xFE;
				//PORTG |= 0x01;
				PORTG |= 0x04;
				break;
		case 1:
				//PORTG &= 0xFD;
				PORTG |= 0x02;
				break;
		case 2 :
				//PORTG &= 0xFB;
				//PORTG |= 0x04;
				PORTG |= 0x01;
				break;
	}
	switch (number)
	{
		case 0:
				PORTC = 0x3F;
				break;
		case 1:
				PORTC = 0x06;
				break;
		case 2:
				PORTC = 0x5B;
				break;
		case 3:
				PORTC = 0x4F;
				break;
		case 4:
				PORTC = 0x66;
				break;
		case 5:
				PORTC = 0x6D;
				break;
		case 6:
				PORTC = 0x7D;
				break;
		case 7:
				PORTC = 0x07;
				break;
		case 8:
				PORTC = 0x7F;
				break;
		case 9:
				PORTC = 0x6F;
				break;
		
	}
	if(dot == 1)
	PORTC |= 0x80;
	else
	PORTC &= 0x7F;
}
void seg4_number(int number,int pin,int dot)//DUT4_SEG
{
	
	
	switch (pin)
	{
		case 0:
				//PORTE &=0xDF;
				//PORTE |= 0x20;
				PORTE |= 0x80;
				break;
		case 1:
				//PORTE &=0xBF;
				PORTE |= 0x40;
				break;
		case 2:
				//PORTE &=0x7F;
				//PORTE |= 0x80;
				PORTE |= 0x20;
				break;
	}
	switch (number)
	{
		case 0:
				PORTH = 0x3F;
				break;
		case 1:
				PORTH = 0x06;
				break;
		case 2:
				PORTH = 0x5B;
				break;
		case 3:
				PORTH = 0x4F;
				break;
		case 4:
				PORTH = 0x66;
				break;
		case 5:
				PORTH = 0x6D;
				break;
		case 6:
				PORTH = 0x7D;
				break;
		case 7:
				PORTH = 0x07;
				break;
		case 8:
				PORTH = 0x7F;
				break;
		case 9:
				PORTH = 0x6F;
				break;
		
	}
	if(dot == 1)
	PORTH |= 0x80;
	else
	PORTH &= 0x7F;
}
void seg5_number(int number,int pin,int dot)//CPU0_MGMT_SEG
{
	
	switch (pin)
	{
		case 0:
				//PORTD &= 0xEF;
				//PORTD |= 0x10;
				PORTD |= 0x40;
				break;
		case 1:
				//PORTD &= 0xDF;
				PORTD |= 0x20;
				break;
		case 2:
				//PORTD &= 0xBF;
				//PORTD |= 0x40;
				PORTD |= 0x10;
				break;
	}
	switch (number)
	{
		case 0:
				PORTJ = 0x3F;
				break;
		case 1:
				PORTJ = 0x06;
				break;
		case 2:
				PORTJ = 0x5B;
				break;
		case 3:
				PORTJ = 0x4F;
				break;
		case 4:
				PORTJ = 0x66;
				break;
		case 5:
				PORTJ = 0x6D;
				break;
		case 6:
				PORTJ = 0x7D;
				break;
		case 7:
				PORTJ = 0x07;
				break;
		case 8:
				PORTJ = 0x7F;
				break;
		case 9:
				PORTJ = 0x6F;
				break;
		
	}
	if(dot == 1)
	PORTJ |= 0x80;
	else
	PORTJ &= 0x7F;
}
void seg6_number(int number,int pin,int dot)//CPU1_MGMT_SEG
{
	
	switch (pin)
	{
		case 0:
				//PORTF &= 0xFE;
				//PORTF |= 0x01;
				PORTF |= 0x04;
				break;
		case 1 :
				//PORTF &= 0xFD;
				PORTF |= 0x02;
				break;
		case 2 :	
				//PORTF &= 0xFB;
				//PORTF |= 0x04;
				PORTF |= 0x01;
				break;
	}
	switch (number)
	{
		case 0:
				PORTK = 0x3F;
				break;
		case 1:
				PORTK = 0x06;
				break;
		case 2:
				PORTK = 0x5B;
				break;
		case 3:
				PORTK = 0x4F;
				break;
		case 4:
				PORTK = 0x66;
				break;
		case 5:
				PORTK = 0x6D;
				break;
		case 6:
				PORTK = 0x7D;
				break;
		case 7:
				PORTK = 0x07;
				break;
		case 8:
				PORTK = 0x7F;
				break;
		case 9:
				PORTK = 0x6F;
				break;		
	}
	if(dot == 1)
	PORTK |= 0x80;
	else
	PORTK &= 0x7F;
}
