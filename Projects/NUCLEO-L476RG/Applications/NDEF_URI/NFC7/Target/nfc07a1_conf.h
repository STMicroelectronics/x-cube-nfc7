/**
  ******************************************************************************
  * @file    nfc07a1_conf.h
  * @author  SRA Application Team
  * @date    27-Apr-2023
  * @brief   This file contains definitions for the NFC7 components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NFC07A1_CONF_H__
#define __NFC07A1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_bus.h"
#include "stm32l4xx_nucleo_errno.h"

#include "stm32l4xx_hal_exti.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define NFC07A1_I2C_Init         BSP_I2C1_Init
#define NFC07A1_I2C_DeInit       BSP_I2C1_DeInit
#define NFC07A1_I2C_ReadReg16    BSP_I2C1_ReadReg16
#define NFC07A1_I2C_WriteReg16   BSP_I2C1_WriteReg16
#define NFC07A1_I2C_Recv         BSP_I2C1_Recv
#define NFC07A1_I2C_IsReady      BSP_I2C1_IsReady

//#define NFC07A1_GetTick          HAL_GetTick

#define NFC07A1_GPO_PIN_PORT GPIOA
#define NFC07A1_GPO_PIN GPIO_PIN_6
#define NFC07A1_LPD_PIN_PORT GPIOA
#define NFC07A1_LPD_PIN GPIO_PIN_8
#define NFC07A1_NFCTAG_GPO_EXTI_LINE EXTI_LINE_6
#define NFC07A1_LED1_PIN_PORT GPIOB
#define NFC07A1_LED1_PIN GPIO_PIN_4
#define NFC07A1_LED2_PIN_PORT GPIOB
#define NFC07A1_LED2_PIN GPIO_PIN_5
#define NFC07A1_LED3_PIN_PORT GPIOA
#define NFC07A1_LED3_PIN GPIO_PIN_10

/* Exported variables --------------------------------------------------------*/
#define NFC07A1_GPO_EXTI EXTI9_5_IRQn
extern EXTI_HandleTypeDef GPO_EXTI;
#define H_EXTI_6 GPO_EXTI

#define NFC07A1_NFCTAG_INSTANCE         (0)

#define NFC07A1_NFCTAG_GPO_PRIORITY     (0)

#ifdef __cplusplus
}
#endif

#endif /* __NFC07A1_CONF_H__*/

