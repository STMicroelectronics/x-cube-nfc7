
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
#include "stm32l0xx_nucleo.h"
#include <stdio.h>
#include <string.h>

/** @defgroup ST25_Nucleo
  * @{
  */

/** @defgroup Main
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Application defined push button, to detect long push */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
uint32_t Button_TimeCount;
/* Private functions ---------------------------------------------------------*/
void MX_NFC7_EH_Init(void);
void MX_NFC7_EH_Process(void);

void MX_NFC7_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN NFC7_Library_Init_PreTreatment */

  /* USER CODE END NFC7_Library_Init_PreTreatment */

  /* Initialize the peripherals and the NFC7 components */

  MX_NFC7_EH_Init();

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

  MX_NFC7_EH_Process();

}

  /**
  * @brief  Initialize the EnergyHarvesting Example
  * @retval None
  */

void MX_NFC7_EH_Init(void)
{

  ST25DVxxKC_EN_STATUS_E value_eh_dyn;
  ST25DVxxKC_EH_MODE_STATUS_E value_eh_mode;

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

  /* Init ST25DVxxKC driver */
  while( NFC07A1_NFCTAG_Init(NFC07A1_NFCTAG_INSTANCE) != NFCTAG_OK );

   /* Init User button to allow user to change EH configuration */
  BSP_PB_Init( BUTTON_KEY, BUTTON_MODE_GPIO );

  /* Init done */
  NFC07A1_LED_Off( GREEN_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_Off( BLUE_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_Off( YELLOW_LED );
  HAL_Delay( 300 );

  /* Check status of Energy Harvesting static configuration */
  NFC07A1_NFCTAG_ReadEHMode(NFC07A1_NFCTAG_INSTANCE, &value_eh_mode );
  if( value_eh_mode == ST25DVXXKC_EH_ACTIVE_AFTER_BOOT )
  {
    /* Energy Harvesting is set after each boot, LED 1 is on */
    NFC07A1_LED_On( GREEN_LED );
	printf("\n\r\n\rEnergy Harvesting is activated after boot.");
  }
  else
  {
    /* Energy Harvesting is reset after each boot, LED 1 is off */
    NFC07A1_LED_Off( GREEN_LED );
	printf("\n\r\n\rEnergy Harvesting is activated by user.");
  }

  /* Check status of Energy Harvesting static configuration */
  NFC07A1_NFCTAG_GetEHENMode_Dyn(NFC07A1_NFCTAG_INSTANCE, &value_eh_dyn );
  if( value_eh_dyn == ST25DVXXKC_DISABLE )
  {
    /* Energy Harvesting is disabled, LED 2 is on */
    NFC07A1_LED_Off( BLUE_LED );
	printf("\n\r\n\rEnergy Harvesting is currently disabled.");
  }
  else
  {
    /* Energy Harvesting is activated, LED 2 is on */
    NFC07A1_LED_On( BLUE_LED );
	printf("\n\r\n\rEnergy Harvesting is activated.");
  }

}

/**
  * @brief  Process of the NFC7 EH application
  * @retval None
  */

void MX_NFC7_EH_Process(void)
{
  ST25DVxxKC_PASSWD_t passwd;
  ST25DVxxKC_EN_STATUS_E value_eh_dyn;
  ST25DVxxKC_EH_MODE_STATUS_E value_eh_mode;
  ST25DVxxKC_I2CSSO_STATUS_E i2csso;
  uint32_t tick_start;
  uint32_t tick_end;

  /*Detect button press*/

  if(BSP_PB_GetState( BUTTON_KEY ) == BUTTON_PRESSED)
  {
    tick_start = HAL_GetTick();
    /* Debouncing */
    HAL_Delay(50);

    do
    {
      tick_end = HAL_GetTick();
    } while ((BSP_PB_GetState( BUTTON_KEY ) == BUTTON_PRESSED) && (tick_end - tick_start) <= 1000);

	if((tick_end - tick_start) > 1000)
    {
      /* Long Button press */
      /* Wait until the button is released */
      while ((BSP_PB_GetState( BUTTON_KEY ) == BUTTON_PRESSED));

      /* Debouncing */
      HAL_Delay(50);

      /* When user button is released after 1s, toggle Energy Harvesting static configuration */

      /* You need to present password to change static configuration */
      NFC07A1_NFCTAG_ReadI2CSecuritySession_Dyn(NFC07A1_NFCTAG_INSTANCE, &i2csso );
      if( i2csso == ST25DVXXKC_SESSION_CLOSED )
      {
        /* if I2C session is closed, present password to open session */
        passwd.MsbPasswd = 0;
        passwd.LsbPasswd = 0;
        NFC07A1_NFCTAG_PresentI2CPassword(NFC07A1_NFCTAG_INSTANCE, passwd );
      }

      /* Toggle static configuration */
      NFC07A1_NFCTAG_ReadEHMode(NFC07A1_NFCTAG_INSTANCE, &value_eh_mode );
      if( value_eh_mode == ST25DVXXKC_EH_ACTIVE_AFTER_BOOT )
      {
        /* Let the user activate energy harvesting */
        value_eh_mode = ST25DVXXKC_EH_ON_DEMAND;
        NFC07A1_NFCTAG_WriteEHMode(NFC07A1_NFCTAG_INSTANCE, value_eh_mode );
        NFC07A1_LED_Off( GREEN_LED );
		printf("\n\r\n\rEnergy Harvesting won't be activated after boot.");
      }
      else
      {
        /* Energy harvesting activated after Power On Reset */
        value_eh_mode = ST25DVXXKC_EH_ACTIVE_AFTER_BOOT;
        NFC07A1_NFCTAG_WriteEHMode(NFC07A1_NFCTAG_INSTANCE, value_eh_mode );
        NFC07A1_LED_On( GREEN_LED );
		printf("\n\r\n\rEnergy Harvesting will now be activated after boot.");
      }
    }
    else
    {
      /* Short Button press */

      /* Button already released */

      /* Debouncing */
      HAL_Delay(50);

      /* When user button is pressed, toggle Energy Harvesting dynamic configuration */
      NFC07A1_NFCTAG_GetEHENMode_Dyn(NFC07A1_NFCTAG_INSTANCE, &value_eh_dyn );
      if( value_eh_dyn == ST25DVXXKC_DISABLE )
      {
        /* Enable energy harvesting */
        NFC07A1_NFCTAG_SetEHENMode_Dyn(NFC07A1_NFCTAG_INSTANCE);
        NFC07A1_LED_On( BLUE_LED );
		printf("\n\r\n\rEnergy Harvesting is now activated.");
      }
      else
      {
        /* Disable energy harvesting */
        NFC07A1_NFCTAG_ResetEHENMode_Dyn(NFC07A1_NFCTAG_INSTANCE);
        NFC07A1_LED_Off( BLUE_LED );
		printf("\n\r\n\rEnergy Harvesting is now disabled.");
      }
    }
  }
}

#ifdef __cplusplus
}
#endif

