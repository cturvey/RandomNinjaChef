//****************************************************************************
// STM32 USART6 to USART1 STM32F746G-Discovery - sourcer32@gmail.com
//  for Duinopeak GPS Shield
// Copyright (C) 2018 - All Rights Reserved
//****************************************************************************

#include "stm32746g_discovery.h" // implicitly stm32f7xx.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy()
#include <time.h>
#include <math.h>

int Verbose = 0;

static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

//****************************************************************************

#define USART_RS232 USART1  // PA9/PB7 VCP
#define USART_GPS USART6 // PC6/PC7 ARDUINO SHIELD
#define USART_GPS_IRQHandler USART6_IRQHandler

#ifdef USART_CR1_TXEIE_TXFNFIE // FIFO Support
#define USART_CR1_RXNEIE USART_CR1_RXNEIE_RXFNEIE
#define USART_CR1_TXEIE  USART_CR1_TXEIE_TXFNFIE
#define USART_ISR_RXNE   USART_ISR_RXNE_RXFNE
#define USART_ISR_TXE    USART_ISR_TXE_TXFNF
#endif

//****************************************************************************

void OutChar(char c)
{
  while((USART_RS232->ISR & USART_ISR_TXE) == 0); // Wait for Empty
  USART_RS232->TDR = c; // Send Char
}

//****************************************************************************

#define LINEMAX 200 // Maximal allowed/expected line length

volatile char line_buffer[LINEMAX + 1]; // Holding buffer with space for terminating NUL
volatile int line_valid = 0;

//****************************************************************************

void USART_GPS_IRQHandler(void) // Sync and Queue NMEA Sentences
{
  static char rx_buffer[LINEMAX];   // Local holding buffer to build line
  static int rx_index = 0;

  if (USART_GPS->ISR & USART_ISR_ORE) // Overrun Error
    USART_GPS->ICR = USART_ICR_ORECF;

  if (USART_GPS->ISR & USART_ISR_NE) // Noise Error
    USART_GPS->ICR = USART_ICR_NCF;

  if (USART_GPS->ISR & USART_ISR_FE) // Framing Error
    USART_GPS->ICR = USART_ICR_FECF;

  if (USART_GPS->ISR & USART_ISR_RXNE) // Received character?
  {
    char rx = (char)(USART_GPS->RDR & 0xFF);

    if ((rx == '\r') || (rx == '\n')) // Is this an end-of-line condition, either will suffice?
    {
      if (rx_index != 0) // Line has some content
      {
        memcpy((void *)line_buffer, rx_buffer, rx_index); // Copy to static line buffer from dynamic receive buffer
        line_buffer[rx_index] = 0; // Add terminating NUL
        line_valid = 1; // flag new line valid for processing

        rx_index = 0; // Reset content pointer
      }
    }
    else
    {
      if ((rx == '$') || (rx_index == LINEMAX)) // If resync or overflows pull back to start
        rx_index = 0;

      rx_buffer[rx_index++] = rx; // Copy to buffer and increment
    }
  }
}

//****************************************************************************

#define FIELD_MAX 20

void ProcessNMEALine(char *s)
{
  char *field[FIELD_MAX];
  int f;
  int i;
  char c;
  int x;

  if (Verbose)
  {
    puts("DecodeNMEA");
    printf("%s\n",s);
  }

  // Check the line synchronization

  if (s[0] != '$')
    return;

  // Verify the line checksum integrity

  c = 0; // checksum

  i = 1; // Xor bytes between $ and *, but not including those bytes

  while((s[i] != 0) && (s[i] != '*'))
    c ^= s[i++];

  if (s[i] == 0)
    return;

  sscanf(&s[i + 1],"%x",&x); // Checksum byte - Note sscanf needs this to be an int, rather than a single byte

  if (c != (char)(x & 0xFF)) // Leave if checksum fails
    return;

  // Parse out fields on , and *

  f = 0;

  while(1)
  {
    field[f++] = s;

    while((*s != 0) && (*s != ',') && (*s != '*') && (*s != 0x0D) && (*s != 0x0A))
      s++;

    if ((*s == 0) || (*s == '*') || (*s == 0x0D) || (*s == 0x0A) || (f == (FIELD_MAX - 1)))
    {
      *s = 0;
      field[f] = NULL;
      break;
    }

    *s++ = 0;
  }

  if (Verbose)
  {
    printf("Fields %02d\n",f);

    for(i=0; i<f; i++)
      printf("#%02d : %s\n",i,field[i]);
  }

  // Process a couple of NMEA sentences for illustration

  if (strncmp(field[0],"$G",2) == 0) // Allow for different Talker ID's ie $GP, $GN, $GL, etc
  {
    if ((strcmp(&field[0][3],"GLL") == 0) && (f > 6)) // Geographic Position, Latitude, Longitude and Time
    {
      double lat, lon;
      int lat_deg, lon_deg;
      double lat_min, lon_min;
      double fix_time;
      int fix_hour, fix_minute;
      double fix_second;
      char lat_hemi, lon_hemi, valid;

      // Field 1 Latitude DDMM.mmmmmm
      // Field 2 Lat Hemi N/S
      // Field 3 Longitude DDMMM.mmmmm
      // Field 4 Lon Hemi E/W
      // Field 5 UTC Time HHMMSS.SSS
      // Field 6 Fix A=Valid, V=Not Valid
      // Field 7 Mode A=Autonomous, D=Differential, E=Estimated, N=Not Valid [Optional] - Simulator Does not report this

      sscanf(field[1],"%lf",&lat);

      lat_hemi = field[2][0];

      sscanf(field[3],"%lf",&lon);

      lon_hemi = field[4][0];

      sscanf(field[5],"%lf",&fix_time);

      valid = field[6][0];

      if (valid == 'A')
      {
        // Extract Time-of-Fix

        fix_minute = (int)fix_time / 100;

        fix_second = fix_time - (fix_minute * 100);

        fix_hour = fix_minute / 100;

        fix_minute = fix_minute % 100;

        // Process Latitude DDMM.mmmmm

        lat_deg = (int)lat / 100; // Decompose NMEA form ASCII into DEGREES and MINUTES

        lat_min = lat - (lat_deg * 100);

        lat = (double)lat_deg + (lat_min / 60.0); // Computed Latitude in DECIMAL DEGREES

        if (lat_hemi == 'S')
        {
          lat_deg = -lat_deg;
          lat = -lat;
        }

        // Process Longitude DDDMM.mmmmm

        lon_deg = (int)lon / 100; // Decompose NMEA form ASCII into DEGREES and MINUTES

        lon_min = lon - (lon_deg * 100);

        lon = (double)lon_deg + (lon_min / 60.0); // Computed Longitude in DECIMAL DEGREES

        if (lon_hemi == 'W')
        {
          lon_deg = -lon_deg;
          lon = -lon;
        }

        printf("GLL: %4d %9.6lf %4d %9.6lf [%+14.10lf %+14.10lf] @ %02d:%02d:%06.3lf\n",
          lat_deg, lat_min, lon_deg, lon_min, lat, lon, fix_hour, fix_minute, fix_second );
      }
      else
        puts("Invalid Fix");
    }
    else if ((strcmp(&field[0][3],"GSA") == 0) && (f > 17)) // GPS DOP and Active Satellites
    {
      char mode, fix;
      double pdop, hdop, vdop;
      int i, sv;

      // Field 1 A=Automatic(3D/2D), M=Manual
      // Field 2 Fix 1=No Fix, 2=2D, 3=3D
      // Field 3 SV List#1
      // Field 14 SV List#12
      // Field 15 PDOP
      // Field 16 HDOP
      // Field 17 VDOP

      mode = field[1][0];
      fix = field[2][0];

      sscanf(field[15],"%lf",&pdop); // Position Dilution of precision (PDOP)
      sscanf(field[16],"%lf",&hdop); // Horizontal Dilution of precision (HDOP)
      sscanf(field[17],"%lf",&vdop); // Vertical Dilution of precision (VDOP)

      switch(mode)
      {
        case 'A' : puts("Mode : Automatic"); break;
        case 'M' : puts("Mode : Manual"); break;
        default : puts("Mode : Unknown");
      }

      switch(fix)
      {
        case '1' : puts("Fix : Not Available"); break;
        case '2' : puts("Fix : 2D"); break;
        case '3' : puts("Fix : 3D"); break;
        default : puts("Fix : Unknown");
      }

      printf("SV :");

      for(i=0; i<12; i++)
      {
        if (field[3+i][0])
        {
          sscanf(field[3+i],"%d",&sv);
          printf(" %3d", sv);
        }
      }

      putchar('\n');
      printf("PDOP : %5.2lf, HDOP : %5.2lf, VDOP : %5.2lf\n", pdop, hdop, vdop);
    }
    else  if ((strcmp(&field[0][3],"GGA") == 0) && (f > 14))
    {
      double lat, lon, alt, msl;
      int lat_deg, lon_deg;
      double lat_min, lon_min;
      double fix_time;
      int fix_hour, fix_minute;
      double fix_second;
      char lat_hemi, lon_hemi;
      int valid;
      double hdop;
      int sv;

      // Field  1 UTC Time HHMMSS.SSS
      // Field  2 Latitude
      // Field  3 Lat Hemi
      // Field  4 Longitude
      // Field  5 Lon Hemi
      // Field  6 Position Fix Indicator - 0=Fix Not available, 1=GPS SPS, 2=GPS SPS DIFF, 3=GPS PPS,
      //              4=RTK, 5=Float RTK, 6=Estimate/Dead Reckoning, 7=Manual, 8=Simulator
      // Field  7 Satellites Used
      // Field  8 HDOP
      // Field  9 MSL Altitude
      // Field 10 Units M
      // Field 11 Geoid Separation
      // Field 12 Units M
      // Field 13 Age of Differential (NULL when not used)
      // Field 14 Differential Reference Station ID

      sscanf(field[2],"%lf",&lat);

      lat_hemi = field[3][0];

      sscanf(field[4],"%lf",&lon);

      lon_hemi = field[5][0];

      sscanf(field[9],"%lf",&msl);

      sscanf(field[11],"%lf",&alt);

      sscanf(field[1],"%lf",&fix_time);

      if (sscanf(field[6],"%d",&valid) != 1)// 0=Fix Not available, 1=GPS SPS, 2=GPS SPS DIFF
        valid = 0;

      sscanf(field[7],"%d",&sv);    // Satellites used (or in view, depends on receiver)

      sscanf(field[8],"%lf",&hdop); // Horizontal Dilution of precision (HDOP)

      if (valid != 0)
      {
        lat_deg = (int)lat / 100;

        lat_min = lat - (lat_deg * 100);

        lat = (double)lat_deg + (lat_min / 60.0);

        if (lat_hemi == 'S')
          lat = -lat;


        lon_deg = (int)lon / 100;

        lon_min = lon - (lon_deg * 100);

        lon = (double)lon_deg + (lon_min / 60.0);

        if (lon_hemi == 'W')
          lon = -lon;

        alt += msl; // Convert to height above datum

        fix_minute = (int)fix_time / 100;

        fix_second = fix_time - (fix_minute * 100);

        fix_hour = fix_minute / 100;

        fix_minute = fix_minute % 100;

        printf("GGA: %+14.10lf %+14.10lf %8.2lf @ %02d:%02d:%06.3lf\n",
            lat, lon, alt,
            fix_hour, fix_minute, fix_second);
      }
    }
  }
}

//****************************************************************************

void USART1_Init(void)
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

  UartHandle.Init.BaudRate   = 115200;
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

void USART6_Init(void)
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
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USART6;

  UartHandle.Init.BaudRate   = 9600;
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

int main(void)
{
  SystemClock_Config();
  CPU_CACHE_Enable();

  USART1_Init(); // VCP
  USART6_Init(); // GPS

  puts("\nSTM32F746G-DISCO GPS NMEA Demo");

  USART_GPS->CR1 |= USART_CR1_RXNEIE; // Enable Interrupt

  while(1)
  {
    if (line_valid)
    {
      ProcessNMEALine((char *)line_buffer); // while technically volatile it will remain
      line_valid = 0;
    }
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

void hard_fault_handler_c(unsigned int * hardfault_args, unsigned int r4, unsigned int r5, unsigned int r6)
{
  printf ("\n[Hard Fault]\n"); // After Joseph Yiu

  printf ("r0 = %08X, r1 = %08X, r2 = %08X, r3 = %08X\n",
    hardfault_args[0], hardfault_args[1], hardfault_args[2], hardfault_args[3]);
  printf ("r4 = %08X, r5 = %08X, r6 = %08X, sp = %08X\n",
    r4, r5, r6, (unsigned int)&hardfault_args[8]);
  printf ("r12= %08X, lr = %08X, pc = %08X, psr= %08X\n",
    hardfault_args[4], hardfault_args[5], hardfault_args[6], hardfault_args[7]);

  printf ("bfar=%08X, cfsr=%08X, hfsr=%08X, dfsr=%08X, afsr=%08X\n",
    *((volatile unsigned int *)(0xE000ED38)),
    *((volatile unsigned int *)(0xE000ED28)),
    *((volatile unsigned int *)(0xE000ED2C)),
    *((volatile unsigned int *)(0xE000ED30)),
    *((volatile unsigned int *)(0xE000ED3C)) );

  while(1);
}

//****************************************************************************
// Hosting of stdio functionality through USART
//****************************************************************************

/* Implementation of putchar (also used by printf function to output data)    */
int SendChar(int ch)                    /* Write character to Serial Port     */
{
  ITM_SendChar(ch); // From core_cm4.c
  OutChar((char)ch);

  return(ch);
}

//****************************************************************************

#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) { return (SendChar(ch)); }

int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}

void _ttywrch(int ch) { SendChar(ch); }

void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}

//****************************************************************************

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  puts("ERROR!");
  while(1);
}

//****************************************************************************

#ifdef  USE_FULL_ASSERT
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
