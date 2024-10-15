#ifndef _AT_H
#define _AT_H

#ifdef __cplusplus
extern "C"
{
#endif

#define AT_RX_BUF_SIZE 128
#define MAX_AT_CMD_LENGTH 32
#define MAX_AT_CMDS 3 // 定义指令的数量

volatile uint8_t AT_Receive_Flag=0;

#ifdef __cplusplus
}
#endif

#endif // !_AT_H