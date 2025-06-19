/*
 * ADL_USB-t1.c
 *
 * Created: 2023-11-30 오후 2:20:01
 * Author : phm1225
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

void initGPIO();
void initTimer();
void initI2CSlave();
void ask_version(int data);
void reset_USB();
void POWER_L_ONOFF(uint16_t data);
void POWER_H_ONOFF(uint16_t data);
void Power_ONOFF_ALL(uint16_t data);
void Modechange_L(uint16_t data);
void Modechange_H(uint16_t data);
void Modechange_ALL(uint16_t data);

int send_data =12;
uint8_t rxbuffer[5] = {0, };
volatile uint8_t receivedData[3] = {0, };
volatile uint8_t dataCount = 0;
unsigned char i2c_buf[16] = {0x7C,};
unsigned char reg_index =0;
unsigned char send_buf[4] = {0xFF,0xFF,0xFF,0xFF};
volatile uint8_t portAvalue =0;
volatile uint8_t portCvalue =0;
volatile uint8_t portValue1 = 0x00;
volatile uint8_t portValue2 = 0x00;
unsigned char combined_value1 = 0;
unsigned char combined_value2 = 0;

int main(void)
{
    /* Replace with your application code */
	initGPIO();
	initTimer();
	initI2CSlave();
	
	sei();
	//ISR 인터럽트 함수를 발생하기 위하여 사용
	//sei();는 인터럽트 함수를 활성화하기 위해
	//cli();는 인터럽트 함수를 비활성화하기 위해사용
	
    while (1) 
    {
		
		
		
		
    }
}

//초기화 함수

void initTimer()
{
	// Timer/Counter 1 설정 (CTC 모드, 분주비 설정 등)
	TCCR1B |= (1 << WGM12);  // CTC 모드 설정
	OCR1A = 15624;           // 1초마다 인터럽트 발생을 위한 TOP 값 설정
	TIMSK |= (1 << OCIE1A); // 타이머 인터럽트 활성화
	TCCR1B |= (1 << CS12) | (1 << CS10);  // 분주비 1024로 설정
	
}

void initI2CSlave() 
{
	TWSR = 0x00;
	TWBR = 0x48;
	// I2C 주소 설정
	TWAR = 0x30<<1;
	//I2C 슬레이브 주소를 0x40으로 설정하는것

	
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) ;
	//TWCr은 슬레이브 rec,tran 모드일경우 TWEA = 1, TWEN = 1, 밑에 인터럽트를 발생할것이기 때문에 인터럽트를 클리어하기위해 TWINT를 1로
	// TWI 슬레이브 수신 완료 인터럽트 활성화
	TWCR |= (1 << TWIE);
	//TWIE = 1 로 사용하여 TWI 인터럽트 활성화
	
}
void initGPIO()
{
	DDRA = 0xFF; //A라인 USB 1~8 power on/off 출력으로 사용
	DDRB = 0xFD; //B라인 USB 1~7 mode change
	DDRC = 0xFF; //C라인 USB 9~16 power on/off 출력으로 사용
	DDRD = 0xF0; //D라인 USB 8~11 mode change
	DDRE = 0x1C; //E라인 USB 12~14 mode change
	DDRF = 0x60; //F라인 USB 15,16 mode change
	DDRG = 0x07;
	
	PORTA = 0xFF;
	PORTB |= 0xFD;
	PORTC = 0xFF;
	PORTD |=0x03;
	PORTD |= 0xF0;
	PORTE |= 0x1C;
	PORTF |= 0x60;
	PORTG = 0xFF;
	
}
//ISR 인터럽트 함수

ISR(TWI_vect) {
	switch (TW_STATUS) {
		case TW_SR_SLA_ACK: // 0x60
							
							
							break;
		case TW_SR_DATA_ACK: //0x80
							//PORTG &= ~(0x04);
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
								case 1 : 
										break;
								case 2 : 
										POWER_L_ONOFF(i2c_buf[2]);
										break;
								case 3 :
										POWER_H_ONOFF(i2c_buf[3]);
										break;
								case 4 :
										Modechange_L(i2c_buf[4]);
										break;
								case 5 :
										Modechange_H(i2c_buf[5]);
										break;
								case 6 :
										Power_ONOFF_ALL(i2c_buf[6]);
										break;
								case 7 :
										Modechange_ALL(i2c_buf[7]);
										break;
							}
							break;
		case TW_ST_SLA_ACK: // A8
		case TW_ST_DATA_ACK: // B8
							switch (reg_index)
							{
								case 0 :
									TWDR = i2c_buf[reg_index];
								case 1 :
									//TWDR = i2c_buf[reg_index];
									break;
								case 2 :
									portAvalue = 0x00;
								
									portAvalue |= ((PORTA & 0x01)<<1);
									portAvalue |= ((PORTA & 0x02)>>1);
									portAvalue |= ((PORTA & 0x04)<<1);
									portAvalue |= ((PORTA & 0x08)>>1);
									portAvalue |= ((PORTA & 0x10)<<1);
									portAvalue |= ((PORTA & 0x20)>>1);
									portAvalue |= ((PORTA & 0x40)<<1);
									portAvalue |= ((PORTA & 0x80)>>1);
								
									TWDR = portAvalue;
									//POWER_L_ONOFF(i2c_buf[2]);
									break;
								case 3 :
									portCvalue = 0x00;
									
									portCvalue |= ((PORTC & 0x01)<<1);
									portCvalue |= ((PORTC & 0x02)>>1);
									portCvalue |= ((PORTC & 0x04)<<1);
									portCvalue |= ((PORTC & 0x08)>>1);
									portCvalue |= ((PORTC & 0x10)>>1);
									portCvalue |= ((PORTC & 0x20)>>1);
									portCvalue |= ((PORTC & 0x40)<<1);
									portCvalue |= ((PORTC & 0x80)>>1);
									TWDR = portCvalue;
									//POWER_H_ONOFF(i2c_buf[3]);
									break;
								case 4 :
								
									portValue1 = 0x00;
									
									portValue1 |= ((PORTB & 0x04)>>2);
									portValue1 |= ((PORTB & 0x01)<<1);
									portValue1 |= ((PORTB & 0x10)>>2);
									portValue1 |= (PORTB & 0x08);
									portValue1 |= ((PORTB & 0x40)>>2);
									portValue1 |= (PORTB & 0x20);
									portValue1 |= ((PORTD & 0x10)<<2);
									portValue1 |= (PORTB & 0x80);
									/*
									portValue1 |= ((PORTD & 0x10) >> 4);
									portValue1 |= (PORTB & 0x80);
									portValue1 |= (PORTB & 0x40);
									portValue1 |= (PORTB & 0x20);//
									portValue1 |= PORTB & 0x10 ;
									portValue1 |= ((PORTB & 0x08) << 2);
									portValue1 |= ((PORTB & 0x04) << 5);//
									portValue1 |= ((PORTB & 0x01) << 6);// 
									*/
									TWDR = portValue1;
									//Modechange_L(i2c_buf[4]);
									break;
								case 5 :
								
									portValue2 = 0x00;
									
									portValue2 |= ((PORTD & 0x40) >> 6);
									portValue2 |= ((PORTD & 0x20) >> 4);
									portValue2 |= (PORTE & 0x04);
									portValue2 |= ((PORTD & 0x80) >> 4);
									portValue2 |= (PORTE & 0x10);
									portValue2 |= ((PORTE & 0x08) << 2);
									portValue2 |= (PORTF & 0x40);
									portValue2 |= ((PORTF & 0x20) << 2);
								
								/*
									portValue2 = 0x00;
									portValue2 |= ((PORTF & 0x80)>>7);
									portValue2 |= ((PORTF & 0x40) >> 3);
									portValue2 |= ((PORTE & 0x10) >> 2);
									portValue2 |= PORTE & 0x08;
									portValue2 |= ((PORTE & 0x04) << 2);
									portValue2 |= ((PORTD & 0x80) >> 2);
									portValue2 |= PORTD & 0x40;
									portValue2 |= ((PORTD & 0x20) << 2);
									*/
									TWDR = portValue2;
									//Modechange_H(i2c_buf[5]);
									break;
								case 6 :
									
									Power_ONOFF_ALL(i2c_buf[6]);
									break;
								case 7 :
									 Modechange_ALL(i2c_buf[7]);
									break;
								case 8:
								
								
									portAvalue = 0x00;
									portCvalue = 0x00;
									portValue1 = 0x00;
									portValue2 = 0x00;
									
									portAvalue |= ((PORTA & 0x01)<<1);
									portAvalue |= ((PORTA & 0x02)>>1);
									portAvalue |= ((PORTA & 0x04)<<1);
									portAvalue |= ((PORTA & 0x08)>>1);
									portAvalue |= ((PORTA & 0x10)<<1);
									portAvalue |= ((PORTA & 0x20)>>1);
									portAvalue |= ((PORTA & 0x40)<<1);
									portAvalue |= ((PORTA & 0x80)>>1);
						
									portCvalue |= ((PORTC & 0x01)<<1);
									portCvalue |= ((PORTC & 0x02)>>1);
									portCvalue |= ((PORTC & 0x04)<<1);
									portCvalue |= ((PORTC & 0x08)>>1);
									portCvalue |= ((PORTC & 0x10)>>1);
									portCvalue |= ((PORTC & 0x20)>>1);
									portCvalue |= ((PORTC & 0x40)<<1);
									portCvalue |= ((PORTC & 0x80)>>1);
									
									portValue1 |= PORTD4;
									portValue1 |= (PORTB7 << 1);
									portValue1 |= (PORTB6 << 2);
									portValue1 |= (PORTB5 << 3);
									portValue1 |= (PORTB4 << 4);
									portValue1 |= (PORTB3 << 5);
									portValue1 |= (PORTB2 << 6);
									portValue1 |= (PORTB0 << 7);
									
									portValue2 |= PORTF7;
									portValue2 |= (PORTF6 << 1);
									portValue2 |= (PORTE4 << 2);
									portValue2 |= (PORTE3 << 3);
									portValue2 |= (PORTE2 << 4);
									portValue2 |= (PORTD7 << 5);
									portValue2 |= (PORTD6 << 6);
									portValue2 |= (PORTD5 << 7);
									
									send_buf[0] = portAvalue;
									send_buf[1] = portCvalue;
									send_buf[2] = portValue1;
									send_buf[3] = portValue2;
									for(int i = 0; i<4; i++)
									{
										TWDR= send_buf[i];
										TWCR |= (1 << TWINT) | (1 << TWEA);
										while (!(TWCR & (1 << TWINT)));
									}
									break;
							}
							
								/*if (reg_index == 0)
								{
									TWDR = i2c_buf[reg_index];
								}
								else if (reg_index == 1)
								{
																	
									send_buf[0] = PORTA;
									send_buf[1] = PORTC;
									
									portValue1 = 0x00;
									portValue2 = 0x00;
									
									portValue1 |= PORTD4;
									portValue1 |= (PORTB7 << 1);
									portValue1 |= (PORTB6 << 2);
									portValue1 |= (PORTB5 << 3);
									portValue1 |= (PORTB4 << 4);
									portValue1 |= (PORTB3 << 5);
									portValue1 |= (PORTB2 << 6);
									portValue1 |= (PORTB0 << 7);
									
									portValue1 |= PIND4;
									portValue1 |= (PINB7 << 1);
									portValue1 |= (PINB6 << 2);
									portValue1 |= (PINB5 << 3);
									portValue1 |= (PINB4 << 4);
									portValue1 |= (PINB3 << 5);
									portValue1 |= (PINB2 << 6);
									portValue1 |= (PINB0 << 7);
									
									portValue2 |= PORTF7;
									portValue2 |= (PORTF6 << 1);
									portValue2 |= (PORTE4 << 2);
									portValue2 |= (PORTE3 << 3);
									portValue2 |= (PORTE2 << 4);
									portValue2 |= (PORTD7 << 5);
									portValue2 |= (PORTD6 << 6);
									portValue2 |= (PORTD5 << 7);
									
									//send_buf[2] = (uint8_t)((1 << PORTB0) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4) | (1 << PORTB5) | (1 << PORTB6) | (1 << PORTB7) | (1 << PORTD4));
									//send_buf[2] = portBValue;
									//send_buf[3] = (uint8_t)((1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7) | (1 << PORTE2) | (1 << PORTE3) | (1 << PORTE4) | (1 << PORTF6) | (1 << PORTF7));
									send_buf[2] = portValue1;
									send_buf[3] = portValue2;
									for(int i = 0; i<4; i++)
									{
										TWDR= send_buf[i];
										TWCR |= (1 << TWINT) | (1 << TWEA);
										while (!(TWCR & (1 << TWINT)));
									}
								}*/
							
							break;
		case TW_ST_DATA_NACK: 
		case TW_SR_STOP: // A0
							
							dataCount = 0;
							
							break;
		default:
							TWCR = (1 << TWINT) | (1 << TWEA);
							break;
	}
	TWCR |= (1 << TWINT);
}
ISR(TIMER1_COMPA_vect)
{
	
	
}



void ask_version(int data)
{
	TWDR = data;
}

void reset_USB()
{
	for (int i =0; i<8;i++)
	{
		PORTA = (PORTA<<i)|0x01;
	}
	for (int i =0; i<8;i++)
	{
		PORTC = (PORTC<<i)|0x01;
		
	}
}

//power ON or OFF
void POWER_L_ONOFF(uint16_t data)
{
	switch(data)
	{
		case 0x00:
				PORTA =  (PORTA&0x00);
				//PORTC = (PORTC&0x00);
				break;
		case 0x01 ://1
				//PORTC =  (PORTC|0x04);//PV버전
				PORTA =  (PORTA|0x02);//REV.A버전
				break;
		case 0x02 ://2
				//PORTC =  (PORTC|0x08);//PV버전
				PORTA =  (PORTA|0x01);//REV.A버전
				break;
		case 0x04 ://3
				//PORTC =  (PORTC|0x10);//PV버전
				PORTA =  (PORTA|0x08);//REV.A버전
				break;
		case 0x08 ://4
				//PORTC =  (PORTC|0x20);//PV버전
				PORTA =  (PORTA|0x04);//REV.A버전
				break;
		case 0x10 ://5
				//PORTC =  (PORTC|0x40);//PV버전
				PORTA =  (PORTA|0x20);//REV.A버전
				break;
		case 0x20 ://6
				//PORTC =  (PORTC|0x80);//PV버전
				PORTA =  (PORTA|0x10);//REV.A버전
				break;
		case 0x40 ://7
				//PORTC =  (PORTC|0x02);//PV버전
				PORTA =  (PORTA|0x80);//REV.A버전
				break;
		case 0x80 ://8
				//PORTC =  (PORTC|0x01);//PV버전
				PORTA =  (PORTA|0x40);//REV.A버전
				break;
		case 0x7F ://8
				//PORTC = (PORTC & 0xFE);//PV버전
				PORTA = (PORTA & 0xBF);//REV.A버전
				break;
		case 0xBF ://7
				//PORTC = (PORTC & 0xFD);//PV버전
				PORTA = (PORTA & 0x7F);//REV.A버전
				break;
		case 0xDF ://6
				//PORTC = (PORTC & 0x7F);//PV버전
				PORTA = (PORTA & 0xEF);//REV.A버전
				break;
		case 0xEF ://5
				//PORTC = (PORTC & 0xBF);//PV버전
				PORTA = (PORTA & 0xDF);//REV.A버전
				break;
		case 0xF7 ://4
				//PORTC = (PORTC & 0xDF);//PV버전
				PORTA = (PORTA & 0xFB);//REV.A버전
				break;
		case 0xFB ://3
				//PORTC = (PORTC & 0xEF);//PV버전
				PORTA = (PORTA & 0xF7);//REV.A버전
				break;
		case 0xFD ://2
				//PORTC = (PORTC & 0xF7);//PV버전
				PORTA = (PORTA & 0xFE);//REV.A버전
				break;
		case 0xFE ://1
				//PORTC = (PORTC & 0xFB);//PV버전
				PORTA = (PORTA & 0xFD);//REV.A버전
				break;
		case 0xFF :
				PORTA =  (PORTA|0xFF);
				//PORTC =  (PORTC|0xFF);
				break;
	}
}



void POWER_H_ONOFF(uint16_t data)
{
	switch(data)
	{
		case 0x00:
				//PORTA =  (PORTA & 0x00);
				PORTC = (PORTC&0x00);
				break;
		case 0x01 ://9
				//PORTA =  (PORTA|0x80);//PV버전
				PORTC =  (PORTC|0x02);//REV.A버전
				break;
		case 0x02 ://10
				//PORTA =  (PORTA|0x40);//PV버전
				PORTC =  (PORTC|0x01);//REV.A버전
				break;
		case 0x04 ://11
				//PORTA =  (PORTA|0x20);//PV버전
				PORTC =  (PORTC|0x08);//REV.A버전
				break;
		case 0x08 ://12
				//PORTA =  (PORTA|0x10);//PV버전
				PORTC =  (PORTC|0x04);//REV.A버전
				break;
		case 0x10 ://13
				PORTA =  (PORTA|0x08);//PV버전
				break;
		case 0x20 ://14
				PORTA =  (PORTA|0x04);//PV버전
				break;
		case 0x40 ://15
				PORTA =  (PORTA|0x02);//PV버전
				break;
		case 0x80 ://16
				PORTA =  (PORTA|0x01);//PV버전
				break;
		case 0x7F ://16
				PORTA = (PORTA & 0xFE);//PV버전
				break;
		case 0xBF ://15
				PORTA = (PORTA & 0xFD);//PV버전
				break;
		case 0xDF ://14
				PORTA = (PORTA & 0xFB);//PV버전
				break;
		case 0xEF ://13
				PORTA = (PORTA & 0xF7);//PV버전
				break;
		case 0xF7 ://12
				//PORTA = (PORTA & 0xEF);//PV버전
				PORTC = (PORTC & 0xFB);//REV.A버전
				break;
		case 0xFB ://11
				//PORTA = (PORTA & 0xDF);//PV버전
				PORTC = (PORTC & 0xF7);//REV.A버전
				break;
		case 0xFD ://10
				//PORTA = (PORTA & 0xBF);//PV버전
				PORTC = (PORTC & 0xFE);//REV.A버전
				break;
		case 0xFE ://9
				//PORTA = (PORTA & 0x7F);//PV버전
				PORTC = (PORTC & 0xFD);//REV.A버전
				break;
		case 0xFF :
				//PORTA =  (PORTA|0xFF);
				PORTC =  (PORTC|0xFF);
				break;
	}
}

void Power_ONOFF_ALL(uint16_t data)
{
	switch(data)
	{
		case 0x00:
				PORTA =  (PORTA&0x00);
				PORTC = (PORTC&0x00);
				break;
		case 0xFF :
				PORTA =  (PORTA|0xFF);
				PORTC =  (PORTC|0xFF);
				break;
	}
}
// CDP or DCP mode

void Modechange_L(uint16_t data)
{
	switch(data)
	{	
		case 0x00 : //all dcp
					PORTB = (PORTB & 0x02);
					PORTD = (PORTD & 0xEF);
					break;	
		case 0x01 ://1
					PORTB = (PORTB | 0x04);
					break;
		case 0x02 ://2
					PORTB = (PORTB | 0x01);
					break;
		case 0x04 ://3
					PORTB = (PORTB | 0x10);
					break;
		case 0x08 ://4
					PORTB = (PORTB | 0x08);
					break;
		case 0x10 ://5
					PORTB = (PORTB | 0x40);
					break;
		case 0x20 ://6
					PORTB = (PORTB | 0x20);
					break;
		case 0x40 ://7
					PORTD = (PORTD | 0x10);
					break;
		case 0x80 ://8
					PORTB = (PORTB | 0x80);
					break;
		case 0x7F ://8
					PORTB = (PORTB & 0x7F);
					break;
		case 0xBF ://7
					PORTD = (PORTD & 0xEF);
					break;
		case 0xDF ://6
					PORTB = (PORTB & 0xDF);
					break;
		case 0xEF ://5
					PORTB = (PORTB & 0xBF);
					break;
		case 0xF7://4
					PORTB = (PORTB & 0xF7);
					break;
		case 0xFB ://3
					PORTB = (PORTB & 0xEF);
					break;
		case 0xFD ://2
					PORTB = (PORTB & 0xFE);
					break;
		case 0xFE ://1
					PORTB = (PORTB & 0xFB);
					break;
		case 0xFF ://all cdp
					PORTB = (PORTB | 0xFD);
					PORTD = (PORTD | 0x10);
					break;
	}

}

void Modechange_H(uint16_t data)
{
	switch(data)
	{
		case 0x00 :	//all dcp
					PORTD = (PORTD & 0x1F);
					PORTE = (PORTE & 0xE3);
					PORTF = (PORTF & 0x9F);
					break;
					
		case 0x01 ://9
					PORTD = (PORTD | 0x40);
					break;
		case 0x02 ://10
					PORTD = (PORTD | 0x20);
					break;
		case 0x04 ://11
					PORTE = (PORTE | 0x04);
					break;
		case 0x08 ://12
					PORTD = (PORTD | 0x80);
					break;
		case 0x10 ://
					PORTE = (PORTE | 0x10);
					break;
		case 0x20 ://14
					PORTE = (PORTE | 0x08);
					break;
		case 0x40 ://15
					PORTF = (PORTF | 0x40);
					break;
		case 0x80 ://16
					PORTF = (PORTF | 0x20);
					break;
		case 0x7f ://16
					PORTF = (PORTF & 0xDF);
					break;
		case 0xBF ://15
					PORTF = (PORTF & 0xBF);
					break;
		case 0xDF ://14
					PORTE = (PORTE & 0xF7);
					break;
		case 0xEF ://13
					PORTE = (PORTE & 0xEF);
					break;
		case 0xF7 ://12
					PORTD = (PORTD & 0x7F);
					break;
		case 0xFB ://11
					PORTE = (PORTE & 0xFB);
					break;
		case 0xFD ://10
					PORTD = (PORTD & 0xDF);
					break;
		case 0xFE ://9
					PORTD = (PORTD & 0xBF);
					break;
		case 0xFF : //all cdp
					PORTD = (PORTD | 0xE0);
					PORTE = (PORTE | 0x1C);
					PORTF = (PORTF | 0xC0);
					break;
	}
	

}

void Modechange_ALL(uint16_t data)
{
	switch (data)
	{
		case 0x00 : //all dcp
				PORTB = (PORTB & 0x02);
				PORTD = (PORTD & 0xEF);
				PORTD = (PORTD & 0x1F);
				PORTE = (PORTE & 0xE3);
				PORTF = (PORTF & 0x3F);
				break;
		case 0xFF :
				PORTB = (PORTB | 0xFD);
				PORTD = (PORTD | 0x10);
				PORTD = (PORTD | 0xE0);
				PORTE = (PORTE | 0x1C);
				PORTF = (PORTF | 0xC0);
				
	}
	
}
