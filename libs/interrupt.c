/*
 * interrupt.c
 *
 *  Created on: Feb 1, 2026
 *      Author: nhan
 */


#include "interrupt.h"
#include "hc05.h"
#include "conf.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		read_imu_flag = 1;
	}
	if(htim->Instance == TIM4)
	{
		read_speed_flag = 1;
	}

}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == hc05.huart)
    {
        if(hc05.rx_dma_buffer[Size - 1] == '\n' || hc05.rx_dma_buffer[Size - 1] == '\r')
        {
			hc05.rx_done = 1;
			hc05.rx_dma_buffer[Size - 1] = '\r';
		}
		HAL_UARTEx_ReceiveToIdle_DMA(hc05.huart, hc05.rx_dma_buffer, RX_BUF_SIZE);
		__HAL_DMA_DISABLE_IT(hc05.dma, DMA_IT_HT);
    }
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == hc05.huart)
    {
        hc05.tx_done = 1;
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == hc05.state.pin)
	{
		if(HAL_GPIO_ReadPin(hc05.state.port, hc05.state.pin) == (GPIO_PinState)HC05_CONNECTED)
		{
			hc05.connected_flag = 1;
		}
		else
		{
			hc05.connected_flag = 0;
		}
	}
}
