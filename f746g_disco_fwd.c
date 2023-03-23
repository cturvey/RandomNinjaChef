//****************************************************************************
// Simple USART Forwarding Example  sourcer32@gmail.com
//  STM32 USART6 to/from USART1 STM32F746G-Discovery
// Copyright (C) 2023 All Rights Reserved
//****************************************************************************

#include "stm32746g_discovery.h" // implicitly stm32f7xx.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy()
#include <time.h>
#include <math.h>

//****************************************************************************

static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

//****************************************************************************

#define BAUD_RATE 115200

#ifdef USART_CR1_TXEIE_TXFNFIE // FIFO Support
#define USART_CR1_RXNEIE USART_CR1_RXNEIE_RXFNEIE
#define USART_CR1_TXEIE  USART_CR1_TXEIE_TXFNFIE
#define USART_ISR_RXNE   USART_ISR_RXNE_RXFNE
#define USART_ISR_TXE    USART_ISR_TXE_TXFNF
#endif

//****************************************************************************

void USART1_Init(void) // PA9/PA10
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  UART_HandleTypeDef UartHandle = { 0 };

  __USART1_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /* UART RX/TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_7;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*## Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = NO parity
      - BaudRate = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USART1;

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
  // Clear any stalling errors

  if (UartInput->ISR & USART_ISR_ORE) // Overrun Error
    UartInput->ICR = USART_ICR_ORECF;

  if (UartInput->ISR & USART_ISR_NE) // Noise Error
    UartInput->ICR = USART_ICR_NCF;

  if (UartInput->ISR & USART_ISR_FE) // Framing Error
    UartInput->ICR = USART_ICR_FECF;

  if (UartInput->ISR & USART_ISR_RXNE) // Received character?
  {
    char data = (char)(UartInput->RDR & 0xFF); // Read it

    while((UartOutput->ISR & USART_ISR_TXE) != 0); // Kill a bit of time for TXE=1 ready

    UartOutput->TDR = data; // Send to destination
  }
}

//****************************************************************************

int main(void)
{
  SystemClock_Config();
  CPU_CACHE_Enable();

  USART1_Init(); // VCP
  USART6_Init(); // GPS

  while(1)
  {
    SimpleForward(USART1, USART6);
    SimpleForward(USART6, USART1);
  }
}

//****************************************************************************

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4; // 8/200MHz=50MHz

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if (ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if (ret != HAL_OK)
  {
    while(1) { ; }
  }
}

//******************************************************************************

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
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

