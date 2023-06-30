
/**
  ******************************************************************************
  * File Name          :  app_nfc7.c
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-NFC7.1.0.1 instances.
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
UART_HandleTypeDef huart;
char uartmsg[80];
uint8_t writedata = 0xAA;
uint8_t readdata = 0x0;
uint8_t cnt = 0;
uint32_t st25dvxxkcbmsize = 0;
uint32_t memindex = 0;
ST25DVxxKC_PASSWD_t passwd;
ST25DVxxKC_I2CSSO_STATUS_E i2csso;
ST25DVxxKC_MEM_SIZE_t st25dvxxkcmemsize;
uint32_t ret;
extern UART_HandleTypeDef hcom_uart[COMn];
/* Private functions ---------------------------------------------------------*/
void MX_NFC7_I2CProtection_Init(void);
void MX_NFC7_I2CProtection_Process(void);

HAL_StatusTypeDef UARTConsolePrint( char *puartmsg );
HAL_StatusTypeDef UARTConsoleScan( uint8_t uartchar );

void MX_NFC7_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN NFC7_Library_Init_PreTreatment */

  /* USER CODE END NFC7_Library_Init_PreTreatment */

  /* Initialize the peripherals and the NFC7 components */

  MX_NFC7_I2CProtection_Init();

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

  MX_NFC7_I2CProtection_Process();

}

/**
  * @brief  Initialize the I2CProtection  feature Example
  * @retval None
  */
void MX_NFC7_I2CProtection_Init(void)
{

  /* Init of the Leds on X-NUCLEO-NFC07A1 board */
  NFC07A1_LED_Init(GREEN_LED);
  NFC07A1_LED_Init(BLUE_LED);
  NFC07A1_LED_Init(YELLOW_LED);

  NFC07A1_LED_On( GREEN_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_On( BLUE_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_On( YELLOW_LED );
  HAL_Delay( 300 );

 /* Init ST25DVXXKC driver */
  while( NFC07A1_NFCTAG_Init(NFC07A1_NFCTAG_INSTANCE) != NFCTAG_OK );

  /* Init done */
  NFC07A1_LED_Off( GREEN_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_Off( BLUE_LED );
  HAL_Delay( 300 );
  NFC07A1_LED_Off( YELLOW_LED );
  HAL_Delay( 300 );

	/* Init UART for display message on console */
  BSP_COM_Init(COM1);

  UARTConsolePrint( "----------------------------------------" );
  UARTConsolePrint( "\n\r*****Welcome to x-cube-nfc7 example*****" );
  UARTConsolePrint( "\n\r----------------------------------------" );

	/* Reset Mailbox enable to allow write to EEPROM */
  NFC07A1_NFCTAG_ResetMBEN_Dyn(NFC07A1_NFCTAG_INSTANCE);

	/* You need to present password to change static configuration */
  NFC07A1_NFCTAG_ReadI2CSecuritySession_Dyn(NFC07A1_NFCTAG_INSTANCE, &i2csso);

  if( i2csso == ST25DVXXKC_SESSION_CLOSED )
	{
     /* if I2C session is closed, present password to open session */
     passwd.MsbPasswd = 0;
     passwd.LsbPasswd = 0;
     NFC07A1_NFCTAG_PresentI2CPassword(NFC07A1_NFCTAG_INSTANCE, passwd);
	}

   /* Get ST25DVXXKC EEPROM size */
   NFC07A1_NFCTAG_ReadMemSize(NFC07A1_NFCTAG_INSTANCE, &st25dvxxkcmemsize);

  /* st25dvxxkcmemsize is composed of Mem_Size (number of blocks) and BlockSize (size of each blocks in bytes) */
  st25dvxxkcbmsize = (st25dvxxkcmemsize.Mem_Size + 1) * (st25dvxxkcmemsize.BlockSize + 1);

  UARTConsolePrint( "\n\r\n\rThis example will create 4 zones in the ST25DVXXKC,\n\rset different protection for each zone and test read/write on it" );
  /* Set mapping configuration on ST25DVXXKC, create 4 identical zone
  Mapping example for a STDV64:
                 _______________
           @0000|               |
                |               |
                |     Zone 1    |
                |               |
           @2048|---------------|
                |               |
                |     Zone 2    |
                |               |
           @4096|---------------|
                |               |
                |     Zone 3    |
                |               |
           @6144|---------------|
                |               |
                |     Zone 4    |
                |               |
           @8192|_______________|
  */

   NFC07A1_NFCTAG_CreateUserZone(NFC07A1_NFCTAG_INSTANCE, (st25dvxxkcbmsize / 4), (st25dvxxkcbmsize / 4), (st25dvxxkcbmsize / 4), (st25dvxxkcbmsize / 4 ));

   UARTConsolePrint( "\n\rZone 1 is not protected" );
   /*Unprotect zone 1*/
    NFC07A1_NFCTAG_WriteI2CProtectZonex(NFC07A1_NFCTAG_INSTANCE, ST25DVXXKC_PROT_ZONE1,  ST25DVXXKC_NO_PROT );

	UARTConsolePrint( "\n\rZone 2 is write protected" );
   /*Protect zone 2 for i2c write without password */
	NFC07A1_NFCTAG_WriteI2CProtectZonex(NFC07A1_NFCTAG_INSTANCE, ST25DVXXKC_PROT_ZONE2,  ST25DVXXKC_WRITE_PROT );

	UARTConsolePrint( "\n\rZone 3 is read protected" );
   /* Protect zone 3 for i2c read without password */
	NFC07A1_NFCTAG_WriteI2CProtectZonex(NFC07A1_NFCTAG_INSTANCE, ST25DVXXKC_PROT_ZONE3,  ST25DVXXKC_READ_PROT );

   UARTConsolePrint( "\n\rZone 4 is read/write protected" );
   /* Protect zone 4 for i2c read and write without password */
   NFC07A1_NFCTAG_WriteI2CProtectZonex(NFC07A1_NFCTAG_INSTANCE, ST25DVXXKC_PROT_ZONE4,  ST25DVXXKC_READWRITE_PROT );

   UARTConsolePrint( "\n\r\n\rPress a key to continue..." );
   UARTConsoleScan( readdata );

   UARTConsolePrint( "\n\r\n\rSession is closed to see protections in operation" );
   /* Session is open, present wrong password to closed it */
   passwd.MsbPasswd = 123;
   passwd.LsbPasswd = 123;
   NFC07A1_NFCTAG_PresentI2CPassword(NFC07A1_NFCTAG_INSTANCE, passwd);

   for( cnt = 1; cnt < 5; cnt++ )
   {
    /* Compute memory index for each zone */
    memindex = (((st25dvxxkcbmsize / 4) + 1) * (cnt - 1));

    sprintf( uartmsg, "\n\r\n\rStart writing Zone %d", cnt );
    UARTConsolePrint( uartmsg );
    ret = NFC07A1_NFCTAG_WriteData(NFC07A1_NFCTAG_INSTANCE, &writedata, memindex, 1 );
    if( ret != NFCTAG_OK )
    {
      sprintf( uartmsg, "\n\rWrite Zone %d protected, need i2c password", cnt );
      UARTConsolePrint( uartmsg );
    }
    else
    {
      sprintf( uartmsg, "\n\rWrite Zone %d done", cnt );
      UARTConsolePrint( uartmsg );
    }

    sprintf( uartmsg, "\n\r\n\rStart reading Zone %d", cnt );
    UARTConsolePrint( uartmsg );
    ret = NFC07A1_NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, &readdata, memindex, 1 );
    if( readdata == 0xFF )
    {
      sprintf( uartmsg, "\n\rRead Zone %d protected, need i2c password", cnt );
      UARTConsolePrint( uartmsg );
    }
    else
    {
      sprintf( uartmsg, "\n\rRead Zone %d done", cnt );
      UARTConsolePrint( uartmsg );
    }
  }

  UARTConsolePrint( "\n\r\n\rPress a key to continue..." );
  UARTConsoleScan( readdata );

  UARTConsolePrint( "\n\r\n\rSession is open to see granted access" );
  /* Session is closed, present password to open it */
  passwd.MsbPasswd = 0;
  passwd.LsbPasswd = 0;
  NFC07A1_NFCTAG_PresentI2CPassword(NFC07A1_NFCTAG_INSTANCE, passwd);

  for( cnt = 1; cnt < 5; cnt++ )
  {
    /* Compute memory index for each zone */
    memindex = (((st25dvxxkcbmsize / 4) + 1) * (cnt - 1));

    sprintf( uartmsg, "\n\r\n\rStart writing Zone %d", cnt );
    UARTConsolePrint( uartmsg );
    ret = NFC07A1_NFCTAG_WriteData(NFC07A1_NFCTAG_INSTANCE, &writedata, memindex, 1 );
    if( ret != NFCTAG_OK )
    {
      sprintf( uartmsg, "\n\rWrite Zone %d protected, need i2c password", cnt );
      UARTConsolePrint( uartmsg );
    }
    else
    {
      sprintf( uartmsg, "\n\rWrite Zone %d done", cnt );
      UARTConsolePrint( uartmsg );
    }

    sprintf( uartmsg, "\n\r\n\rStart reading Zone %d", cnt );
    UARTConsolePrint( uartmsg );
    ret = NFC07A1_NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, &readdata, memindex, 1 );
    if( readdata == 0xFF )
    {
      sprintf( uartmsg, "\n\rRead Zone %d protected, need i2c password", cnt );
      UARTConsolePrint( uartmsg );
    }
    else
    {
      sprintf( uartmsg, "\n\rRead Zone %d done", cnt );
      UARTConsolePrint( uartmsg );
    }
  }

  UARTConsolePrint( "\n\r\n\r----------------------------------------" );
  UARTConsolePrint( "\n\r*****        end of example        *****" );
  UARTConsolePrint( "\n\r----------------------------------------\n\r" );
}

  /**
  * @brief  Process of the I2CProtection application
  * @retval None
  */
void MX_NFC7_I2CProtection_Process(void)
{

}

/**
 * @brief   This function sends data on the uart
 * @param   puartmsg:
 * @retval  HAL_StatusTypeDef
 */
HAL_StatusTypeDef UARTConsolePrint( char *puartmsg )
{
  return HAL_UART_Transmit( &hcom_uart[COM1], (uint8_t *)puartmsg, strlen( puartmsg ), 500);
}

/**
 * @brief   This function wait a data on the uart
 * @param   uartchar received character
 * @retval  HAL_StatusTypeDef
 */
HAL_StatusTypeDef UARTConsoleScan( uint8_t uartchar )
{
  while( HAL_UART_Receive( &hcom_uart[COM1], &uartchar, 1, 500) == HAL_TIMEOUT );

  return HAL_OK;
}

#ifdef __cplusplus
}
#endif

