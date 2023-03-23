//****************************************************************************
// Simple USART Forwarding Example  sourcer32@gmail.com
//  STM32 USART2 to/from USART6 STM32F411
// Copyright (C) 2023 All Rights Reserved
//****************************************************************************

#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy()
#include <time.h>
#include <math.h>

//****************************************************************************

static void SystemClock_Config(void);
static void Error_Handler(void);

//****************************************************************************

#define BAUD_RATE 115200

//****************************************************************************

void USART2_Init(void) // PA2/PA3
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  UART_HandleTypeDef UartHandle = { 0 };

  __USART2_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

  /* UART RX/TX GPIO pin configuration */
	
  GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_2;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*## Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = NO parity
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USART2;

  UartHandle.Init.BaudRate   = BAUD_RATE;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;

  UartHandle.Init.OverSampling   = UART_OVERSAMPLING_16;
#ifdef UART_ONE_BIT_SAMPLE_DISABLE
  UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
#endif

  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  } // sourcer32@gmail.com
}

//****************************************************************************

void USART6_Init(void) // PC6/PC7
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  UART_HandleTypeDef UartHandle = { 0 };

  __USART6_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();

  /* UART RX/TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*## Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = NO parity
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USART6;

  UartHandle.Init.BaudRate   = BAUD_RATE;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;

  UartHandle.Init.OverSampling   = UART_OVERSAMPLING_16;
#ifdef UART_ONE_BIT_SAMPLE_DISABLE
  UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
#endif

  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  HAL_NVIC_SetPriority(USART6_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART6_IRQn);
}

//****************************************************************************

// Assumption that data rates are equally matched.
//  Could use a FIFO / Ring Buffer to provide more elasticity

void SimpleForward(USART_TypeDef *UartInput, USART_TypeDef *UartOutput)
{
	uint16_t data;

  // Clear any stalling errors

  if (UartInput->SR & USART_SR_ORE) // Overrun Error
    data = UartInput->DR; // clear with data read

  if (UartInput->SR & USART_SR_NE) // Noise Error
    data = UartInput->DR; // clear with data read

  if (UartInput->SR & USART_SR_FE) // Framing Error
    data = UartInput->DR; // clear with data read

  if (UartInput->SR & USART_SR_RXNE) // Received character?
  {
    data = (UartInput->DR & 0xFF); // Read it

    while((UartOutput->SR & USART_SR_TXE) != 0); // Kill a bit of time for TXE=1 ready

    UartOutput->DR = data; // Send to destination
  }
}

//****************************************************************************

int main(void)
{
  SystemClock_Config();

  USART2_Init();
  USART6_Init();

  while(1)
  {
    SimpleForward(USART2, USART6);
    SimpleForward(USART6, USART2);
  }
}

//****************************************************************************

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 400
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

//****************************************************************************

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1);
}

//****************************************************************************

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

//****************************************************************************

