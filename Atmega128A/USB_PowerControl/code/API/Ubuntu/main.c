#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <libudev.h> 
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


#define I2C_DEVICE "/dev/i2c-1"

#define NXE_REG_FW_VERSION 0x00 //FW_VERSION 읽기 위하여 0x00
#define NXE_REG_USB_POWER_LSB_INDEX 0x02 // 1~8번 USB ON/OFF
#define NXE_REG_USB_POWER_MSB_INDEX 0x03 // 9~16번 USB ON/OFF
#define NXE_REG_USB_MODE_CHANGE_LSB_INDEX 0x04 // 1~8번 USB CDP/DCP
#define NXE_REG_USB_MODE_CHANGE_MSB_INDEX 0x05 // 9~16번 USB CDP/DCP


int init_i2c(int *i2c_port); //i2c 초기화 
int check_cmd(int argc,char *argv[],int *cmd,int *data); //argc 와 argv를 체크하여 내가 원하는 값을 받았을경우 cmd와 data로 값을 저장
/*
cmd =0 get firmware ver
cmd =1 USB POWER on
cmd =2 USB poweroff
cmd =3 USB mode change to cdp
cmd =4 USB mode change to dcp
cmd =5 USB power all on
cmd =6 USB power all off
cmd =7 USB mode change all to cdp 
cmd =8 USB mode change all to dcp
cmd =9 USB get all data
*/
void get_version();//cmd = 0 
int set_usb_power_on(int data);//cmd = 1
int set_usb_power_off(int data);//cmd = 2
int set_usb_mode_cdp();//cmd = 3
int set_usb_mode_dcp();//cmd = 4
void set_usb_power_all_on();//cmd = 5
void set_usb_power_all_off();//cmd = 6
void set_usb_mode_all_cdp();//cmd = 7
void set_usb_mode_all_dcp();//cmd = 8
void usb_all_data();//cmd = 9



void Check_file(const char *dirPath,int i); //숫자-숫자 로 되어있는 파일을 찾기 위하여 만든 함수
void search_serial (const char* data,int i); //폴더를 찾았을경우 시리얼 번호를 얻기 위하여 만든 함수
char dir[10]; 
char serial_number[16];


typedef struct USB_DATA
{
	int port_num;//port number을 사용하기 위하여 int형
	bool port_power_state; //on과 off만 존재하기 때문에 bool사용
	bool port_config_state; //마찬가지로 cdp와 dcp만 사용하기 때문에 bool사용
	int adb_num[2]; // adb-num는 3-2와 같이 숫자-숫자 로 구성되어있어 앞에 숫자를 [0]에 뒤에 숫자를 [1]에 저장하기 위하여 [2]만큼 배정
	char SERIAL[16]; //serial번호는 엄청 긴건 엄청 길기때문에 16으로 크기를 제한
}USB_state;
//USB_state의 이름으로 만든 USB 전체 구조체

USB_state USB[16]; //USB_state를 1~16번까지 넣을 구조체 배열
int i2c_port =0; 
int argv_cmd =0; 
int control_data =0;
uint8_t send_buf[5] = {0,}; //i2c로 데이터를 보내기 위하여 send_buf를 선언  
uint8_t rxbuffer[5] = {0,}; // i2c로 데이터를 받기 위하여 rxbuffer를 선언
uint8_t rx_buf[5] = {0,};  //받은 데이터를 한 배열로 저장하기 위하여 rx_buf 선언

int main(int argc, char *argv[])//gcc로 명령어를 전달받을경우 그 다음 텍스트의 대하여 알고 처리하기 위하여 인자 선언
{	
	if (init_i2c(&i2c_port) !=0) //만약 i2c가 연결되어있지 않을경우 다음 write나 read로 넘어가지 않도록 return 사용
	{
		return -1; //정상적일경우 0으로 return 되지만 -1로 오류를 표현 But -1로 설정한 이유는 찾지 못함
	}
	else //정상적으로 0이 return 되었을경우 다음 check문으로 들어간다. 
	{
		if(check_cmd(argc,argv,&argv_cmd,&control_data) == 0) //int 형식으로 된 check_cmd가 정상적으로 0으로 return시 if문 안으로 
		{													//인자는 입력된 argc,argv를 가져온후 안에서 cmd,data를 계산한 후 밖으로 가져나오기 위해 포인터 사용  
			switch (argv_cmd)//check_cmd에서 가져온 argv_cmd를 사용하여 case문 들어간다
			{
				case 0: //get firmware ver
					get_version();
					break;
				case 1 ://cmd =1 USB POWER on
					if(set_usb_power_on(control_data) !=0)//check_cmd에서 가져온 control_data를 사용하는데 있어 1~16의 숫자가 아닌 다른 숫자를 받을경우 
					{									 //return -1로 0이 아닌 값을 return했을경우 바로 종료할수 있게 설정
						return -1;
					}
					else //정상적으로 0을 return 했을경우 break로 다음 진행
					{
						break;
					}
				case 2 ://cmd =2 USB poweroff
					if(set_usb_power_off(control_data)!=0)//check_cmd에서 가져온 control_data를 사용하는데 있어 1~16의 숫자가 아닌 다른 숫자를 받을경우 
					{									 //return -1로 0이 아닌 값을 return했을경우 바로 종료할수 있게 설정
						return -1;
					}
					else
					{
						break;
					}
				case 3 ://cmd =3 USB mode change to cdp
					if(set_usb_mode_cdp(control_data)!=0)//check_cmd에서 가져온 control_data를 사용하는데 있어 1~16의 숫자가 아닌 다른 숫자를 받을경우 
					{									 //return -1로 0이 아닌 값을 return했을경우 바로 종료할수 있게 설정
						return -1;
					}
					else
					{
						break;
					}
				case 4 ://cmd =4 USB mode change to dcp
					if(set_usb_mode_dcp(control_data) !=0)//check_cmd에서 가져온 control_data를 사용하는데 있어 1~16의 숫자가 아닌 다른 숫자를 받을경우 
					{									 //return -1로 0이 아닌 값을 return했을경우 바로 종료할수 있게 설정
						return -1;
					}
					else 
					{
						break;
					}
				case 5 ://cmd =5 USB power all on
					set_usb_power_all_on();
					break;
				case 6 ://cmd =6 USB power all off
					set_usb_power_all_off();
					break;
				case 7 ://cmd =7 USB mode change all to cdp
					set_usb_mode_all_cdp();
					break;
				case 8 ://cmd =8 USB mode change all to dcp
					set_usb_mode_all_dcp();
					break;
				case 9 ://cmd =9 USB get all data
					usb_all_data(USB); // USB_state를 USB[16] 으로 배열로 선언한것을 넣어서 사용
					break;
				default :
					break;
			}

			argv_cmd = 0;//다시 argv_cmd를 0으로 초기화 하기 위하여 But 필요 없을거 같음
		}
		else 
		{
			return 0; //else로 예외사항에 return을 사용하여 
		}
	close(i2c_port);//통신을 끝낸후 i2c_port를 닫아준다.

	}
	for (int i=0;i<8;i++)//for문을 사용하여 i로 1d.0부터 1d.7까지 검색하기 위하여 반복문 사용
	{
		char buf[256]; //문자열을 사용하여 주소를 받기 위하여 buf를 char형식 사용
		sprintf(buf, "/sys/devices/pci0000:00/0000:00:1d.%d/", i); // i를 계속 변경될경우 buf에 문자열을 저장하기 위하여 sprintf문을 사용
		Check_file(buf, i);	//1d.i 의 고정 주소를 사용하여 숫자-숫자가 있을경우를 찾기 위하여 함수 사용
	}
	//printf("%s\r\n",ptr_usb->SERIAL);
}



int init_i2c(int *i2c_port) //i2c를 초기화 하기 위하여 원래 void 함수 형식으로 사용하였지만, return 으로 연결되지 않았을경우 종료시키기 위하여 int형식으로 변경 
{
	*i2c_port = open(I2C_DEVICE, O_RDWR); //open을 사용 하여 i2c를 연결 
	if(*i2c_port <0) //open으로 연결 하지 못할경우 -1로 return되기 때문에 0보다 작을경우 연결되지 못하였기 때문에 return -1
	{
		perror("i2c not connect");
		return -1;
	}
	if (ioctl(*i2c_port,I2C_SLAVE, 0x30) <0) //ioctl를 사용하여 0x30으로 address주소 설정하여 연결했을경우 연결이 되지 않는다면 -1 
	{
		perror("i2c correct slave add");
		return -1;
	}
	return 0;
}
int check_cmd(int argc,char *argv[],int *cmd,int *data)//int main에서 argc와 argv를 전달받아 cmd와 data에 값을 저장하기 위하여 인자로 선언
{
	if (strcmp(argv[1],"get") == 0 || strcmp(argv[1],"GET")==0) // get을 받았을때 cmd를 0으로 data는 필요없기때문에
	{
		*cmd = 0;
		return 0;
	}
	else if(strcmp(argv[1],"help") == 0 || strcmp(argv[1],"HELP") == 0)//help를 사용하여 사용자가 명령어를 알수 있도록 하기 위하여 
	{
		printf("./sudo serial_final_gcc text -> correct command\r\n");
		printf("  -text list-  \r\n");
		printf("test        -> all USB : PORT NUMBER, on/off, cdp/dcp, ADB PORT NUMBER, SERIAL\r\n");
		printf("            ->모든 USB : 포트번호, ON/OFF, CDP/DCP, ADB 포트 번호, 시리얼 번호\r\n");
		printf("on number   -> number is 1~16, You can turn on the number of USB you want\r\n");
		printf("on 숫자      ->숫자는 1~16, ON을 원하는 포트 번호를 적어주세요\r\n");
		printf("on all      -> You can turn on all USB\r\n");
		printf("            ->모든 USB를 off할수 있다.\r\n");
		printf("off number  -> number is 1~16, You can turn off the number of USB you want\r\n");
		printf("off 숫자     ->숫자는 1~16, Off을 원하는 포트 번호를 적어주세요.\r\n");
		printf("off all     -> You can turn off all USB\r\n");
		printf("            ->모든 USB를 off할수 있다.\r\n");
		printf("cdp number  -> number is 1~16, You can change cdp mode to the USB you want\r\n");
		printf("cdp 숫자     ->숫자는 1~16, cdp 모드를 원하는 포트 번호를 적어주세요\r\n");
		printf("cdp all     -> You can change cdp mode all USB\r\n");
		printf("            -> 모든 USB를 cdp모드로 변경할수있다.\r\n");
		printf("dcp number  -> number is 1~16, You can change dcp mode to the USB you want\r\n");
		printf("dcp 숫자     ->숫자는 1~16, dcp 모드를 원하는 포트 번호를 적어주세요\r\n");
		printf("dcp all     -> You can change dcp mode all USB\r\n");
		printf("            -> 모든 USB를 dep모드로 변경할수있다.\r\n");
		return -1;
		 
	}
	else if (strcmp(argv[1],"on") == 0 || strcmp(argv[1],"ON")==0) //on을 사용하였을 경우 
	{
		if(strcmp(argv[2],"all") == 0 || strcmp(argv[2],"ALL") == 0) // on all 일 경우
		{
			*cmd = 5; 
			return 0;
		}
		else if (*data = atoi(argv[2]) !=0)//정수가 아닐경우 0으로 return 되기때문에 0이 아닐경우
		{
			*cmd =1;
			*data = atoi(argv[2]);
			return 0;
		}
		else //정수도 아닌 all도 아닐경우 
		{
			return -1;
		}
	}
	else if (strcmp(argv[1],"off") == 0 || strcmp(argv[1],"OFF")==0)//off를 사용하였을 경우 
	{
		if(strcmp(argv[2],"all") == 0 || strcmp(argv[2],"ALL") == 0)//off all 일 경우
		{
			*cmd = 6;
			return 0;
		}
		else if (*data = atoi(argv[2]) !=0)//정수가 아닐경우 0으로 return 되기때문에 0이 아닐경우
		{
			*cmd =2;
			*data = atoi(argv[2]);
			return 0;
		}
		else //정수도 아닌 all도 아닐경우 
		{
			return -1;
		}
	}
	else if (strcmp(argv[1],"cdp") == 0 || strcmp(argv[1],"CDP")==0)//cdp를 사용하였을 경우 
	{
		if(strcmp(argv[2],"all") == 0 || strcmp(argv[2],"ALL") == 0)//cdp all 일 경우
		{
			*cmd = 7;
			return 0;
		}
		else if (*data = atoi(argv[2]) !=0)//정수가 아닐경우 0으로 return 되기때문에 0이 아닐경우
		{
			*cmd =3;
			*data = atoi(argv[2]);
			return 0;
		}
		else //정수도 아닌 all도 아닐경우 
		{
			return -1;
		}
	}
	else if (strcmp(argv[1],"dcp") == 0 || strcmp(argv[1],"DCP")==0)//dcp를 사용하였을 경우
	{
		if(strcmp(argv[2],"all") == 0 || strcmp(argv[2],"ALL") == 0)//dcp all 일 경우
		{
			*cmd = 8;
			return 0;
		}
		else if (*data = atoi(argv[2]) !=0)//정수가 아닐경우 0으로 return 되기때문에 0이 아닐경우
		{
			*cmd =4;
			*data = atoi(argv[2]);
			return 0;
		}
		else //정수도 아닌 all도 아닐경우
		{
			return -1;
		}
	}
	else if (strcmp(argv[1],"test") == 0 || strcmp(argv[1],"TEST")==0)//test를 사용하였을 경우 
	{
		*cmd =9;
		return 0;
	}
	else //그 외 일경우 return -1을 사용하여 탈출
	{
		return -1;
	}
	
}
void get_version()//cmd = 0
{
	send_buf[0] = NXE_REG_FW_VERSION;//#define으로 위에 0x00으로 설정
	write(i2c_port,send_buf,1); //write로 위에 저장된 send_buf를 i2c를 통하여 보냄
	read(i2c_port,rxbuffer,1); //read로 i2c로 오는 정보를 받는다
	printf("FIRMWARE VERSION : %x\r\n",rxbuffer[0]); //받은 정보를 사용
}
int set_usb_power_on(int data)//cmd = 1
{
	if (data<9)//1~8번까지 9~16번까지로 나누기 위하여 if문 사용
	{
		send_buf[0] = NXE_REG_USB_POWER_LSB_INDEX; //#define으로 위에 0x02으로 설정
		send_buf[1] = (0x01 << (data -1));//8,7,6,5,4,3,2,1 순으로 비트를 설정해놨기 때문에 
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+1,(rxbuffer[0] & (0x01<<i)) ? "ON":"OFF"); //받은 정보를 & (and)와 ?로 ON또는 OFF로 표현  
		}
		return 0;
		
	}
	else if (8<data<17)
	{
		send_buf[0] = NXE_REG_USB_POWER_MSB_INDEX;//#define으로 위에 0x03으로 설정
		send_buf[1] = (0x01 << (data-9));//16,15,14,13,12,11,10,9 순으로 비트를 설정해놨기 때문
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+9,(rxbuffer[0] & (0x01<<i)) ? "ON":"OFF");  //받은 정보를 & (and)와 ?로 ON또는 OFF로 표현 
		}
		return 0;
	} 
	else 
	{
		return -1;
	}
	

}
int set_usb_power_off(int data)//cmd = 2
{
	if (data<9)
	{
		send_buf[0] = NXE_REG_USB_POWER_LSB_INDEX;//#define으로 위에 0x02으로 설정
		send_buf[1] = ~(0x01 << (data -1));//8,7,6,5,4,3,2,1 순으로 비트를 설정해놨기 때문에 ~(not)을 사용해서 반전 시킨다 
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+1,(rxbuffer[0] & (0x01<<i)) ? "ON":"OFF"); //받은 정보를 & (and)와 ?로 ON또는 OFF로 표현 
		}
		return 0;
	}
	else if (8<data<17)
	{
		send_buf[0] = NXE_REG_USB_POWER_MSB_INDEX;//#define으로 위에 0x03으로 설정
		send_buf[1] = ~(0x01 << (data-9));//16,15,14,13,12,11,10,9 순으로 비트를 설정해놨기 때문에 ~(not)을 사용해서 반전 시킨다 
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+9,(rxbuffer[0] & (0x01<<i)) ? "ON":"OFF"); //받은 정보를 & (and)와 ?로 ON또는 OFF로 표현 
		}
		return 0;
	} 
	else 
	{
		return -1;
	}
}
int set_usb_mode_cdp(int data)//cmd = 3
{
	if (data<9)
	{
		send_buf[0] = NXE_REG_USB_MODE_CHANGE_LSB_INDEX;//#define으로 위에 0x04으로 설정
		send_buf[1] = (0x01 << (data -1));
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+1,(rxbuffer[0] & (0x01<<i)) ? "CDP":"DCP"); 
		}
		return 0;
	}
	else if (8<data<17)
	{
		send_buf[0] = NXE_REG_USB_MODE_CHANGE_MSB_INDEX;//#define으로 위에 0x05으로 설정
		send_buf[1] = (0x01 << (data-9));
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+9,(rxbuffer[0] & (0x01<<i)) ? "CDP":"DCP"); 
		}
		return 0;
	} 
	else 
	{
		return -1;
	}
	
	
}
int set_usb_mode_dcp(int data)//cmd = 4
{
	if (data<9)
	{
		send_buf[0] = NXE_REG_USB_MODE_CHANGE_LSB_INDEX;
		send_buf[1] = ~(0x01 << (data -1));
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+1,(rxbuffer[0] & (0x01<<i)) ? "CDP":"DCP"); 
		}
		return 0;
	}
	else if (8<data<17)
	{
		send_buf[0] = NXE_REG_USB_MODE_CHANGE_MSB_INDEX;
		send_buf[1] = ~(0x01 << (data-9));
		write(i2c_port,send_buf,2);
		read(i2c_port,rxbuffer,1);
		for (int i=0; i<8;i++)
		{
			printf("USB %d : %s\r\n",i+9,(rxbuffer[0] & (0x01<<i)) ? "CDP":"DCP"); 
		}
		return 0;
	} 
	else 
	{
		return -1;
	}
}
void set_usb_power_all_on()//cmd = 5
{
	send_buf[0] = NXE_REG_USB_POWER_LSB_INDEX;
	send_buf[1] = 0xFF;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[0] = rxbuffer[0];//이 부분에서 다음 을 다시 받게 된다면 rxbuffer의 값이 바뀌기 때문에 배열 rx_buf에 저장 
	send_buf[0] = NXE_REG_USB_POWER_MSB_INDEX;
	send_buf[1] = 0xFF;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[1] = rxbuffer[0];
	rx_buf[2] = (rx_buf[0]&rx_buf[1]);
	printf("USB all %s\r\n",(rx_buf[2] & 0xFF ) ? "ON":"OFF");

}
void set_usb_power_all_off()//cmd = 6
{
	send_buf[0] = NXE_REG_USB_POWER_LSB_INDEX;
	send_buf[1] = 0x00;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[0] = rxbuffer[0];
	send_buf[0] = NXE_REG_USB_POWER_MSB_INDEX;
	send_buf[1] = 0x00;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[1] = rxbuffer[0];
	rx_buf[2] = (rx_buf[0]&rx_buf[1]);
	printf("USB all %s\r\n",(rx_buf[2] & 0x00) ? "ON":"OFF");


}
void set_usb_mode_all_cdp()//cmd = 7
{
	send_buf[0] = NXE_REG_USB_MODE_CHANGE_LSB_INDEX;
	send_buf[1] = 0xFF;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[0] = rxbuffer[0];
	send_buf[0] = NXE_REG_USB_MODE_CHANGE_MSB_INDEX;
	send_buf[1] = 0xFF;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[1] = rxbuffer[0];
	rx_buf[2] = (rx_buf[0]&rx_buf[1]);
	printf("USB all %s\r\n",(rx_buf[2] & 0xFF) ? "CDP":"DCP");
}
void set_usb_mode_all_dcp()//cmd = 8
{
	send_buf[0] = NXE_REG_USB_MODE_CHANGE_LSB_INDEX;
	send_buf[1] = 0x00;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[0] = rxbuffer[0]; 
	send_buf[0] = NXE_REG_USB_MODE_CHANGE_MSB_INDEX;
	send_buf[1] = 0x00;
	write(i2c_port,send_buf,2);
	read(i2c_port,rxbuffer,1);
	rx_buf[1] = rxbuffer[0];
	rx_buf[2] = (rx_buf[0]&rx_buf[1]);
	printf("USB all %s\r\n",(rx_buf[2] & 0x00) ? "CDP":"DCP");
}
void usb_all_data(USB_state *ptr_usb)//cmd = 9
{
	send_buf[0] = NXE_REG_USB_POWER_LSB_INDEX;
	write(i2c_port,send_buf,1);
	read(i2c_port,rxbuffer,1);//0x02의 값을 받기 위하여
	rx_buf[0] = rxbuffer[0];
	send_buf[0] = NXE_REG_USB_POWER_MSB_INDEX;
	write(i2c_port,send_buf,1);
	read(i2c_port,rxbuffer,1);//0x03의 값을 받기 위하여
	rx_buf[1] = rxbuffer[0];
	send_buf[0] = NXE_REG_USB_MODE_CHANGE_LSB_INDEX;
	write(i2c_port,send_buf,1);
	read(i2c_port,rxbuffer,1);//0x04의 값을 받기 위하여
	rx_buf[2] = rxbuffer[0];
	send_buf[0] = NXE_REG_USB_MODE_CHANGE_MSB_INDEX;
	write(i2c_port,send_buf,1);
	read(i2c_port,rxbuffer,1);//0x05의 값을 받기 위하여
	rx_buf[3] = rxbuffer[0];
	
	
	for (int i=0;i<8;i++)//1d.숫자에서 숫자가 0~7까지 있기때문에 연결되어 있는것을 검색
	{
		char buf[256];
		sprintf(buf, "/sys/devices/pci0000:00/0000:00:1d.%d/", i);//1d.숫자 까지가 고정이기 때문에 buf에 이 주소값을 저장하여 사용
		Check_file(buf, i);		
	}
	for (int i =1;i<17;i++)//i를 1~16까지 usb_number을 맞추기 위하여 저장 및 출력
	{
		if (i<9)//rx_buf의 값이 1~8까지 이기 떄문에 1~8,9~16까지 나눠서 사용
		{
			ptr_usb->port_num =i;
			ptr_usb->port_power_state = rx_buf[0]&(0x01<<(i-1));
			ptr_usb->port_config_state = rx_buf[2]&(0x01<<(i-1));
			printf("USB PORT NUMBER : %d  ",ptr_usb->port_num);
			printf("PORT POWER : %s  ",ptr_usb->port_power_state ? "ON":"OFF");
			printf("PORT MODE : %s  ",ptr_usb->port_config_state ? "CDP":"DCP");
			printf("ADB PORT NUM : %d-%d  ",ptr_usb->adb_num[0],ptr_usb->adb_num[1]);
			printf("SERIAL NUMBER : %s\r\n",ptr_usb->SERIAL);
		}
		else 
		{
			ptr_usb->port_num =i;
			ptr_usb->port_power_state = rx_buf[1]&(0x01<<(i-9));
			ptr_usb->port_config_state = rx_buf[3]&(0x01<<(i-9));
			printf("USB PORT NUMBER : %d  ",ptr_usb->port_num);
			printf("PORT POWER : %s  ",ptr_usb->port_power_state ? "ON":"OFF");
			printf("PORT MODE : %s  ",ptr_usb->port_config_state ? "CDP":"DCP");
			printf("ADB PORT NUM : %d-%d  ",ptr_usb->adb_num[0],ptr_usb->adb_num[1]);
			printf("SERIAL NUMBER : %s\r\n",ptr_usb->SERIAL);
		}
		ptr_usb++;
	}


}

void Check_file(const char *dirPath, int i)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirPath)) !=NULL)//맨처음 받은 dirpath에서 경로의 폴더를 열기위하여 없거나 열지 못했을경우 이 if문안으로 들어오지 못한다
	{
		while ((ent = readdir(dir)) != NULL)//ent에 폴더안의 있는 값들을 하나씩 읽어온다.
		{
			if (ent -> d_type ==DT_DIR)//ent 가 폴더일 경우 if문 안으로 
			{
				if(strcmp(ent->d_name,".") != 0 && strcmp(ent->d_name, "..") != 0)//현재 폴더와 상위폴더를 제외하고 하위 폴더를 찾기 위하여
				{
					char usb_port[300];//숫자-숫자의 정보를 저장하기 위하여 문자열로 정의
					char subDir[300]; //지금 들어와있는 폴더경로를 저장하기 위하여 문자열로 정의
					snprintf(subDir,sizeof(subDir),"%s/%s",dirPath,ent->d_name);//폴더의 안으로 들어가면서 그 폴더의 경로 저장
					int first_num,second_num;//숫자-숫자이므로 숫자들을 저장하기 위하여 int로 선언
					sscanf(ent->d_name,"%d-%d",&first_num,&second_num); //sscanf로 숫자-숫자의 값을 first_num과 second_num에 저장한다
					snprintf(usb_port,sizeof(usb_port),"%d-%d",first_num,second_num);//위에 구했던 값을 usb_port값에 저장해준다
					
					if(strcmp(ent->d_name,usb_port) == 0)//usb_port의 이름으로 되어있는 폴더를 찾았을경우 이 if문 안으로 들어오게 된다. 
					{
						//printf("find!!!!\r\n");
						//printf("USB PORT NUMBER %s\r\n",usb_port);
						//printf("firstnum : %d\r\n",first_num);
						//printf("secondnum : %d\r\n",second_num);
						snprintf(subDir,sizeof(subDir),"%s/%s/serial",dirPath,ent->d_name);//들어와진 경로의 뒤에 /serial을 붙여 subDir에 저장
						
						if(second_num == 2)//second_num이 2일 경우 1,3,5,7,9,11,13,15이므로 
						{				  //USB[]는 배열이므로 -1해서 계산
							USB[i*2].adb_num[0] = first_num;
							USB[i*2].adb_num[1] = second_num;
							search_serial(subDir,i*2);
						}
						else//second_num이 1일경우 2,4,6,8,10,12,14,16 이므로
						{
							USB[i*2+1].adb_num[0] = first_num;
							USB[i*2+1].adb_num[1] = second_num;
							search_serial(subDir, i*2+1);
						}
					}
					Check_file(subDir,i);//만약 숫자-숫자의 파일을 못찾았을경우 다른 폴더도 체크하기 위하여 재귀(다시) 함수를 불러 사용
				}
			}
			
		}
		closedir(dir);//열었던 폴더를 닫는다
	}
	else
	{
	//printf("aa\r\n");
	}
}

			
void search_serial (const char* data,int i)//위에서 계산된 i와 주소값을 받아 온다
{
	FILE *serialFile = fopen(data,"r");//주소값의 해당되는 파일을 연다
	if (serialFile != NULL)//그 파일이 비어있지 않으면
	{
		if (fgets(serial_number,sizeof(serial_number),serialFile) != NULL)//파일에 쓰여있는 시리얼 번호를 serial_number로 저장하여 사용
		{
			//sprintf(USB[i].SERIAL,serial_number);
        	snprintf(USB[i].SERIAL,sizeof(USB[i].SERIAL),"%s",serial_number);//serial_number로 저장 된값을 USB[i]에 맞게 저장
           	 //USb[i].SERIAL = serial_number;
			//ptr_usb->SERIAL == serial_number;
		}
		else //폴더는 존재하지만 그 파일이 비어있을경우 
			printf("NO serial number"); 
	}
	else //폴더가 열리지 않을경우,그 폴더가 존재하지 않을경우
		printf("no serial number file");
		
	fclose(serialFile);//열었던 폴더를 닫는다
}

