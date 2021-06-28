/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1272 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
 /*******************************************************************************
  * @file    sx1272usi.c - Modified for I-LRWAN1-NUCLEO by sourcer32@gmail.com
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    01-June-2017
  * @brief   driver sx1272usi board
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "radio.h"
#include "sx1272.h"
#include "sx1272usi.h"


#define IRQ_HIGH_PRIORITY  2

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */


void SX1272SetXO(uint8_t state);

uint32_t SX1272GetWakeTime(void);

void SX1272IoIrqInit(DioIrqHandler **irqHandlers);

void SX1272_SetAntSw(uint8_t opMode);

uint8_t SX1272GetPaSelect(uint32_t channel);

void SX1272SetAntSwLowPower(bool status);

void SX1272SetRfTxPower(int8_t power);

void SX1272SetAntSw(uint8_t opMode);
/*!
 * \brief Controls the antena switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] opMode Current radio operating mode
 */
static LoRaBoardCallback_t BoardCallbacks = { SX1272SetXO,
                                              SX1272GetWakeTime,
                                              SX1272IoIrqInit,
                                              SX1272SetRfTxPower,
                                              SX1272SetAntSwLowPower,
                                              SX1272SetAntSw};

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1272IoInit,
    SX1272IoDeInit,
    SX1272Init,
    SX1272GetStatus,
    SX1272SetModem,
    SX1272SetChannel,
    SX1272IsChannelFree,
    SX1272Random,
    SX1272SetRxConfig,
    SX1272SetTxConfig,
    SX1272CheckRfFrequency,
    SX1272GetTimeOnAir,
    SX1272Send,
    SX1272SetSleep,
    SX1272SetStby,
    SX1272SetRx,
    SX1272StartCad,
    SX1272SetTxContinuousWave,
    SX1272ReadRssi,
    SX1272Write,
    SX1272Read,
    SX1272WriteBuffer,
    SX1272ReadBuffer,
    SX1272SetSyncWord,
    SX1272SetMaxPayloadLength,
    SX1272GetRadioWakeUpTime
};

uint32_t SX1272GetWakeTime(void)
{
  return  BOARD_WAKEUP_TIME;
}

void SX1272SetXO(uint8_t state)
{
}

void SX1272IoInit(void)
{
  GPIO_InitTypeDef initStruct = {0};

  SX1272BoardInit(&BoardCallbacks);

  initStruct.Mode = GPIO_MODE_IT_RISING;
  initStruct.Pull = GPIO_PULLDOWN;
  initStruct.Speed = GPIO_SPEED_HIGH;

  HW_GPIO_Init(RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct);
  HW_GPIO_Init(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct);
  HW_GPIO_Init(RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct);
  HW_GPIO_Init(RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct);

  /* Initialize I-NUCLEO-LRWAN1 Antenna IO */

  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;

  HW_GPIO_Init(RADIO_ANT_RX_SWITCH_PORT, RADIO_ANT_RX_SWITCH_PIN, &initStruct); // RXEN
  HW_GPIO_Init(RADIO_ANT_TX_SWITCH_PORT, RADIO_ANT_TX_SWITCH_PIN, &initStruct); // TXEN
}

void SX1272IoIrqInit(DioIrqHandler **irqHandlers)
{
  HW_GPIO_SetIrq(RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, IRQ_HIGH_PRIORITY, irqHandlers[0]);
  HW_GPIO_SetIrq(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, IRQ_HIGH_PRIORITY, irqHandlers[1]);
  HW_GPIO_SetIrq(RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, IRQ_HIGH_PRIORITY, irqHandlers[2]);
  HW_GPIO_SetIrq(RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, IRQ_HIGH_PRIORITY, irqHandlers[3]);
}

void SX1272IoDeInit(void)
{
  GPIO_InitTypeDef initStruct = {0};

  initStruct.Mode = GPIO_MODE_IT_RISING; //GPIO_MODE_ANALOG;
  initStruct.Pull = GPIO_PULLDOWN;

  HW_GPIO_Init(RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct);
  HW_GPIO_Init(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct);
  HW_GPIO_Init(RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct);
  HW_GPIO_Init(RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct);
}

void SX1272SetRfTxPower(int8_t power)
{
  uint8_t paConfig = 0;
  uint8_t paDac = 0;

  paConfig = SX1272Read(REG_PACONFIG);
  paDac = SX1272Read(REG_PADAC);

  paConfig = (paConfig & RF_PACONFIG_PASELECT_MASK) | SX1272GetPaSelect(SX1272.Settings.Channel);

  if ((paConfig & RF_PACONFIG_PASELECT_PABOOST) == RF_PACONFIG_PASELECT_PABOOST)
  {
    if (power > 17) // 5 .. 20
    {
      paDac = (paDac & RF_PADAC_20DBM_MASK) | RF_PADAC_20DBM_ON;

      if (power < 5)
        power = 5;
      else if (power > 20)
        power = 20;

      paConfig = (paConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK) | (uint8_t)((uint16_t)(power - 5) & 0x0F);
    }
    else // 2 .. 17
    {
      paDac = (paDac & RF_PADAC_20DBM_MASK) | RF_PADAC_20DBM_OFF;

      if (power < 2)
        power = 2;
      else if (power > 17)
        power = 17;

      paConfig = (paConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK) | (uint8_t)((uint16_t)(power - 2) & 0x0F);
    }
  }
  else // -1 .. 14
  {
    if (power < -1)
      power = -1;
    else if (power > 14)
      power = 14;

    paConfig = (paConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK) | (uint8_t)((uint16_t)(power + 1) & 0x0F);
  }

  SX1272Write(REG_PACONFIG, paConfig);
  SX1272Write(REG_PADAC, paDac);
}

uint8_t SX1272GetPaSelect(uint32_t channel)
{
#if 0
  return RF_PACONFIG_PASELECT_RFO;
#else
  return RF_PACONFIG_PASELECT_PABOOST;
#endif
}

void SX1272SetAntSwLowPower(bool status)
{
  // Ant Switch Controlled by SX1272 IC

  if (status)
  {
    HW_GPIO_Write(RADIO_ANT_RX_SWITCH_PORT, RADIO_ANT_RX_SWITCH_PIN, 0); // RXEN
    HW_GPIO_Write(RADIO_ANT_TX_SWITCH_PORT, RADIO_ANT_TX_SWITCH_PIN, 0); // TXEN
  }
}

void SX1272SetAntSw(uint8_t opMode)
{
  switch(opMode)
  {
    case RFLR_OPMODE_TRANSMITTER:
    {
      SX1272.RxTx = 1;

      /* Switch the antenna of I-NUCLEO-LRWAN1 in TX mode */

      HW_GPIO_Write(RADIO_ANT_RX_SWITCH_PORT, RADIO_ANT_RX_SWITCH_PIN, 0); // RXEN
      HW_GPIO_Write(RADIO_ANT_TX_SWITCH_PORT, RADIO_ANT_TX_SWITCH_PIN, 1); // TXEN

      PRINTF("ANT-TX %d\n", opMode);

      break;
    }

    case RFLR_OPMODE_STANDBY:
    case RFLR_OPMODE_SLEEP:
    {
      HW_GPIO_Write(RADIO_ANT_RX_SWITCH_PORT, RADIO_ANT_RX_SWITCH_PIN, 0); // RXEN
      HW_GPIO_Write(RADIO_ANT_TX_SWITCH_PORT, RADIO_ANT_TX_SWITCH_PIN, 0); // TXEN

      PRINTF("ANT-OFF %d\n", opMode);

      break;
    }

    case RFLR_OPMODE_RECEIVER:
    case RFLR_OPMODE_RECEIVER_SINGLE:
    case RFLR_OPMODE_CAD:
    default:
    {
      SX1272.RxTx = 0;

      /* Switch the antenna of I-NUCLEO-LRWAN1 in RX mode */

      HW_GPIO_Write(RADIO_ANT_TX_SWITCH_PORT, RADIO_ANT_TX_SWITCH_PIN, 0); // TXEN
      HW_GPIO_Write(RADIO_ANT_RX_SWITCH_PORT, RADIO_ANT_RX_SWITCH_PIN, 1); // RXEN

//      PRINTF("ANT-RX %d\n", opMode);

      break;
    }
  }
}

bool SX1272CheckRfFrequency(uint32_t frequency)
{
  // Implement check. Currently all frequencies are supportted
  return true;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

