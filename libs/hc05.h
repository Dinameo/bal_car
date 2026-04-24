#ifndef __HC05_H
#define __HC05_H
#include "main.h"
#include "common.h"

#include "pidcb.h"




#define RX_BUF_SIZE 64
#define TX_BUF_SIZE 64
#define HC05_AT_BAUDRATE		38400
#define HC05_TIMEOUT			100

#define HC05_AT_TEST				"AT\r\n"
#define HC05_AT_RESET				"AT+RESET\r\n"
#define HC05_AT_GET_VER				"AT+VERSION?\r\n"
#define HC05_AT_RS_DEFAULT			"AT+ORGL\r\n"		// pin code: 1234, device name: H-C-2010-06-01, Baud 38400
#define HC05_AT_GET_ADDR			"AT+ADDR?\r\n"
#define HC05_AT_GET_NAME			"AT+NAME?\r\n"
#define HC05_AT_SET_NAME			"AT+NAME="
#define HC05_AT_GET_ROLE			"AT+ROLE?\r\n"
#define HC05_AT_GET_PIN				"AT+PSWD?\r\n"
#define HC05_AT_SET_PIN				"AT+PSWD="
#define HC05_AT_DISC				"AT+DISC\r\n"
#define HC05_AT_GET_UART			"AT+UART?\r\n"
#define HC05_AT_SET_UART			"AT+UART=" // <baude>,<stopbit>,<parity>\r\n


typedef enum
{
    HC05_MODE_NORMAL = 0,
    HC05_MODE_AT = 1
} HC05_Mode_t;

typedef enum
{
    HC05_DISCONNECTED = GPIO_PIN_RESET,
    HC05_CONNECTED = GPIO_PIN_SET
} HC05_State_t;


typedef struct
{
	volatile uint8_t tx_done;
	volatile uint8_t rx_done;
	uint8_t rx_dma_buffer[RX_BUF_SIZE];
	char tx_dma_buffer[TX_BUF_SIZE];
	volatile uint8_t connected_flag;


	Pin_t en;
	Pin_t state;
	UART_HandleTypeDef* huart;
	DMA_HandleTypeDef* dma;
} HC05_t;
void HC05_Init(HC05_t* hc05, UART_HandleTypeDef* huart, DMA_HandleTypeDef* dma, Pin_t en, Pin_t state);
void HC05_Start_Rx_DMA(HC05_t* hc05);
void HC05_Connect(HC05_t* hc05);
HC05_State_t HC05_Read_State(HC05_t* hc05);
void HC05_Tx_DMA(HC05_t* hc05, uint8_t len);
float String2Num(uint8_t* str, uint8_t start, uint8_t end);
void Float2String(float num, uint8_t* str, uint8_t prec);
void Int2String(int32_t num, uint8_t* str);
//PID_index_t Get_PID_Param(PID_Param_t* pid_param, uint8_t* str, uint8_t len);
//void Get_PID_Param(PID_Param_t* pid_param, uint8_t* str, uint8_t len);
void Get_PID_Param(PID_Param_t* pid_param_p, PID_Param_t* pid_param_y, uint8_t* str, uint8_t len);



#endif
