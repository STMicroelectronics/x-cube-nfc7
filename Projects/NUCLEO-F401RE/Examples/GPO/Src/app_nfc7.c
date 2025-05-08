/**
  ******************************************************************************
  * File Name          :  app_nfc7.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-NFC7.2.0.0 instances.
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

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_nfc7.h"
#include "main.h"

/* Includes ------------------------------------------------------------------*/
#include "nfc07a1_nfctag.h"
#include <stdio.h>

/** @defgroup ST25_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
volatile uint8_t GPOActivated = 0;
ST25DVxxKC_FIELD_STATUS_E fieldpresence;

/* Private functions ---------------------------------------------------------*/
void MX_NFC7_GPO_Init(void);
void MX_NFC7_GPO_Process(void);
void BSP_GPO_Callback(void);

void MX_NFC7_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN NFC7_Library_Init_PreTreatment */

  /* USER CODE END NFC7_Library_Init_PreTreatment */

  /* Initialize the peripherals and the NFC7 components */

  MX_NFC7_GPO_Init();

  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN NFC7_Library_Init_PostTreatment */

  /* USER CODE END NFC7_Library_Init_PostTreatment */
}
/*
 * LM background task
 */
void MX_NFC7_Process(void)
{
  /* USER CODE BEGIN NFC7_Library_Process */

  /* USER CODE END NFC7_Library_Process */

  MX_NFC7_GPO_Process();

}

  /**
  * @brief  Initialize the NFC7 GPO Example
  * @retval None
  */
void MX_NFC7_GPO_Init(void)
{

 const ST25DVxxKC_PASSWD_t st25dvxxkc_i2c_password = {.MsbPasswd = 0, .LsbPasswd=0};
  /* Configuration of X-NUCLEO-NFC07A1                                          */
  /******************************************************************************/
  /* Init of the Leds on X-NUCLEO-NFC07A1 board */
  NFC07A1_LED_Init(GREEN_LED );
  NFC07A1_LED_Init(BLUE_LED );
  NFC07A1_LED_Init(YELLOW_LED );
  NFC07A1_LED_On( GREEN_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_On( BLUE_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_On( YELLOW_LED );
  HAL_Delay( 300 );

  /* Init UART for display message on console */
  BSP_COM_Init(COM1);

  printf("------------------------------------------");
  printf("\n\r***** Welcome to x-cube-nfc7 example *****");
  printf("\n\r------------------------------------------");

  /* Init ST25DVXXKC driver */
  while( NFC07A1_NFCTAG_Init(NFC07A1_NFCTAG_INSTANCE) != NFCTAG_OK );

  /* Set EXTI settings for GPO Interrupt */
  NFC07A1_GPO_Init();

    /* Present configuration password */
  NFC07A1_NFCTAG_PresentI2CPassword(NFC07A1_NFCTAG_INSTANCE, st25dvxxkc_i2c_password );

  /* Set GPO Configuration */
  NFC07A1_NFCTAG_ConfigIT(NFC07A1_NFCTAG_INSTANCE,ST25DVXXKC_GPO1_ENABLE_MASK | ST25DVXXKC_GPO1_FIELDCHANGE_MASK );

  printf("\n\r\n\rST25DVxxKC initialization succeeded!\n\r");

  /* Init done */
  NFC07A1_LED_Off( GREEN_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_Off( BLUE_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_Off( YELLOW_LED );
  HAL_Delay( 300 );

}

/**
  * @brief  Process of the NFC7 GPO application
  * @retval None
  */
void MX_NFC7_GPO_Process(void)
{

    if( GPOActivated == 1 )
    {
      /* Switch on the GREEN LED according to the field */
      NFC07A1_NFCTAG_GetRFField_Dyn(NFC07A1_NFCTAG_INSTANCE, &fieldpresence );
      if( fieldpresence == ST25DVXXKC_FIELD_ON )
      {
        NFC07A1_LED_On( GREEN_LED );
		printf("\n\rGPO activated, Field on detected.");
      }
      else
      {
        NFC07A1_LED_Off( GREEN_LED );
		printf("\n\rGPO activated, Field off detected.");
      }
      GPOActivated = 0;
    }
}

/**
  * @brief  NFC7 GPO callback
  * @retval None
  */
void BSP_GPO_Callback(void)
{
  /* Prevent unused argument(s) compilation warning */
  GPOActivated = 1;
  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */
}

#ifdef __cplusplus
}
#endif

