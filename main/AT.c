#include "AT.h"
#include "board.h"
#include <stdint.h>
#include <string.h>
#include "flash.h"

extern uint8_t AT_Rx_Buf[AT_RX_BUF_SIZE];
uint8_t Rx_Idex = 0;
#define AT_CMD_ADDRESS 0x08004000

uint8_t AnalyzeData(void)
{
    if(AT_Receive_Flag==1)
    {
        AT_Receive_Flag=0;
        //计算有效数据长度
        uint8_t len=0;
        for(uint8_t i=0;i<AT_RX_BUF_SIZE;i++)
        {
            if(AT_Rx_Buf[i]=='\0')
            {
                len=i;
                break;
            }
        }
        //将有效数据复制到databuf
        uint8_t databuf[len];
        memset(databuf, 0, sizeof(databuf));
        memcpy(databuf, AT_Rx_Buf, len);
        //解析数据
        if(databuf[0]=='A'&&databuf[1]=='T')
        {
           uart_printf("OK\r\n");
           if(databuf[2]=='+' && databuf[3]=='C' && databuf[4]=='M' && databuf[5]=='D')
            {
                uart_printf("recv AT+CMDS\r\n");
                flashProgram(AT_CMD_ADDRESS,databuf,len);
            }
        }
    }
    return 0;
}

