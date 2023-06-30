/**
  ******************************************************************************
  * @file    nfc07a1.h
  * @author  SRA MMY Application Team
  * @brief   This file contains definitions for the x_nucleo_nfc07a1.c
  *          board specific functions.
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
#ifndef __X_NUCLEO_NFC07A1_H
#define __X_NUCLEO_NFC07A1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nfc07a1_conf.h"
 
#include "st25dvxxkc.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup X_NUCLEO_NFC07A1
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC07A1_Exported_Types
  * @{
  */
/**
 * @brief  NFC07A1 Led enumerator definition
 */
typedef enum 
{
  GREEN_LED = 0,
  BLUE_LED,
  YELLOW_LED
}NFC07A1_Led_E;

/**
 * @brief  NFC07A1 Ack Nack enumerator definition
 */
typedef enum 
{
  I2CANSW_ACK = 0,
  I2CANSW_NACK
}NFC07A1_I2CANSW_E;

/**
 * @brief  NFC07A1 Led structure definition
 */
typedef struct
{
  uint16_t          NFC07A1_LED_PIN;
  GPIO_TypeDef *    NFC07A1_LED_PIN_PORT;
}NFC07A1_Led_TypeDef;

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
#define NFC07A1_INIT_CLK_GREEN_LED()        NFC07A1_INIT_CLK_LED1_RFD( );
#define NFC07A1_INIT_CLK_BLUE_LED()         NFC07A1_INIT_CLK_LED2_RFD( );
#define NFC07A1_INIT_CLK_YELLOW_LED()       NFC07A1_INIT_CLK_LED3_RFD( );


/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup ST25DVxxKC_NUCLEO_Exported_Functions
  * @{
  */
int32_t NFC07A1_LED_Init(  NFC07A1_Led_E led );
int32_t NFC07A1_LED_DeInit( NFC07A1_Led_E led );
int32_t NFC07A1_LED_On( const NFC07A1_Led_E led );
int32_t NFC07A1_LED_Off( const NFC07A1_Led_E led );
int32_t NFC07A1_LED_Toggle( const NFC07A1_Led_E led );
int32_t NFC07A1_GPO_Init( void );
int32_t NFC07A1_GPO_DeInit( void );
int32_t NFC07A1_GPO_ReadPin( void );
void NFC07A1_GPO_IRQHandler(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
  }
#endif
#endif /* __X_NUCLEO_NFC07A1_H */


