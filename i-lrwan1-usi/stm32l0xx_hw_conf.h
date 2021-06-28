/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: contains hardaware configuration Macros and Constants

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
 /******************************************************************************
  * @file    stm32l0xx_hw_conf.h - Modified for I-LRWAN1-NUCLEO by sourcer32@gmail.com
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    01-June-2017
  * @brief   contains hardaware configuration Macros and Constants
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONF_L0_H__
#define __HW_CONF_L0_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//#define RADIO_DIO_4
//#define RADIO_DIO_5

/* LORA I/O definition */

#ifdef USE_STM32L0XX_NUCLEO

#define RADIO_RESET_PORT                          GPIOA // A0
#define RADIO_RESET_PIN                           GPIO_PIN_0

#define RADIO_MOSI_PORT                           GPIOA // D11
#define RADIO_MOSI_PIN                            GPIO_PIN_7

#define RADIO_MISO_PORT                           GPIOA // D12
#define RADIO_MISO_PIN                            GPIO_PIN_6

#define RADIO_SCLK_PORT                           GPIOA // D13
#define RADIO_SCLK_PIN                            GPIO_PIN_5

#define RADIO_NSS_PORT                            GPIOB // D10
#define RADIO_NSS_PIN                             GPIO_PIN_6

#define RADIO_DIO_0_PORT                          GPIOA // D2
#define RADIO_DIO_0_PIN                           GPIO_PIN_10

#define RADIO_DIO_1_PORT                          GPIOB // D3
#define RADIO_DIO_1_PIN                           GPIO_PIN_3

#define RADIO_DIO_2_PORT                          GPIOB // D4
#define RADIO_DIO_2_PIN                           GPIO_PIN_5

#define RADIO_DIO_3_PORT                          GPIOB // D5
#define RADIO_DIO_3_PIN                           GPIO_PIN_4

#ifdef RADIO_DIO_4
#define RADIO_DIO_4_PORT                          GPIOA // D8
#define RADIO_DIO_4_PIN                           GPIO_PIN_9
#endif

#ifdef RADIO_DIO_5
#define RADIO_DIO_5_PORT                          GPIOC // D9
#define RADIO_DIO_5_PIN                           GPIO_PIN_7
#endif

#define RADIO_ANT_SWITCH_PORT                     GPIOC // A4 RXTX
#define RADIO_ANT_SWITCH_PIN                      GPIO_PIN_1

#define BAT_LEVEL_PORT                            GPIOA // A2 - ADC_IN4
#define BAT_LEVEL_PIN                             GPIO_PIN_4

/*  SPI MACRO redefinition */

#define SPIX                            SPI1
#define SPIX_CLK_ENABLE()               __HAL_RCC_SPI1_CLK_ENABLE()

#define SPIX_AF                         GPIO_AF0_SPI1

#define SPIX_FORCE_RESET()              __HAL_RCC_SPI1_FORCE_RESET()
#define SPIX_RELEASE_RESET()            __HAL_RCC_SPI1_RELEASE_RESET()

/* ADC MACRO redefinition */

#define ADC_READ_CHANNEL                ADC_CHANNEL_4
#define ADCCLK_ENABLE()                 __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCCLK_DISABLE()                __HAL_RCC_ADC1_CLK_DISABLE()


/* --------------------------- RTC HW definition -------------------------------- */

#define RTC_OUTPUT       DBG_RTC_OUTPUT

#define RTC_Alarm_IRQn              RTC_IRQn
/* --------------------------- USART HW definition -------------------------------*/


#define USARTX                           USART2
#define USARTX_CLK_ENABLE()              __USART2_CLK_ENABLE()
#define USARTX_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTX_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTX_FORCE_RESET()             __USART2_FORCE_RESET()
#define USARTX_RELEASE_RESET()           __USART2_RELEASE_RESET()


#define USARTX_TX_PIN                  GPIO_PIN_2
#define USARTX_TX_GPIO_PORT            GPIOA
#define USARTX_TX_AF                   GPIO_AF4_USART2
#define USARTX_RX_PIN                  GPIO_PIN_3
#define USARTX_RX_GPIO_PORT            GPIOA
#define USARTX_RX_AF                   GPIO_AF4_USART2

/* Definition for USARTx's NVIC */
#define USARTX_IRQn                      USART2_IRQn
#define USARTX_IRQHandler                USART2_IRQHandler

#endif // USE_STM32L0XX_NUCLEO

#ifdef USE_I_NUCLEO_LRWAN1

/*  Pin Level Analysis by sourcer32@gmail.com

USI WM-SG-SM-42
 1  GND
 2  PA8/RCC_MCO
 3  PB10/LPUART_TX
 4  PB2/LPTIM1_OUT
 5  PA7/ADC_IN7
 6  PA1/COMP1_INP ??
 7  VREF+
 8  GND
 9  VDDA
10  PB5/LPTIM1_IN1
11  PA0/SYS_WKUP1
12  GND
13  GND
14  GND
15  GND
16  GND
17  GND
18  GND
19  GND
20  MICRO_RST_N
21  PA9/RF_RST (SX1272)
22  PA2/DIO0   (SX1272)
23  PA3/DIO1   (SX1272)
24  PA5/DIO2   (SX1272)
25  PA6/DIO3   (SX1272)
26  PA10/DIO4  (SX1272) ??TYPO NOT PA1
27  DIO5       (SX1272) ??
28  BOOT0
29  GND
30  VDD_RFS
31  VDD_RFS
32  GND
33  VDD_3V3
34  VDD_3V3
35  GND
36  PB3/SPI1_SCK  (SX1272)
37  PB4/SPI1_MISO (SX1272)
38  PA12/SPI_MOSI (SX1272)
39  PA15/SPI1_NSS (SX1272)
40  GND
41  GND
42  GND
43  PB8/RF_SW_CTRL2
44  PA4/RF_SW_CTRL1
45  GND
46  GND
47  RF_OUT (TEST, HAS U.FL)
48  GND
49  PB7/I2C1_SDA
50  PB6/I2C1_SCL
51  PA13/SWDIO
52  PA14/SWCLK
53  PB1/ADC_IN9
54  PA11/COMP1_OUT
55  PB11/LPUART_RX
56  PB0/ADC_IN8
57  GND
..
81  GND



STM32L052T8Y6 (WLCSP 36)
 1  A1 PA13 (SWDIO)
 2  A2 PA15 (SPI1_NSS)  SX1272
 3  A3 PB4  (SPI_MISO)  SX1272
 4  A4 PB7  (I2C1_SDA)
 5  A5 VDD
 6  A6 PC14 (OSC32_IN)
 7  B1 PA12 (SPI1_MOSI) SX1272
 8  B2 PA14 (SWCLK)
 9  B3 PB3  (SPI1_SCK)  SX1272
10  B4 PB6  (I2C1_SCL)
11  B5 PB8  (RF_SW_CTRL2) TXEN
12  B6 PC15 (OSC32-OUT)
13  C1 PA10 DIO4        SX1272
14  C2 PA11 (COMP1_OUT)
15  C3 PB1  (ADC_IN9)
16  C4 PB5  (LPTIM1_IN1)
17  C5 BOOT0
18  C6 NRST
19  D1 PA9  RF_RST      SX1272
20  D2 PB11 (LPUART_RX)
21  D3 PB0  (ADC_IN8)
22  D4 PA0  (SYS_WKUP1)
23  D5 VDDA
24  D6 VSS
25  E1 PA8  (MCO)
26  E2 PB10 (LPUART_TX)
27  E3 PA6  DIO3        SX1272
28  E4 PA4  (RF_SW_CTRL1) RXEN
29  E5 PA2  DIO0        SX1272
30  E6 VREF+
31  F1 VDD
32  F2 PB2  (LPTIM1_OUT)
33  F3 PA7  (ADC_IN7)
34  F4 PA5  DIO2        SX1272
35  F5 PA3  DIO1        SX1272
36  F6 PA1  (COMP1_IN)

*/


#define RADIO_RESET_PORT                          GPIOA  // PA9
#define RADIO_RESET_PIN                           GPIO_PIN_9

#define RADIO_MOSI_PORT                           GPIOA // PA12
#define RADIO_MOSI_PIN                            GPIO_PIN_12

#define RADIO_MISO_PORT                           GPIOB // PB4
#define RADIO_MISO_PIN                            GPIO_PIN_4

#define RADIO_SCLK_PORT                           GPIOB // PB3
#define RADIO_SCLK_PIN                            GPIO_PIN_3

#define RADIO_NSS_PORT                            GPIOA // PA15
#define RADIO_NSS_PIN                             GPIO_PIN_15

#define RADIO_DIO_0_PORT                          GPIOA // PA2
#define RADIO_DIO_0_PIN                           GPIO_PIN_2

#define RADIO_DIO_1_PORT                          GPIOA // PA3
#define RADIO_DIO_1_PIN                           GPIO_PIN_3

#define RADIO_DIO_2_PORT                          GPIOA // PA5
#define RADIO_DIO_2_PIN                           GPIO_PIN_5

#define RADIO_DIO_3_PORT                          GPIOA // PA6
#define RADIO_DIO_3_PIN                           GPIO_PIN_6

#ifdef RADIO_DIO_4
#define RADIO_DIO_4_PORT                          GPIOA // PA10
#define RADIO_DIO_4_PIN                           GPIO_PIN_10
#endif

#ifdef RADIO_DIO_5
#define RADIO_DIO_5_PORT                          GPIOC //??
#define RADIO_DIO_5_PIN                           GPIO_PIN_7
#endif

#define RADIO_ANT_RX_SWITCH_PORT                  GPIOA // PA4 RXEN RF_SW_CTRL1
#define RADIO_ANT_RX_SWITCH_PIN                   GPIO_PIN_4

#define RADIO_ANT_TX_SWITCH_PORT                  GPIOB // PB8 TXEN RF_SW_CTRL2
#define RADIO_ANT_TX_SWITCH_PIN                   GPIO_PIN_8

/*  SPI MACRO redefinition */

#define SPIX                             SPI1
#define SPIX_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()

#define SPIX_AF                          GPIO_AF0_SPI1

#define SPIX_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIX_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()


/* --------------------------- RTC HW definition -------------------------------- */

#define RTC_OUTPUT       DBG_RTC_OUTPUT

#define RTC_Alarm_IRQn              RTC_IRQn
/* --------------------------- USART HW definition -------------------------------*/


#define USARTX                           LPUART1
#define USARTX_CLK_ENABLE()              __LPUART1_CLK_ENABLE()
#define USARTX_RX_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define USARTX_TX_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()

#define USARTX_FORCE_RESET()             __LPUART1_FORCE_RESET()
#define USARTX_RELEASE_RESET()           __LPUART1_RELEASE_RESET()


#define USARTX_TX_PIN                  GPIO_PIN_10 // PB10 LPUART1_TX
#define USARTX_TX_GPIO_PORT            GPIOB
#define USARTX_TX_AF                   GPIO_AF4_LPUART1
#define USARTX_RX_PIN                  GPIO_PIN_11 // PB11 LPUART1_RX
#define USARTX_RX_GPIO_PORT            GPIOB
#define USARTX_RX_AF                   GPIO_AF4_LPUART1

/* Definition for USARTx's NVIC */
#define USARTX_IRQn                      LPUART1_IRQn
#define USARTX_IRQHandler                LPUART1_IRQHandler

#endif // USE_I_NUCLEO_LRWAN1

#define LED_Toggle( x )
#define LED_On( x )
#define LED_Off( x )

#ifdef __cplusplus
}
#endif

#endif /* __HW_CONF_L0_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

