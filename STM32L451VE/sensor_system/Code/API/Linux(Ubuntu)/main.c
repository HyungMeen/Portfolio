#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <stdlib.h>

void uart_init(int *serial_port);
void send_text(int argc, char *argv[]);

uint8_t cal_checksum(uint8_t data[],uint8_t size)
        {
                uint16_t checksum = 0;
                for (uint8_t i =0; i<size;i++)
                {
                        checksum +=data[i];
                }
                return checksum;
        }
uint16_t check_checksum(uint8_t data[])
        {
                uint16_t checksum = 0;
                for(int i=0;i<5;i++)
                {
                        if (i==3)
                        {
                                continue;
                        }
                        checksum+=data[i];
                }
                return checksum;
        }

uint8_t send_buf[5] = {0, };
uint8_t receive_buf[5] ={0, };
char rxbuffer[5] = {0, };
char buffer[1] = {0, };
int serial_port =0;

void uart_init(int *serial_port)
{
        *serial_port = open("/dev/ttyUSB0", O_RDWR);

        struct termios tty;
        memset(&tty, 0, sizeof(tty));

        cfsetospeed(&tty,B115200);
        cfsetispeed(&tty,B115200);
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_lflag = 0;
        tty.c_oflag = 0;
        tty.c_cc[VMIN] = 0;
        tty.c_cc[VTIME] = 5;
}

void send_text (int argc, char *argv[])
{
        send_buf[0] = 0x22;
        if(strcmp(argv[1], "start") == 0 || strcmp(argv[1], "START") == 0)
        {
                send_buf[0] = 0x15;
                write(serial_port, send_buf,sizeof(send_buf));
                exit(0);
        }
        else if (strcmp(argv[1], "stop") == 0 ||  strcmp(argv[1], "STOP") == 0)
        {
                send_buf[0] = 0x16;
                write(serial_port, send_buf,sizeof(send_buf));
        }
        else if (strcmp(argv[1], "set") == 0 || strcmp(argv[1], "SET") == 0)
        {
                if (strcmp(argv[2],"temp") == 0 || strcmp(argv[2],"TEMP") == 0)
                {
                        send_buf[1] = 0x74;
                        send_buf[2] = strtol(argv[3], NULL, 16);
                }
                else if (strcmp(argv[2],"rh") == 0 || strcmp(argv[2],"RH") == 0)
                {
                        send_buf[1] = 0x75;
                        send_buf[2] = strtol(argv[3], NULL, 16);
                }
                else
                {
                        printf("please correct command");
                }
        }
        else if (strcmp(argv[1],"get") == 0 || strcmp(argv[1],"GET") == 0)
        {
                if (strcmp(argv[2],"acc") == 0 || strcmp(argv[2],"ACC") == 0)
                {
                        send_buf[1] =0x70;
                }
                else if (strcmp(argv[2],"temp") == 0 || strcmp(argv[2],"TEMP") == 0)
                {
                        send_buf[1] = 0x71;
                }
                else if (strcmp(argv[2],"rh") == 0 || strcmp(argv[2],"RH") == 0)
                {
                        send_buf[1] = 0x72;
                }
                else
                {
                        printf("please correct command");
                }
        }
        else
        {
                printf("please correct command");
        }
        send_buf[4] = 0x44;
        send_buf[3] = cal_checksum(send_buf,5);
}


int main(int argc, char *argv[])
{
        memset(send_buf,0,sizeof(send_buf));
        uart_init(&serial_port);
        send_text(argc,argv);
        write(serial_port, send_buf,sizeof(send_buf));
        for (int i=0; i<5;i++)
        {
                read(serial_port,&rxbuffer[i],sizeof(rxbuffer));
        }
        if(rxbuffer[3] == check_checksum(rxbuffer))
        {
                if(rxbuffer[1] == 0x71 || rxbuffer[1] == 0x74)
                {
                        printf("Temp =  %hhu\r\n",rxbuffer[2]);
                }
                else if (rxbuffer[1] == 0x72 || rxbuffer[1] == 0x75)
                {
                        printf("RH = %hhu\r\n",rxbuffer[2]);
                }
                else if (rxbuffer[1] == 0x70)
                {
                        printf("acc = %hhu\r\n",rxbuffer[2]);
                }
        }

}


