#include "main.h"
#include "hc05.h"

#include "pidcb.h"
#include <stdlib.h>






void HC05_Init(HC05_t* hc05, UART_HandleTypeDef* huart, DMA_HandleTypeDef* dma, Pin_t en, Pin_t state)
{
	hc05->en = en;
	hc05->state = state;
	hc05->huart = huart;
	hc05->dma = dma;
	hc05->rx_done = 0;
	hc05->tx_done = 1;
	hc05->tx_dma_buffer[0] = 0;
	hc05->rx_dma_buffer[0] = 0;
	hc05->connected_flag = 0;

	// Data mode
	HAL_GPIO_WritePin(hc05->en.port, hc05->en.pin, GPIO_PIN_RESET);

}
void HC05_Start_Rx_DMA(HC05_t* hc05)
{
	HAL_UARTEx_ReceiveToIdle_DMA(hc05->huart, hc05->rx_dma_buffer, RX_BUF_SIZE);
	__HAL_DMA_DISABLE_IT(hc05->dma, DMA_IT_HT);

}
HC05_State_t HC05_Read_State(HC05_t* hc05)
{
	if(HAL_GPIO_ReadPin(hc05->state.port, hc05->state.pin) == (GPIO_PinState) HC05_DISCONNECTED)
		return HC05_DISCONNECTED;
	return HC05_CONNECTED;
}
void HC05_Connect(HC05_t* hc05)
{
	while(HC05_Read_State(hc05) != HC05_CONNECTED);
	hc05->connected_flag = 1;
}

//HAL_StatusTypeDef HC05_Init_Baudrate(HC05_t* hc05, uint32_t baudrate)
//{
//	hc05->huart->Init.BaudRate = baudrate;
//	return HAL_UART_Init(hc05->huart);
//}
//void HC05_Change_Mode(HC05_t* hc05, HC05_Mode_t mode)
//{
//	HC05_Init_Baudrate(HC05_AT_BAUDRATE);
//	if(mode == HC05_MODE_AT)
//		HAL_GPIO_WritePin(hc05->en.port, hc05->en.pin, GPIO_PIN_SET);
//	else if(mode == HC05_MODE_NORMAL)
//		HAL_GPIO_WritePin(hc05->en.port, hc05->en.pin, GPIO_PIN_RESET);
//}

//void HC05_Put(HC05_t* hc05, uint8_t* data)
//{
//	uint8_t len = 0;
//	while(data[len++] !='\0');
//	HAL_UART_Transmit(hc05->huart, data, len-1, HC05_TIMEOUT);
//}
//void HC05_Take(uint8_t* buffer, uint8_t len)
//{
////	HAL_UART_Receive(hc05.huart, buffer, len, HC05_TIMEOUT);
//	HAL_UART_Receive_IT(hc05.huart, buffer, 1);
//}
//void HC05_Put_Command(HC05_t* hc05, uint8_t* cmnd, uint8_t* resp, uint8_t len)
//{
//	HC05_Put(hc05, cmnd);
//}
//void HC05_Get_Name(HC05_t* hc05, uint8_t* respone, uint8_t len)
//{
//	HC05_Put_Command(hc05, (uint8_t*) HC05_AT_GET_NAME, respone, len);
//}
//void HC05_Get_Pin(HC05_t* hc05, uint8_t* respone, uint8_t len)
//{
//	HC05_Put_Command(hc05, (uint8_t*) HC05_AT_GET_PIN, respone, len);
//}
//void HC05_Creat_AT_Command(uint8_t* res, uint8_t* at_cmnd, uint8_t* str)
//{
//
//	while(*at_cmnd != '\0')
//	{
//		*res = *at_cmnd;
//		res++;
//		at_cmnd++;
//	}
//	while(*str != '\0')
//	{
//		*res = *str;
//		res++;
//		str++;
//	}
//	*res = '\r';
//	res++;
//	*res = '\n';
//	res++;
//	*res = '\0';
//}
//void HC05_Set_Name(HC05_t* hc05, uint8_t* name, uint8_t* respone, uint8_t len)
//{
//	uint8_t str[32] = {'\0'};
//	HC05_Creat_AT_Command(str, (uint8_t*) HC05_AT_SET_NAME, name);
//	HC05_Put_Command(hc05, str, respone, len);
//}
//void HC05_Set_Pin(HC05_t* hc05, uint8_t* pin, uint8_t* respone, uint8_t len)
//{
//	uint8_t str[32] = {'\0'};
//	HC05_Creat_AT_Command(str, (uint8_t*) HC05_AT_SET_PIN, pin);
//	HC05_Put_Command(hc05, str, respone, len);
//}
//void HC05_Set_Uart(HC05_t* hc05, uint8_t* uart, uint8_t* respone, uint8_t len)
//{
//	uint8_t str[32] = {'\0'};
//	HC05_Creat_AT_Command(str, (uint8_t*) HC05_AT_SET_UART, uart);
//	HC05_Put_Command(hc05, str, respone, len);
//}
//void Int2String(int32_t num, uint8_t* str)
//{
//	uint8_t len = 0;
//	int8_t i = 0;
//	int8_t j = 0;
//	if(num < 0)
//	{
//		str[len++] = '-';
//		i++;
//		num = -num;
//	}
//	do
//	{
//		str[len++] = num%10 + '0';
//		num /= 10;
//	}
//	while(num != 0);
//
//	j = len - 1;
//	while(i < j)
//	{
//		uint8_t temp = str[j];
//		str[j] = str[i];
//		str[i] = temp;
//		i++;
//		j--;
//	}
//	str[len] = '\0';
//}
void HC05_Tx_DMA(HC05_t* hc05, uint8_t len)
{
	hc05->tx_done = 0;
	HAL_UART_Transmit_DMA(hc05->huart, (uint8_t*)hc05->tx_dma_buffer, len);
}
float String2Num(uint8_t* str, uint8_t start, uint8_t end)
{
//	int32_t int_part = 0;
//	int32_t frac_part = 0;
//	uint8_t is_neg = 0;
//	uint8_t is_frac = 0;
//	int32_t frac_div = 1;


//	for(int8_t p = start; p <= end; p++)
//	{
//		if(str[p] == '.')
//			is_frac = 1;
//		else if(str[p] == '-')
//			is_neg = 1;
//		else
//		{
//			if(!is_frac)
//			{
//				int_part *= 10;
//				int_part += str[p] - '0';
//			}
//			else
//			{
//
//				frac_part = frac_part * 10 + (str[p] - '0');
//				frac_div *= 10;
//			}
//		}
//
//	}
//	float num = int_part + (float)frac_part / frac_div;
//	if(is_neg) num = -num;

	int8_t str_sub[50] = {0};
	for(int8_t i = start; i <= end; i++)
	{
		str_sub[i-start] = str[i];
	}
	float num = atof((char*)str_sub);


	return num;

}
//PID_index_t Get_PID_Param(PID_Param_t* pid_param, uint8_t* str, uint8_t len)
//{
//    uint8_t start = 0;
//    uint8_t stt = 0;
//    PID_index_t pid_idx;
//
//    for(uint8_t i = 0; i < len; i++)
//    {
//        if(str[i] == ',' || str[i] == '\0' || str[i] == '\r' || str[i] == '\n')
//        {
//        	if(stt == 0)
//        		pid_idx = (uint8_t)String2Num(str, start, i - 1);
//        	else if(stt == 1)
//        		pid_param->setpoint = String2Num(str, start, i - 1);
//        	else if(stt == 2)
//                pid_param->kp = String2Num(str, start, i - 1);
//            else if(stt == 3)
//                pid_param->ki = String2Num(str, start, i - 1);
//            else if(stt == 4)
//                pid_param->kd = String2Num(str, start, i - 1);
//
//            start = i + 1;
//            stt++;
//
//            if(stt >= 5) return pid_idx;
//        }
//    }
//    return pid_idx;
//
//}
void Get_PID_Param(PID_Param_t* pid_param_p, PID_Param_t* pid_param_y, uint8_t* str, uint8_t len)
{
    uint8_t start = 0;
    uint8_t stt = 0;

    for(uint8_t i = 0; i < len; i++)
    {
        if(str[i] == ',' || str[i] == '\0' || str[i] == '\r' || str[i] == '\n')
        {
//        	if(stt == 0)
//        		pid_idx = (uint8_t)String2Num(str, start, i - 1);
        	if(stt == 0)
        		pid_param_p->setpoint = String2Num(str, start, i - 1);
        	else if(stt == 1)
                pid_param_p->kp = String2Num(str, start, i - 1);
            else if(stt == 2)
                pid_param_p->ki = String2Num(str, start, i - 1);
            else if(stt == 3)
                pid_param_p->kd = String2Num(str, start, i - 1);
            else if(stt == 4)
				pid_param_y->setpoint = String2Num(str, start, i - 1);
			else if(stt == 5)
				pid_param_y->kp = String2Num(str, start, i - 1);
			else if(stt == 6)
				pid_param_y->ki = String2Num(str, start, i - 1);
			else if(stt == 7)
				pid_param_y->kd = String2Num(str, start, i - 1);

            start = i + 1;
            stt++;

            if(stt >= 8) return;
        }
    }
//    return pid_idx;

}
void Int2String(int32_t num, uint8_t* str)
{
	uint8_t len = 0;
	int8_t i = 0;
	int8_t j = 0;
	if(num < 0)
	{
		str[len++] = '-';
		i++;
		num = -num;
	}
	do
	{
		str[len++] = num%10 + '0';
		num /= 10;
	}
	while(num != 0);

	j = len - 1;
	while(i < j)
	{
		uint8_t temp = str[j];
		str[j] = str[i];
		str[i] = temp;
		i++;
		j--;
	}
	str[len] = '\0';
}
void Float2String(float num, uint8_t* str, uint8_t prec)
{
	if(num < 0)
	{
		*str = '-';
		str++;
		num = -num;
	}
	int32_t frac_multiplier = 1;
	for(int8_t i = 0; i < prec; i++)
		frac_multiplier *= 10;
	num = (num + 0.5f/frac_multiplier);

	uint32_t int_part = (uint32_t) num;
	Int2String(int_part, str);
	if (prec == 0) return;

	while(*str != '\0') str++;
	*str = '.';
	str++;
	int32_t frac_part = (num - int_part)*frac_multiplier;
	for(int8_t i = 0; i < prec; i++)
	{
		frac_multiplier /= 10;
		uint8_t digit = frac_part / frac_multiplier;
		*str = digit + '0';
		str++;
		frac_part %= frac_multiplier;
	}
	*str = '\0';
}
