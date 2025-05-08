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
#include "common.h"
#include "ndef_config.h"
#include "ndef_nfcv_wrapper.h"
#include "ndef_message.h"
#include "ndef_types.h"
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
ndefContext NdefCtx;

/* Private functions ---------------------------------------------------------*/
void MX_NFC7_NDEF_URI_Init(void);
void MX_NFC7_NDEF_URI_Process(void);

void MX_NFC7_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN NFC7_Library_Init_PreTreatment */

  /* USER CODE END NFC7_Library_Init_PreTreatment */

  /* Initialize the peripherals and the NFC7 components */
  MX_NFC7_NDEF_URI_Init();

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

  MX_NFC7_NDEF_URI_Process();

}

void MX_NFC7_NDEF_URI_Init(void)
{
  uint8_t         rawMessageBuf[30];
  ndefStatus      err;
  ndefConstBuffer bufUri;
  ndefType        uri;
  ndefRecord      record;
  ndefMessage     message;
  ndefBuffer      bufRawMessage;
  static uint8_t  ndefURI[] = "st.com/st25-demo";

  /******************************************************************************/
  /* Configuration of X-NUCLEO-NFC07A1                                          */
  /******************************************************************************/
  /* Init of the Leds on X-NUCLEO-NFC07A1 board */
  NFC07A1_LED_Init(GREEN_LED);
  NFC07A1_LED_Init(BLUE_LED);
  NFC07A1_LED_Init(YELLOW_LED);

  NFC07A1_LED_On(GREEN_LED);
  HAL_Delay(300);
  NFC07A1_LED_On(BLUE_LED);
  HAL_Delay(300);
  NFC07A1_LED_On(YELLOW_LED);
  HAL_Delay(300);

  /* Init UART for display message on console */
  BSP_COM_Init(COM1);

  printf("------------------------------------------");
  printf("\n\r***** Welcome to x-cube-nfc7 example *****");
  printf("\n\r------------------------------------------");

  /* Init ST25DVxxKC driver */
  while (NFC07A1_NFCTAG_Init(NFC07A1_NFCTAG_INSTANCE) != NFCTAG_OK);
  printf("\n\r\n\rST25DVxxKC initialization succeeded!");
  /* Reset Mailbox enable to allow write to EEPROM */
  NFC07A1_NFCTAG_ResetMBEN_Dyn(NFC07A1_NFCTAG_INSTANCE);

  /* Check if no NDEF detected, init mem in Tag Type 5 */
  ndefT5TagContextInitialization(&NdefCtx);

  if (ndefT5TagNdefDetect(&NdefCtx) != ERR_NONE)
  {
    if (ndefT5TagFormat(&NdefCtx, NULL, 1) != ERR_NONE)
    {
      printf("CCFILE initialization failed!");
    }
    else
    {
      if (ndefT5TagNdefDetect(&NdefCtx) != ERR_NONE)
      {
        printf("NDEF formatting error, Tag cannot be initialized!");
      }
    }
  }

  /* Init done */
  NFC07A1_LED_Off(GREEN_LED);
  HAL_Delay(300);
  NFC07A1_LED_Off(BLUE_LED);
  HAL_Delay(300);
  NFC07A1_LED_Off(YELLOW_LED);
  HAL_Delay(300);

  /* Prepare URI NDEF record content */
  bufUri.buffer = ndefURI;
  bufUri.length = strlen((char *)ndefURI);

  err = ndefRtdUriInit(&uri, NDEF_URI_PREFIX_HTTP_WWW, &bufUri); /* Initialize URI type structure */

  /* Fill record with URI info */
  err |= ndefRtdUriToRecord(&uri, &record); /* Encode URI Record */

  err |= ndefMessageInit(&message);

  /* Added record to NDEF message */
  err |= ndefMessageAppend(&message, &record); /* Append URI to message */

  bufRawMessage.buffer = rawMessageBuf;
  bufRawMessage.length = sizeof(rawMessageBuf);
  err |= ndefMessageEncode(&message, &bufRawMessage); /* Encode the message to the raw buffer */

  /* Write NDEF message to tag */
  err |= ndefT5TagWriteRawMessage(&NdefCtx, bufRawMessage.buffer, bufRawMessage.length);

  if (err != ERR_NONE)
  {
    printf("\n\r\n\rWrite error, URL has not been written!");
  }
  else
  {
    printf("\n\r\n\rURI NDEF message written to memory : www.st.com/st25-demo.");
  }

  /* Set the LED3 on to indicate Programming done */
  NFC07A1_LED_On(YELLOW_LED);

  printf("\n\r\n\r------------------------------------------");
  printf("\n\r*****         end of example         *****");
  printf("\n\r------------------------------------------\n\r");
}

/**
  * @brief  Process of the NDEF_URI application
  * @retval None
  */
void MX_NFC7_NDEF_URI_Process(void)
{

}

#ifdef __cplusplus
}
#endif

