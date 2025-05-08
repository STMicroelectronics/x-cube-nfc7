/**
  ******************************************************************************
  * @file           : ndef_config.h
  * @brief          : This file allows to select which NDEF features to use.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#ifndef NDEF_CONFIG_H
#define NDEF_CONFIG_H


/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "st_errno.h"
#include <stdint.h>
#include <stdbool.h>
//#include "st_errno.h"


/*
 ******************************************************************************
 * GLOBAL DEFINES
 ******************************************************************************
 */

#define ndefStatus stError    /*!< NDEF error type */

#define NDEF_CC_BUF_LEN             17U                                                /*!< CC buffer len. Max length = 17 in case of T4T v3             */
#define NDEF_NFCV_SUPPORTED_CMD_LEN  4U                                                /*!< Ext sys info supported commands list length                  */
#define NDEF_NFCV_UID_LEN            8U                                                /*!< NFC-V UID length                                             */

#define NDEF_SHORT_VFIELD_MAX_LEN  254U                                                /*!< Max V-field length for 1-byte Lengh encoding                 */
#define NDEF_TERMINATOR_TLV_LEN      1U                                                /*!< Terminator TLV size                                          */
#define NDEF_TERMINATOR_TLV_T     0xFEU                                                /*!< Terminator TLV T=FEh                                         */

#define NDEF_T5T_TxRx_BUFF_HEADER_SIZE        1U                                       /*!< Request Flags/Responses Flags size                           */
#define NDEF_T5T_TxRx_BUFF_FOOTER_SIZE        2U                                       /*!< CRC size                                                     */

#define NDEF_T5T_TxRx_BUFF_SIZE               \
          (32U +  NDEF_T5T_TxRx_BUFF_HEADER_SIZE + NDEF_T5T_TxRx_BUFF_FOOTER_SIZE)     /*!< T5T working buffer size                                      */


typedef enum {
    NDEF_DEV_NONE          = 0x00U,                            /*!< Device type undef                                  */
    NDEF_DEV_T1T           = 0x01U,                            /*!< Device type T1T                                    */
    NDEF_DEV_T2T           = 0x02U,                            /*!< Device type T2T                                    */
    NDEF_DEV_T3T           = 0x03U,                            /*!< Device type T3T                                    */
    NDEF_DEV_T4T           = 0x04U,                            /*!< Device type T4AT/T4BT                              */
    NDEF_DEV_T5T           = 0x05U,                            /*!< Device type T5T                                    */
} ndefDeviceType;

/*! NDEF states  */
typedef enum {
    NDEF_STATE_INVALID     = 0x00U,                            /*!< Invalid state (e.g. no CC)                         */
    NDEF_STATE_INITIALIZED = 0x01U,                            /*!< State Initialized (no NDEF message)                */
    NDEF_STATE_READWRITE   = 0x02U,                            /*!< Valid NDEF found. Read/Write capability            */
    NDEF_STATE_READONLY    = 0x03U,                            /*!< Valid NDEF found. Read only                        */
} ndefState;

/*! T5T Capability Container  */
typedef struct {
    uint8_t                  ccLen;                            /*!< CC Length                                          */
    uint8_t                  magicNumber;                      /*!< Magic number i.e. E1h or E2h                       */
    uint8_t                  majorVersion;                     /*!< Major version i.e. 1                               */
    uint8_t                  minorVersion;                     /*!< Minor version i.e. 0                               */
    uint8_t                  readAccess;                       /*!< NDEF READ access condition                         */
    uint8_t                  writeAccess;                      /*!< NDEF WRITE access condition                        */
    uint16_t                 memoryLen;                        /*!< MLEN (Memory Len). T5T_Area size = 8 * MLEN (bytes)*/
    bool                     specialFrame;                     /*!< Use Special Frames for Write-alike commands        */
    bool                     lockBlock;                        /*!< (EXTENDED_)LOCK_SINGLE_BLOCK supported             */
    bool                     mlenOverflow;                     /*!< memory size exceeds 2040 bytes (Android)           */
    bool                     multipleBlockRead;                /*!< (EXTENDED_)READ_MULTIPLE_BLOCK supported           */
} ndefCapabilityContainerT5T;

/*! Generic Capability Container  */
typedef union {

    ndefCapabilityContainerT5T   t5t;                          /*!< T5T Capability Container                           */

} ndefCapabilityContainer;

/*! NFCV (Extended) System Information  */
typedef struct {
    uint16_t    numberOfBlock;                    /*!< Number of block                                    */
    uint8_t     UID[NDEF_NFCV_UID_LEN];           /*!< UID Value                                          */
    uint8_t     supportedCmd[NDEF_NFCV_SUPPORTED_CMD_LEN];/*!< Supported Commands list                    */
    uint8_t     infoFlags;                        /*!< Information flags                                  */
    uint8_t     DFSID;                            /*!< DFSID Value                                        */
    uint8_t     AFI;                              /*!< AFI Value                                          */
    uint8_t     blockSize;                        /*!< Block Size Value                                   */
    uint8_t     ICRef;                            /*!< IC Reference                                       */
} ndefSystemInformation;

/*! NDEF T5T sub context structure */
typedef struct {
    const uint8_t*               uid;                          /*!< UID in Addressed mode, NULL: Non-addr/Selected mode*/
    uint8_t                      flags;                        /*!< Command flags                                      */
    uint32_t                     TlvNDEFOffset;                /*!< NDEF TLV message offset                            */
    uint8_t                      blockLen;                     /*!< T5T BLEN parameter                                 */
    ndefSystemInformation        sysInfo;                      /*!< System Information (when supported)                */
    bool                         sysInfoSupported;             /*!< System Information Supported flag                  */
    bool                         legacySTHighDensity;          /*!< Legacy ST High Density flag                        */
    uint8_t                      txrxBuf[NDEF_T5T_TxRx_BUFF_SIZE];/*!< Tx Rx Buffer                                    */
    uint8_t                      cacheBuf[NDEF_T5T_TxRx_BUFF_SIZE];/*!< Cache buffer                                   */
    uint32_t                     cacheBlock;                   /*!< Block number of cached buffer                      */
    bool                         useMultipleBlockRead;         /*!< Access multiple block read                         */
    bool                         stDevice;                     /*!< ST device                                          */
} ndefT5TContext;

/*! NDEF context structure */
typedef struct {
    ndefDeviceType               type;                         /*!< NDEF Device type                                   */
    ndefState                    state;                        /*!< Tag state e.g. NDEF_STATE_INITIALIZED              */
    ndefCapabilityContainer      cc;                           /*!< Capability Container                               */
    uint32_t                     messageLen;                   /*!< NDEF message length                                */
    uint32_t                     messageOffset;                /*!< NDEF message offset                                */
    uint32_t                     areaLen;                      /*!< Area Length for NDEF storage                       */
    uint8_t                      ccBuf[NDEF_CC_BUF_LEN];       /*!< buffer for CC                                      */
    const struct ndefPollerWrapperStruct*
                                 ndefPollWrapper;              /*!< pointer to array of function for wrapper           */
    union {
        ndefT5TContext t5t;                                    /*!< T5T context                                        */
    } subCtx;                                                  /*!< Sub-context union                                  */
} ndefContext;

/*
 ******************************************************************************
 * NDEF FEATURES CONFIGURATION
 ******************************************************************************
 */

#ifdef NDEF_CONFIG_CUSTOM

#include "ndef_config_custom.h"

/* Define default configuration when not set in ndef_config_custom.h */

#ifndef NDEF_FEATURE_FULL_API
#define NDEF_FEATURE_FULL_API                  false      /*!< Support Write, Format, Check Presence, set Read-only in addition to the Read feature */
#endif

#ifndef NDEF_TYPE_EMPTY_SUPPORT
#define NDEF_TYPE_EMPTY_SUPPORT                false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_FLAT_SUPPORT
#define NDEF_TYPE_FLAT_SUPPORT                 false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_AAR_SUPPORT
#define NDEF_TYPE_RTD_AAR_SUPPORT              false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_DEVICE_INFO_SUPPORT
#define NDEF_TYPE_RTD_DEVICE_INFO_SUPPORT      false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_URI_SUPPORT
#define NDEF_TYPE_RTD_URI_SUPPORT              false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_TEXT_SUPPORT
#define NDEF_TYPE_RTD_TEXT_SUPPORT             false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_WLC_SUPPORT
#define NDEF_TYPE_RTD_WLC_SUPPORT              false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_RTD_TNEP_SUPPORT
#define NDEF_TYPE_RTD_TNEP_SUPPORT             false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_MEDIA_SUPPORT
#define NDEF_TYPE_MEDIA_SUPPORT                false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_BLUETOOTH_SUPPORT
#define NDEF_TYPE_BLUETOOTH_SUPPORT            false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_VCARD_SUPPORT
#define NDEF_TYPE_VCARD_SUPPORT                false      /* NDEF library configuration missing. Disabled by default */
#endif
#ifndef NDEF_TYPE_WIFI_SUPPORT
#define NDEF_TYPE_WIFI_SUPPORT                 false      /* NDEF library configuration missing. Disabled by default */
#endif

#else /* NDEF_CONFIG_CUSTOM */

#define NDEF_FEATURE_FULL_API                  true       /*!< Support Write, Format, Check Presence, set Read-only in addition to the Read feature */

#define NDEF_TYPE_EMPTY_SUPPORT                true       /*!< Support Empty type                          */
#define NDEF_TYPE_FLAT_SUPPORT                 false      /*!< Support Flat type                           */
#define NDEF_TYPE_RTD_DEVICE_INFO_SUPPORT      false      /*!< Support RTD Device Information type         */
#define NDEF_TYPE_RTD_TEXT_SUPPORT             false      /*!< Support RTD Text type                       */
#define NDEF_TYPE_RTD_URI_SUPPORT              true       /*!< Support RTD URI type                        */
#define NDEF_TYPE_RTD_AAR_SUPPORT              false      /*!< Support RTD Android Application Record type */
#define NDEF_TYPE_RTD_WLC_SUPPORT              false      /*!< Support RTD WLC Types                       */
#define NDEF_TYPE_RTD_WPCWLC_SUPPORT           false      /*!< Support RTD WPC WLC type                    */
#define NDEF_TYPE_RTD_TNEP_SUPPORT             false      /*!< Support RTD TNEP Types                      */
#define NDEF_TYPE_MEDIA_SUPPORT                false      /*!< Support Media type                          */
#define NDEF_TYPE_BLUETOOTH_SUPPORT            false      /*!< Support Bluetooth types                     */
#define NDEF_TYPE_VCARD_SUPPORT                false      /*!< Support vCard type                          */
#define NDEF_TYPE_WIFI_SUPPORT                 false      /*!< Support Wifi type                           */

#endif /* NDEF_CONFIG_CUSTOM */

#endif /* NDEF_CONFIG_H */

/**
  * @}
  *
  */
