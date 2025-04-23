#ifndef AT_H
#define AT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define AT_RX_BUF_SIZE 128
#define MAX_AT_CMD_LENGTH 32
#define MAX_AT_CMDS 3 // 定义指令的数量

extern volatile uint8_t AT_Receive_Flag;

#ifdef __cplusplus
}
#endif

#endif // !_AT_H