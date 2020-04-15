#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <iostream> 
#include <wiringPi.h>
#include <iostream>

#define RELAY1 29     //查看第三部分 引脚图 gpio.29对应wiringPi编码为 29
#define RELAY2 28     //查看第三部分 引脚图 gpio.28对应wiringPi编码为 28
#define RELAY3 27
#define RELAY4 26
#define ON 1
#define OFF 0
#define SERVER_PORT 25000
#define BUFF_LEN 1024

void handle_udp_msg(int fd)
{
	setenv("WIRINGPI_GPIOMEM", "1", 1);
	wiringPiSetup();			//初始化wiringPi
    pinMode(RELAY1,OUTPUT);	//将RELAY管脚设置为OUTPUT
	pinMode(RELAY2,OUTPUT);
    pinMode(RELAY3,OUTPUT);	//将RELAY管脚设置为OUTPUT
	pinMode(RELAY4,OUTPUT);	
	//unsigned char s[8];
    unsigned char turn_light_control;
    unsigned char auto_light_control;
    unsigned char xx_control;
    unsigned char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息

    while(1)
    {
        memset(buf, 0, BUFF_LEN);
        len = sizeof(clent_addr);
        count = recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1)
        {
            printf("recieve data fail!\n");
            return;
            }

       if(count > 0)
       {
           turn_light_control = buf[0];
           auto_light_control = buf[1];
           xx_control = buf[2];  
           // printf("%d, %d, %d\n",turn_light_control,auto_light_control,xx_control);
           std::cout << "turn_light_control: "<< (int)turn_light_control << "   auto_light_control: "<<(int)auto_light_control << "   xx_control: "<< (int)xx_control <<std::endl;                    
            }
       /* 
        {
            for(int i = 0; i < 8; i++)
			{
				s[i] = buf[i];
				//printf("%02x ", s[i]);
				printf("\n逆变换---将8字节数组转换为double型数据:\n");  
				memcpy(&money_dst,s,sizeof(double)); 
    			 //显示 
   				printf("%f \n",money_dst);
			}
    */
		
        if (turn_light_control == 1)
        {
            digitalWrite(RELAY1,ON);
            digitalWrite(RELAY2,OFF);
            }
        else if (turn_light_control == 2)
        {
            digitalWrite(RELAY1,ON);
            digitalWrite(RELAY2,OFF);
            }
        else 
        {
            digitalWrite(RELAY2,OFF);
            digitalWrite(RELAY1,OFF);
            }
                
        if (auto_light_control == 1)
        {
            digitalWrite(RELAY3,ON);
            }
        else 
        {
            digitalWrite(RELAY3,OFF);
            }
                
                
        if (xx_control == 1)
        {
            digitalWrite(RELAY4,ON);
            }
        else 
        {
            digitalWrite(RELAY4,OFF);
            }
            
        usleep(10000);
                
    } 


}



/*
    server:
            socket-->bind-->recvfrom-->sendto-->close
*/

int main(int argc, char* argv[])
{
    int server_fd, ret;
    struct sockaddr_in ser_addr; 

    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
        return -1;
    }

    handle_udp_msg(server_fd);   //处理接收到的数据

    close(server_fd);
    return 0;
}
