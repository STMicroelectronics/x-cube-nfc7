/**
  ******************************************************************************
  * @file           : nfc07a1_ndef_nfcv_wrapper.h
  * @brief          : Provides NDEF methods and definitions to access NFC-V Forum T5T
  *
  *  This module provides an interface to perform as a NFC-V Reader/Writer
  *  to handle a Type 5 Tag T5T
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

#ifndef NDEF_NFCV_WRAPPER_H
#define NDEF_NFCV_WRAPPER_H

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "ndef_config.h"
#include "nfc07a1_nfctag.h"

/* Aliases to BSP functions */
#define NFCTAG_ReadData     NFC07A1_NFCTAG_ReadData
#define NFCTAG_WriteData    NFC07A1_NFCTAG_WriteData
#define NFCTAG_ReadRegister NFC07A1_NFCTAG_ReadRegister

ndefStatus ndefT5TagContextInitialization(ndefContext *ctx);
ndefStatus ndefT5TagNdefDetect(ndefContext *ctx);
ndefStatus ndefT5TagFormat(ndefContext *ctx, const ndefCapabilityContainer *cc, uint32_t options);
uint8_t ndefT5TagGetBlockLength(ndefContext *ctx);
ndefStatus ndefT5TGetMemoryConfig(ndefContext *ctx);
ndefStatus ndefT5TagReadBytes(ndefContext *ctx, uint32_t offset, uint32_t len, uint8_t *buf);
ndefStatus ndefT5TagReadRawMessage(ndefContext *ctx, uint8_t *buf, uint32_t bufLen);
ndefStatus ndefT5TagWriteRawMessageLen(ndefContext *ctx, uint32_t rawMessageLen, bool writeTerminator);
ndefStatus ndefT5TagWriteRawMessage(ndefContext *ctx, const uint8_t *buf, uint32_t bufLen);
ndefStatus ndefT5TagCheckAvailableSpace(const ndefContext *ctx, uint32_t messageLen);
ndefStatus ndefT5TagBeginWriteMessage(ndefContext *ctx, uint32_t messageLen);
ndefStatus ndefT5TagEndWriteMessage(ndefContext *ctx, uint32_t messageLen, bool writeTerminator);

#endif /* NDEF_NFCV_WRAPPER_H */

