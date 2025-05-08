/**
  ******************************************************************************
  * @file           : ndef_nfcv_wrapper.c
  * @brief          : Provides NDEF methods and definitions to access NFC-V Forum T5T
  *
  *  This module provides an interface to perform as a NFC-V serial interface
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

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include "ndef_nfcv_wrapper.h"
#include "utils.h"

/*
******************************************************************************
* ENABLE SWITCH
******************************************************************************
*/

#ifndef NDEF_FEATURE_FULL_API
#error " NDEF: Module configuration missing. Please enable/disable Full API by setting: NDEF_FEATURE_FULL_API"
#endif  /*NDEF feature undefined**/

#ifdef TEST_NDEF
#define NDEF_SKIP_T5T_SYS_INFO /* Must not call ndefT5TGetSystemInformation() in test mode */
#endif  /*TEST NDEF**/

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/
#define NDEF_T5T_MLEN_DIVIDER                  8U    /*!<  T5T_area size is measured in bytes is equal to 8 * MLEN */

#define NDEF_T5T_TLV_T_LEN                     1U    /*!< TLV T Length: 1 bytes                             */
#define NDEF_T5T_TLV_L_1_BYTES_LEN             1U    /*!< TLV L Length: 1 bytes                             */
#define NDEF_T5T_TLV_L_3_BYTES_LEN             3U    /*!< TLV L Length: 3 bytes                             */

#define NDEF_T5T_MAX_BLOCK_1_BYTE_ADDR       256U    /*!< Max number of blocks for 1 byte addressing        */
#define NDEF_T5T_MAX_MLEN_1_BYTE_ENCODING    256U    /*!< MLEN max value for 1 byte encoding                */

#define NDEF_T5T_TL_MIN_SIZE  (NDEF_T5T_TLV_T_LEN \
+ NDEF_T5T_TLV_L_1_BYTES_LEN) /*!< Min TL size                                       */

#define NDEF_T5T_TL_MAX_SIZE  (NDEF_T5T_TLV_T_LEN \
+ NDEF_T5T_TLV_L_3_BYTES_LEN) /*!< Max TL size                                       */

#define NDEF_T5T_TLV_NDEF                   0x03U    /*!< TLV flag NDEF value                               */
#define NDEF_T5T_TLV_PROPRIETARY            0xFDU    /*!< TLV flag PROPRIETARY value                        */
#define NDEF_T5T_TLV_TERMINATOR             0xFEU    /*!< TLV flag TERMINATOR value                         */
#define NDEF_T5T_TLV_RFU                    0x00U    /*!< TLV flag RFU value                                */

#define NDEF_T5T_ACCESS_ALWAYS               0x0U    /*!< Read/Write Access. 00b: Always                    */
#define NDEF_T5T_ACCESS_RFU                  0x1U    /*!< Read/Write Access. 01b: RFU                       */
#define NDEF_T5T_ACCESS_PROPRIETARY          0x2U    /*!< Read/Write Access. 00b: Proprietary               */
#define NDEF_T5T_ACCESS_NEVER                0x3U    /*!< Read/Write Access. 00b: Never                     */

#define NDEF_T5T_SYSINFO_MAX_LEN              22U    /*!< Max length for (Extended) Get System Info response */

#define NDEF_T5T_MAX_BLOCK_1_BYTE_ADDR       256U    /*!< Max number of blocks for 1 byte addressing        */

#ifndef NDEF_T5T_N_RETRY_ERROR
#define NDEF_T5T_N_RETRY_ERROR                2U     /*!< nT5T,RETRY,ERROR DP 2.2  �B.12                    */
#endif /* NDEF_T5T_N_RETRY_ERROR */

#define NDEF_T5T_FLAG_LEN                     1U     /*!< Flag byte length                                  */

#define NDEF_T5T_MAPPING_VERSION_1_0                     (1U << 6)    /*!< T5T Version 1.0                                    */

#define NDEF_SYSINFO_FLAG_DFSID_POS                           (0U)    /*!< Info flags DFSID flag position                     */
#define NDEF_SYSINFO_FLAG_AFI_POS                             (1U)    /*!< Info flags AFI flag position                       */
#define NDEF_SYSINFO_FLAG_MEMSIZE_POS                         (2U)    /*!< Info flags Memory Size flag position               */
#define NDEF_SYSINFO_FLAG_ICREF_POS                           (3U)    /*!< Info flags IC reference flag position              */
#define NDEF_SYSINFO_FLAG_MOI_POS                             (4U)    /*!< Info flags MOI flag position                       */
#define NDEF_SYSINFO_FLAG_CMDLIST_POS                         (5U)    /*!< Info flags Command List flag position              */
#define NDEF_SYSINFO_FLAG_CSI_POS                             (6U)    /*!< Info flags CSI flag position                       */
#define NDEF_SYSINFO_FLAG_LEN_POS                             (7U)    /*!< Info flags Length  position                        */

#define NDEF_CMDLIST_READSINGLEBLOCK_POS                      (0U)    /*!< Cmd List: ReadSingleBlock position                 */
#define NDEF_CMDLIST_WRITESINGLEBLOCK_POS                     (1U)    /*!< Cmd List: WriteSingleBlock position                */
#define NDEF_CMDLIST_LOCKSINGLEBLOCK_POS                      (2U)    /*!< Cmd List: LockSingleBlock position                 */
#define NDEF_CMDLIST_READMULTIPLEBLOCKS_POS                   (3U)    /*!< Cmd List: ReadMultipleBlocks position              */
#define NDEF_CMDLIST_WRITEMULTIPLEBLOCKS_POS                  (4U)    /*!< Cmd List: WriteMultipleBlocks position             */
#define NDEF_CMDLIST_SELECT_POS                               (5U)    /*!< Cmd List: SelectSupported position                 */
#define NDEF_CMDLIST_RESETTOREADY_POS                         (6U)    /*!< Cmd List: ResetToReady position                    */
#define NDEF_CMDLIST_GETMULTIPLEBLOCKSECSTATUS_POS            (7U)    /*!< Cmd List: GetMultipleBlockSecStatus position       */

#define NDEF_CMDLIST_WRITEAFI_POS                             (0U)    /*!< Cmd List: WriteAFI position                        */
#define NDEF_CMDLIST_LOCKAFI_POS                              (1U)    /*!< Cmd List: LockAFI position                         */
#define NDEF_CMDLIST_WRITEDSFID_POS                           (2U)    /*!< Cmd List: WriteDSFID position                      */
#define NDEF_CMDLIST_LOCKDSFID_POS                            (3U)    /*!< Cmd List: LockDSFID position                       */
#define NDEF_CMDLIST_GETSYSTEMINFORMATION_POS                 (4U)    /*!< Cmd List: GetSystemInformation position            */
#define NDEF_CMDLIST_CUSTOMCMDS_POS                           (5U)    /*!< Cmd List: CustomCmds position                      */
#define NDEF_CMDLIST_FASTREADMULTIPLEBLOCKS_POS               (6U)    /*!< Cmd List: FastReadMultipleBlocks position          */

#define NDEF_CMDLIST_EXTREADSINGLEBLOCK_POS                   (0U)    /*!< Cmd List: ExtReadSingleBlock position              */
#define NDEF_CMDLIST_EXTWRITESINGLEBLOCK_POS                  (1U)    /*!< Cmd List: ExtWriteSingleBlock position             */
#define NDEF_CMDLIST_EXTLOCKSINGLEBLOCK_POS                   (2U)    /*!< Cmd List: ExtLockSingleBlock position              */
#define NDEF_CMDLIST_EXTREADMULTIPLEBLOCKS_POS                (3U)    /*!< Cmd List: ExtReadMultipleBlocks position           */
#define NDEF_CMDLIST_EXTWRITEMULTIPLEBLOCKS_POS               (4U)    /*!< Cmd List: ExtWriteMultipleBlocks position          */
#define NDEF_CMDLIST_EXTGETMULTIPLEBLOCKSECSTATUS_POS         (5U)    /*!< Cmd List: ExtGetMultipleBlockSecStatus position    */
#define NDEF_CMDLIST_FASTEXTENDEDREADMULTIPLEBLOCKS_POS       (6U)    /*!< Cmd List: FastExtendedReadMultipleBlocks position  */

#define NDEF_T5T_CC_MAGIC_1_BYTE_ADDR_MODE                   0xE1U    /*!< T5T CC Magic Number (1-byte Address Mode)           */
#define NDEF_T5T_CC_MAGIC_2_BYTE_ADDR_MODE                   0xE2U    /*!< T5T CC Magic Number (2-byte Address Mode)           */
#define NDEF_T5T_CC_LEN_4_BYTES                                 4U    /*!< T5T CC Length (4 bytes)                            */
#define NDEF_T5T_CC_LEN_8_BYTES                                 8U    /*!< T5T CC Length (8 bytes)                            */
#define NDEF_T5T_FORMAT_OPTION_NFC_FORUM                        1U    /*!< Format tag according to NFC Forum MLEN computation */

#define ndefT5TMajorVersion(V)                                         ((uint8_t)( (V) >> 6U))                                            /*!< Get major version                                  */
#define ndefT5TMinorVersion(V)                                         ((uint8_t)(((V) >> 4U) & 3U))                                      /*!< Get minor version                                  */

#define ndefT5TSysInfoDFSIDPresent(infoFlags)                          (((infoFlags) >> NDEF_SYSINFO_FLAG_DFSID_POS)   & 0x01U)           /*!< Returns DFSID presence flag                        */
#define ndefT5TSysInfoAFIPresent(infoFlags)                            (((infoFlags) >> NDEF_SYSINFO_FLAG_AFI_POS)     & 0x01U)           /*!< Returns AFI presence flag                          */
#define ndefT5TSysInfoMemSizePresent(infoFlags)                        (((infoFlags) >> NDEF_SYSINFO_FLAG_MEMSIZE_POS) & 0x01U)           /*!< Returns Memory size presence flag                  */
#define ndefT5TSysInfoICRefPresent(infoFlags)                          (((infoFlags) >> NDEF_SYSINFO_FLAG_ICREF_POS)   & 0x01U)           /*!< Returns IC Reference presence flag                 */
#define ndefT5TSysInfoMOIValue(infoFlags)                              (((infoFlags) >> NDEF_SYSINFO_FLAG_MOI_POS)     & 0x01U)           /*!< Returns MOI value                                  */
#define ndefT5TSysInfoCmdListPresent(infoFlags)                        (((infoFlags) >> NDEF_SYSINFO_FLAG_CMDLIST_POS) & 0x01U)           /*!< Returns Command List presence flag                 */
#define ndefT5TSysInfoCSIPresent(infoFlags)                            (((infoFlags) >> NDEF_SYSINFO_FLAG_CSI_POS)     & 0x01U)           /*!< Returns CSI presence flag                          */
#define ndefT5TSysInfoLenValue(infoFlags)                              (((infoFlags) >> NDEF_SYSINFO_FLAG_LEN_POS)     & 0x01U)           /*!< Returns Info flag length value                     */

#define ndefT5TSysInfoReadSingleBlockSupported(cmdList)                (((cmdList)[0] >> NDEF_CMDLIST_READSINGLEBLOCK_POS)                & 0x01U) /*!< Returns ReadSingleBlock support flag                 */
#define ndefT5TSysInfoWriteSingleBlockSupported(cmdList)               (((cmdList)[0] >> NDEF_CMDLIST_WRITESINGLEBLOCK_POS)               & 0x01U) /*!< Returns WriteSingleBlock support flag                */
#define ndefT5TSysInfoLockSingleBlockSupported(cmdList)                (((cmdList)[0] >> NDEF_CMDLIST_LOCKSINGLEBLOCK_POS)                & 0x01U) /*!< Returns LockSingleBlock support flag                 */
#define ndefT5TSysInfoReadMultipleBlocksSupported(cmdList)             (((cmdList)[0] >> NDEF_CMDLIST_READMULTIPLEBLOCKS_POS)             & 0x01U) /*!< Returns ReadMultipleBlocks support flag              */
#define ndefT5TSysInfoWriteMultipleBlocksSupported(cmdList)            (((cmdList)[0] >> NDEF_CMDLIST_WRITEMULTIPLEBLOCKS_POS)            & 0x01U) /*!< Returns WriteMultipleBlocks support flag             */
#define ndefT5TSysInfoSelectSupported(cmdList)                         (((cmdList)[0] >> NDEF_CMDLIST_SELECT_POS)                         & 0x01U) /*!< Returns SelectSupported support flag                 */
#define ndefT5TSysInfoResetToReadySupported(cmdList)                   (((cmdList)[0] >> NDEF_CMDLIST_RESETTOREADY_POS)                   & 0x01U) /*!< Returns ResetToReady support flag                    */
#define ndefT5TSysInfoGetMultipleBlockSecStatusSupported(cmdList)      (((cmdList)[0] >> NDEF_CMDLIST_GETMULTIPLEBLOCKSECSTATUS_POS)      & 0x01U) /*!< Returns GetMultipleBlockSecStatus support flag       */

#define ndefT5TSysInfoWriteAFISupported(cmdList)                       (((cmdList)[1] >> NDEF_CMDLIST_WRITEAFI_POS)                       & 0x01U) /*!< Returns WriteAFI support flag                        */
#define ndefT5TSysInfoLockAFISupported(cmdList)                        (((cmdList)[1] >> NDEF_CMDLIST_LOCKAFI_POS)                        & 0x01U) /*!< Returns LockAFI support flag                         */
#define ndefT5TSysInfoWriteDSFIDSupported(cmdList)                     (((cmdList)[1] >> NDEF_CMDLIST_WRITEDSFID_POS)                     & 0x01U) /*!< Returns WriteDSFID support flag                      */
#define ndefT5TSysInfoLockDSFIDSupported(cmdList)                      (((cmdList)[1] >> NDEF_CMDLIST_LOCKDSFID_POS)                      & 0x01U) /*!< Returns LockDSFID support flag                       */
#define ndefT5TSysInfoGetSystemInformationSupported(cmdList)           (((cmdList)[1] >> NDEF_CMDLIST_GETSYSTEMINFORMATION_POS)           & 0x01U) /*!< Returns GetSystemInformation support flag            */
#define ndefT5TSysInfoCustomCmdsSupported(cmdList)                     (((cmdList)[1] >> NDEF_CMDLIST_CUSTOMCMDS_POS)                     & 0x01U) /*!< Returns CustomCmds support flag                      */
#define ndefT5TSysInfoFastReadMultipleBlocksSupported(cmdList)         (((cmdList)[1] >> NDEF_CMDLIST_FASTREADMULTIPLEBLOCKS_POS)         & 0x01U) /*!< Returns FastReadMultipleBlocks support flag          */

#define ndefT5TSysInfoExtReadSingleBlockSupported(cmdList)             (((cmdList)[2] >> NDEF_CMDLIST_EXTREADSINGLEBLOCK_POS)             & 0x01U) /*!< Returns ExtReadSingleBlock support flag              */
#define ndefT5TSysInfoExtWriteSingleBlockSupported(cmdList)            (((cmdList)[2] >> NDEF_CMDLIST_EXTWRITESINGLEBLOCK_POS)            & 0x01U) /*!< Returns ExtWriteSingleBlock support flag             */
#define ndefT5TSysInfoExtLockSingleBlockSupported(cmdList)             (((cmdList)[2] >> NDEF_CMDLIST_EXTLOCKSINGLEBLOCK_POS)             & 0x01U) /*!< Returns ExtLockSingleBlock support flag              */
#define ndefT5TSysInfoExtReadMultipleBlocksSupported(cmdList)          (((cmdList)[2] >> NDEF_CMDLIST_EXTREADMULTIPLEBLOCKS_POS)          & 0x01U) /*!< Returns ExtReadMultipleBlocks support flag           */
#define ndefT5TSysInfoExtWriteMultipleBlocksSupported(cmdList)         (((cmdList)[2] >> NDEF_CMDLIST_EXTWRITEMULTIPLEBLOCKS_POS)         & 0x01U) /*!< Returns ExtWriteMultipleBlocks support flag          */
#define ndefT5TSysInfoExtGetMultipleBlockSecStatusSupported(cmdList)   (((cmdList)[2] >> NDEF_CMDLIST_EXTGETMULTIPLEBLOCKSECSTATUS_POS)   & 0x01U) /*!< Returns ExtGetMultipleBlockSecStatus support flag    */
#define ndefT5TSysInfoFastExtendedReadMultipleBlocksSupported(cmdList) (((cmdList)[2] >> NDEF_CMDLIST_FASTEXTENDEDREADMULTIPLEBLOCKS_POS) & 0x01U) /*!< Returns FastExtendedReadMultipleBlocks support flag  */

#define ndefT5TInvalidateCache(ctx)     { (ctx)->subCtx.t5t.cacheBlock = 0xFFFFFFFFU; }    /*!< Invalidate the internal cache, before reading a buffer  */
#define ndefT5TIsValidCache(ctx, block) ( (ctx)->subCtx.t5t.cacheBlock == (block) )        /*!< Check the internal cache is valid to avoid useless read */

/*
*****************************************************************************
* GLOBAL TYPES
******************************************************************************
*/

#define NDEF_T5T_UID_MANUFACTURER_ID_POS       6U    /*!< Manufacturer ID Offset in UID buffer (reverse)    */
#define NDEF_T5T_MANUFACTURER_ID_ST         0x02U    /*!< Manufacturer ID for ST                            */

/*
******************************************************************************
* GLOBAL MACROS
******************************************************************************
*/

/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/

/*******************************************************************************/
static ndefStatus ndefT5TReadLField(ndefContext *ctx)
{
  ndefStatus           ret;
  uint32_t             offset;
  uint8_t              data[3];
  uint16_t             lenTLV;

  ctx->state = NDEF_STATE_INVALID;
  offset = ctx->subCtx.t5t.TlvNDEFOffset;
  offset++;
  ret = ndefT5TagReadBytes(ctx, offset, 1, data);
  if (ret != ERR_NONE)
  {
    /* Conclude procedure */
    return ret;
  }
  offset++;
  lenTLV = data[0];
  if (lenTLV == (NDEF_SHORT_VFIELD_MAX_LEN + 1U))
  {
    ret = ndefT5TagReadBytes(ctx, offset, 2, data);
    if (ret != ERR_NONE)
    {
      /* Conclude procedure */
      return ret;
    }
    offset += 2U;
    lenTLV = GETU16(&data[0]);
  }
  ctx->messageLen    = lenTLV;
  ctx->messageOffset = offset;

  if (lenTLV == 0U)
  {
    if (!((ctx->cc.t5t.readAccess  == NDEF_T5T_ACCESS_ALWAYS) && (ctx->cc.t5t.writeAccess == NDEF_T5T_ACCESS_ALWAYS)))
    {
      /* Conclude procedure  */
      return ERR_REQUEST;
    }
    ctx->state = NDEF_STATE_INITIALIZED;
  }
  else
  {
    if (!(ctx->cc.t5t.readAccess == NDEF_T5T_ACCESS_ALWAYS))
    {
      /* Conclude procedure  */
      return ERR_REQUEST;
    }
    ctx->state = (ctx->cc.t5t.writeAccess == NDEF_T5T_ACCESS_ALWAYS) ? NDEF_STATE_READWRITE : NDEF_STATE_READONLY;
  }

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagContextInitialization(ndefContext *ctx)
{
#if !defined NDEF_SKIP_T5T_SYS_INFO
  ndefStatus    result;
#endif

  if (ctx == NULL)
  {
    return ERR_PARAM;
  }

  ndefT5TInvalidateCache(ctx);

  /* Reset info about the card */
  ctx->type                     = NDEF_DEV_T5T;
  ctx->state                    = NDEF_STATE_INVALID;

  /* Initialize CC fields, used in NDEF detect */
  ctx->cc.t5t.ccLen             = 0U;
  ctx->cc.t5t.magicNumber       = 0U;
  ctx->cc.t5t.majorVersion      = 0U;
  ctx->cc.t5t.minorVersion      = 0U;
  ctx->cc.t5t.readAccess        = 0U;
  ctx->cc.t5t.writeAccess       = 0U;
  ctx->cc.t5t.memoryLen         = 0U;
  ctx->cc.t5t.specialFrame      = false;
  ctx->cc.t5t.lockBlock         = false;
  ctx->cc.t5t.mlenOverflow      = false;
  ctx->cc.t5t.multipleBlockRead = false;

  ctx->subCtx.t5t.blockLen      = 0U;
  ctx->subCtx.t5t.TlvNDEFOffset = 0U; /* Offset for TLV */
  ctx->subCtx.t5t.useMultipleBlockRead = false;

  ctx->subCtx.t5t.stDevice = 1;

  /* Get block length, and set subCtx.t5t.legacySTHighDensity */
  ctx->subCtx.t5t.blockLen = ndefT5TagGetBlockLength(ctx);

  ctx->subCtx.t5t.sysInfoSupported = false;

  result = ndefT5TGetMemoryConfig(ctx);
  if (result != ERR_NONE)
  {
    return result;
  }

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagNdefDetect(ndefContext *ctx)
{
  ndefStatus result;
  uint8_t    tmpBuf[NDEF_T5T_TL_MAX_SIZE];
  ndefStatus returnCode = ERR_REQUEST; /* Default return code */
  uint16_t   offset;
  uint16_t   length;
  uint32_t   TlvOffset;
  bool       exit;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  ctx->state                           = NDEF_STATE_INVALID;
  ctx->cc.t5t.ccLen                    = 0U;
  ctx->cc.t5t.memoryLen                = 0U;
  ctx->cc.t5t.multipleBlockRead        = false;
  ctx->messageLen                      = 0U;
  ctx->messageOffset                   = 0U;
  ctx->areaLen                         = 0U;

  /* Read the CC with Single Block Read command(s) first and update multipleBlockRead flag later */
  result = NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, ctx->ccBuf, 0, 4);
  if ((result == ERR_NONE) && ((ctx->ccBuf[0] == (uint8_t)0xE1U) || (ctx->ccBuf[0] == (uint8_t)0xE2U)))
  {
    ctx->cc.t5t.magicNumber           =  ctx->ccBuf[0U];
    ctx->cc.t5t.majorVersion          = (ctx->ccBuf[1U] >> 6U) & 0x03U;
    ctx->cc.t5t.minorVersion          = (ctx->ccBuf[1U] >> 4U) & 0x03U;
    ctx->cc.t5t.readAccess            = (ctx->ccBuf[1U] >> 2U) & 0x03U;
    ctx->cc.t5t.writeAccess           = (ctx->ccBuf[1U] >> 0U) & 0x03U;
    ctx->cc.t5t.memoryLen             =  ctx->ccBuf[2U];
    ctx->cc.t5t.specialFrame          = (((ctx->ccBuf[3U] >> 4U) & 0x01U) != 0U);
    ctx->cc.t5t.lockBlock             = (((ctx->ccBuf[3U] >> 3U) & 0x01U) != 0U);
    ctx->cc.t5t.mlenOverflow          = (((ctx->ccBuf[3U] >> 2U) & 0x01U) != 0U);

    /* Check Magic Number TS T5T v1.0 - 7.5.1.2 */
    if ((ctx->cc.t5t.magicNumber != NDEF_T5T_CC_MAGIC_1_BYTE_ADDR_MODE) &&
       (ctx->cc.t5t.magicNumber != NDEF_T5T_CC_MAGIC_2_BYTE_ADDR_MODE))
    {
      return ERR_REQUEST;
    }

    /* Check version - 7.5.1.2 */
    if (ctx->cc.t5t.majorVersion > ndefT5TMajorVersion(NDEF_T5T_MAPPING_VERSION_1_0))
    {
      return ERR_REQUEST;
    }

    /* Check read access - 7.5.1.2 */
    if (ctx->cc.t5t.readAccess != NDEF_T5T_ACCESS_ALWAYS)
    {
      return ERR_REQUEST;
    }

    if (ctx->cc.t5t.memoryLen != 0U)
    {
      /* 4-byte CC */
      ctx->cc.t5t.ccLen         = NDEF_T5T_CC_LEN_4_BYTES;
      if ((ctx->cc.t5t.memoryLen == 0xFFU) && ctx->cc.t5t.mlenOverflow)
      {
        if ((ctx->subCtx.t5t.sysInfoSupported == true) && (ndefT5TSysInfoMemSizePresent(ctx->subCtx.t5t.sysInfo.infoFlags) != 0U))
        {
          ctx->cc.t5t.memoryLen = (uint16_t)((ctx->subCtx.t5t.sysInfo.numberOfBlock * ctx->subCtx.t5t.sysInfo.blockSize) / NDEF_T5T_MLEN_DIVIDER);
        }
      }
    }
    else
    {
      /* 8-byte CC */
      result = NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, &ctx->ccBuf[4U], 4, 4);
      if ((result == ERR_NONE))
      {
        ctx->cc.t5t.ccLen     = NDEF_T5T_CC_LEN_8_BYTES;
        ctx->cc.t5t.memoryLen = ((uint16_t)ctx->ccBuf[6U] << 8U) + (uint16_t)ctx->ccBuf[7U];
      }
    }

    /* Update multipleBlockRead flag after having read the second half of 8-byte CC */
    ctx->cc.t5t.multipleBlockRead     = (((ctx->ccBuf[3U] >> 0U ) & 0x01U) != 0U);

    if ((ctx->subCtx.t5t.sysInfoSupported == true) &&
       (ndefT5TSysInfoMemSizePresent(ctx->subCtx.t5t.sysInfo.infoFlags) != 0U) &&
         (ctx->cc.t5t.memoryLen == (uint16_t)((ctx->subCtx.t5t.sysInfo.numberOfBlock * ctx->subCtx.t5t.sysInfo.blockSize) / NDEF_T5T_MLEN_DIVIDER)) &&
           (ctx->cc.t5t.memoryLen > 0U))
    {
      ctx->cc.t5t.memoryLen--; /* remove CC area from memory length */
    }

    ctx->messageLen     = 0U;
    ctx->messageOffset  = ctx->cc.t5t.ccLen;
    /* TS T5T v1.0 4.3.1.17 T5T_Area size is measured in bytes, is equal to MLEN * 8 */
    ctx->areaLen        = (uint32_t)ctx->cc.t5t.memoryLen * NDEF_T5T_MLEN_DIVIDER;

    TlvOffset = ctx->cc.t5t.ccLen;
    exit      = false;
    while( (exit == false) && (TlvOffset < (ctx->cc.t5t.ccLen + ctx->areaLen)) )
    {
      result = NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, tmpBuf, TlvOffset, NDEF_T5T_TL_MIN_SIZE);
      if (result != ERR_NONE)
      {
        return result;
      }
      offset = NDEF_T5T_TLV_T_LEN + NDEF_T5T_TLV_L_1_BYTES_LEN;
      length = tmpBuf[1U];
      if (length == (NDEF_SHORT_VFIELD_MAX_LEN + 1U))
      {
        /* Size is encoded in 1 + 2 bytes */
        result = NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, tmpBuf, TlvOffset, NDEF_T5T_TL_MAX_SIZE);
        if (result != ERR_NONE)
        {
          return result;
        }
        length = (((uint16_t)tmpBuf[2U]) << 8U) + (uint16_t)tmpBuf[3U];
        offset += 2U;
      }
      if (tmpBuf[0U] == (uint8_t)NDEF_T5T_TLV_NDEF)
      {
        /* NDEF record return it */
        returnCode                    = ERR_NONE;  /* Default */
        ctx->subCtx.t5t.TlvNDEFOffset = TlvOffset; /* Offset for TLV */
        ctx->messageOffset            = TlvOffset + offset;
        ctx->messageLen               = length;
        if (length == 0U)
        {
          /* Req 40 7.5.1.6 */
          if ( (ctx->cc.t5t.readAccess  == NDEF_T5T_ACCESS_ALWAYS) &&
              (ctx->cc.t5t.writeAccess == NDEF_T5T_ACCESS_ALWAYS) )
          {
            ctx->state = NDEF_STATE_INITIALIZED;
          }
          else
          {
            ctx->state = NDEF_STATE_INVALID;
            returnCode = ERR_REQUEST; /* Default */
          }
          exit = true;
        }
        else
        {
          if (ctx->cc.t5t.readAccess == NDEF_T5T_ACCESS_ALWAYS)
          {
            if (ctx->cc.t5t.writeAccess == NDEF_T5T_ACCESS_ALWAYS)
            {
              ctx->state = NDEF_STATE_READWRITE;
            }
            else
            {
              ctx->state = NDEF_STATE_READONLY;
            }
          }
          exit = true;
        }
      }
      else if (tmpBuf[0U]== (uint8_t) NDEF_T5T_TLV_TERMINATOR)
      {
        /* NDEF end */
        exit = true;
      }
      else
      {
        /* Skip Proprietary and RFU too */
        TlvOffset += (uint32_t)offset + (uint32_t)length;
      }
    }
  }
  else
  {
    /* No CC File */
    returnCode = ERR_REQUEST;
    if (result != ERR_NONE)
    {
      returnCode = result;
    }
  }

  return returnCode;
}

/*******************************************************************************/
static ndefStatus ndefT5TagWriteCC(ndefContext *ctx)
{
  ndefStatus  ret;
  uint8_t*    buf;
  uint8_t     dataIt;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  buf    = ctx->ccBuf;
  dataIt = 0U;
  /* Encode CC */
  buf[dataIt] = ctx->cc.t5t.magicNumber;                                                                /* Byte 0 */
  dataIt++;
  buf[dataIt] = (uint8_t)(((ctx->cc.t5t.majorVersion  & 0x03U) << 6) |                                  /* Byte 1 */
                          ((ctx->cc.t5t.minorVersion  & 0x03U) << 4) |                                  /*        */
                            ((ctx->cc.t5t.readAccess    & 0x03U) << 2) |                                  /*        */
                              ((ctx->cc.t5t.writeAccess   & 0x03U) << 0));                                  /*        */
  dataIt++;
  buf[dataIt] = (ctx->cc.t5t.ccLen == NDEF_T5T_CC_LEN_8_BYTES) ? 0U : (uint8_t)ctx->cc.t5t.memoryLen;   /* Byte 2 */
  dataIt++;
  buf[dataIt] = 0U;                                                                                     /* Byte 3 */
  if( ctx->cc.t5t.multipleBlockRead ) { buf[dataIt] |= 0x01U; }                                         /* Byte 3  b0 MBREAD                */
  if( ctx->cc.t5t.mlenOverflow )      { buf[dataIt] |= 0x04U; }                                         /* Byte 3  b2 Android MLEN overflow */
  if( ctx->cc.t5t.lockBlock )         { buf[dataIt] |= 0x08U; }                                         /* Byte 3  b3 Lock Block            */
  if( ctx->cc.t5t.specialFrame )      { buf[dataIt] |= 0x10U; }                                         /* Byte 3  b4 Special Frame         */
  dataIt++;
  if (ctx->cc.t5t.ccLen == NDEF_T5T_CC_LEN_8_BYTES)
  {
    buf[dataIt] = 0U;                                                                                 /* Byte 4 */
    dataIt++;
    buf[dataIt] = 0U;                                                                                 /* Byte 5 */
    dataIt++;
    buf[dataIt] = (uint8_t)(ctx->cc.t5t.memoryLen >> 8);                                              /* Byte 6 */
    dataIt++;
    buf[dataIt] = (uint8_t)(ctx->cc.t5t.memoryLen >> 0);                                              /* Byte 7 */
    dataIt++;
  }

  ret = NFCTAG_WriteData(NFC07A1_NFCTAG_INSTANCE, buf, 0, ctx->cc.t5t.ccLen);
  return ret;
}

/*******************************************************************************/
ndefStatus ndefT5TagFormat(ndefContext *ctx, const ndefCapabilityContainer *cc, uint32_t options)
{
  ndefStatus               result;
  static const uint8_t     emptyNDEF[] = { 0x03U, 0x00U };

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  /* Reset previous potential info about NDEF messages */
  ctx->messageLen               = 0U;
  ctx->messageOffset            = 0U;
  ctx->subCtx.t5t.TlvNDEFOffset = 0U;

  if (cc != NULL)
  {
    if ((cc->t5t.ccLen != NDEF_T5T_CC_LEN_8_BYTES) && (cc->t5t.ccLen != NDEF_T5T_CC_LEN_4_BYTES))
    {
      return ERR_PARAM;
    }
    (void)ST_MEMCPY(&ctx->cc, cc, sizeof(ndefCapabilityContainer));
  }
  else
  {
    /* Try to find the appropriate cc values */
    ctx->cc.t5t.magicNumber  = NDEF_T5T_CC_MAGIC_1_BYTE_ADDR_MODE; /* E1 */
    ctx->cc.t5t.majorVersion = ndefT5TMajorVersion(NDEF_T5T_MAPPING_VERSION_1_0);
    ctx->cc.t5t.minorVersion = ndefT5TMinorVersion(NDEF_T5T_MAPPING_VERSION_1_0);
    ctx->cc.t5t.readAccess   = NDEF_T5T_ACCESS_ALWAYS;
    ctx->cc.t5t.writeAccess  = NDEF_T5T_ACCESS_ALWAYS;

    ctx->cc.t5t.specialFrame = false;
    ctx->cc.t5t.lockBlock    = false;
    ctx->cc.t5t.memoryLen    = 0U;
    ctx->cc.t5t.mlenOverflow = false;

    ctx->cc.t5t.memoryLen = (uint16_t)((ctx->subCtx.t5t.sysInfo.numberOfBlock * ctx->subCtx.t5t.sysInfo.blockSize) / NDEF_T5T_MLEN_DIVIDER);

    if ((options & NDEF_T5T_FORMAT_OPTION_NFC_FORUM) == NDEF_T5T_FORMAT_OPTION_NFC_FORUM) /* NFC Forum format */
    {
      if (ctx->cc.t5t.memoryLen >= NDEF_T5T_MAX_MLEN_1_BYTE_ENCODING)
      {
        ctx->cc.t5t.ccLen = NDEF_T5T_CC_LEN_8_BYTES;
      }
      if (ctx->cc.t5t.memoryLen > 0U)
      {
        ctx->cc.t5t.memoryLen--; /* remove CC area from memory length */
      }
    }
    else /* Android format */
    {
      ctx->cc.t5t.ccLen = NDEF_T5T_CC_LEN_4_BYTES;
      if (ctx->cc.t5t.memoryLen >= NDEF_T5T_MAX_MLEN_1_BYTE_ENCODING)
      {
        ctx->cc.t5t.mlenOverflow = true;
        ctx->cc.t5t.memoryLen    = 0xFFU;
      }
    }

    if (!ctx->subCtx.t5t.legacySTHighDensity && (ctx->subCtx.t5t.sysInfo.numberOfBlock > NDEF_T5T_MAX_BLOCK_1_BYTE_ADDR))
    {
      ctx->cc.t5t.magicNumber = NDEF_T5T_CC_MAGIC_2_BYTE_ADDR_MODE; /* E2 */
    }
  }

  result = ndefT5TagWriteCC(ctx);
  if (result != ERR_NONE)
  {
    /* If write fails, try to use special frame if not yet used */
    if (!ctx->cc.t5t.specialFrame)
    {
      HAL_Delay(20U); /* Wait to be sure that previous command has ended */
      ctx->cc.t5t.specialFrame = true; /* Add option flag */
      result = ndefT5TagWriteCC(ctx);
      if( result != ERR_NONE )
      {
        ctx->cc.t5t.specialFrame = false; /* Add option flag */
        return result;
      }
    }
    else
    {
      return result;
    }
  }

  /* Update info about current NDEF */

  ctx->subCtx.t5t.TlvNDEFOffset = ctx->cc.t5t.ccLen;

  result = NFCTAG_WriteData(NFC07A1_NFCTAG_INSTANCE, emptyNDEF, ctx->subCtx.t5t.TlvNDEFOffset, sizeof(emptyNDEF));
  if (result == ERR_NONE)
  {
    /* Update info about current NDEF */
    ctx->messageOffset = (uint32_t)ctx->cc.t5t.ccLen + NDEF_T5T_TLV_T_LEN + NDEF_T5T_TLV_L_1_BYTES_LEN;
    ctx->state         = NDEF_STATE_INITIALIZED;
  }
  return result;
}

/*******************************************************************************/
uint8_t ndefT5TagGetBlockLength(ndefContext *ctx)
{
  int32_t    result;
  uint8_t       blockLen = 0;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return 0;
  }

  result = NFCTAG_ReadRegister(NFC07A1_NFCTAG_INSTANCE, ctx->subCtx.t5t.txrxBuf, 0x0016, 1);

  if (result == BSP_ERROR_NONE)
  {
    blockLen = ctx->subCtx.t5t.txrxBuf[0] + 1;
  }

  return blockLen;
}

/*******************************************************************************/
static ndefStatus ndefT5TGetSystemInformation(ndefContext *ctx, bool extended)
{
  int32_t    result;
  uint8_t    rxBuf[NDEF_T5T_SYSINFO_MAX_LEN];
  uint8_t*   resp;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  result = NFCTAG_ReadRegister(NFC07A1_NFCTAG_INSTANCE, rxBuf, 0x0012, 14);
  if(result != NFCTAG_OK)
  {
    return ERR_SYSTEM;
  }

  resp = &rxBuf[0U];

  ctx->subCtx.t5t.sysInfo.DFSID = *resp;
  resp++;

  ctx->subCtx.t5t.sysInfo.AFI = *resp;
  resp++;

  ctx->subCtx.t5t.sysInfo.numberOfBlock = resp[1];
  ctx->subCtx.t5t.sysInfo.numberOfBlock = (ctx->subCtx.t5t.sysInfo.numberOfBlock << 8) | resp[0];
  resp+=2;
  ctx->subCtx.t5t.sysInfo.blockSize = *resp;
  resp++;
  ctx->subCtx.t5t.sysInfo.numberOfBlock++;
  ctx->subCtx.t5t.sysInfo.blockSize++;

  ctx->subCtx.t5t.sysInfo.ICRef = *resp;
  resp++;

  /* get UID */
  (void)ST_MEMCPY(ctx->subCtx.t5t.sysInfo.UID, resp, NDEF_NFCV_UID_LEN);

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TGetMemoryConfig(ndefContext *ctx)
{
  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  /* Extended Get System Info */
  if (ndefT5TGetSystemInformation(ctx, true) == ERR_NONE)
  {
    ctx->subCtx.t5t.sysInfoSupported = true;
  }

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagReadBytes(ndefContext *ctx, uint32_t offset, uint32_t len, uint8_t *buf)
{
  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T) || (buf == NULL))
  {
    return ERR_PARAM;
  }

  if (NFCTAG_ReadData(NFC07A1_NFCTAG_INSTANCE, buf, offset, len) != NFCTAG_OK)
  {
    return ERR_PROTO;
  }

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagReadRawMessage(ndefContext *ctx, uint8_t *buf, uint32_t bufLen)
{
  ndefStatus result;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T) || (buf == NULL))
  {
    return ERR_PARAM;
  }

  ndefT5TInvalidateCache(ctx);
  result = ndefT5TReadLField(ctx);
  if (result != ERR_NONE)
  {
    /* Conclude procedure */
    return result;
  }

  if (ctx->state <= NDEF_STATE_INITIALIZED)
  {
    /* Conclude procedure  */
    return ERR_WRONG_STATE;
  }

  if (ctx->messageLen > bufLen)
  {
    return ERR_NOMEM;
  }

  result = ndefT5TagReadBytes(ctx, ctx->messageOffset, ctx->messageLen, buf);
  if (result != ERR_NONE)
  {
    ctx->state = NDEF_STATE_INVALID;
  }
  return result;
}

/*******************************************************************************/
ndefStatus ndefT5TagWriteBytes(ndefContext *ctx, uint32_t offset, const uint8_t *buf, uint32_t len, bool pad, bool writeTerminator)
{
  bool            lvWriteTerminator = writeTerminator;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T) || (len == 0U) || (ctx->subCtx.t5t.blockLen == 0U))
  {
    return ERR_PARAM;
  }

  if (NFCTAG_WriteData(NFC07A1_NFCTAG_INSTANCE, buf, offset, len) != NFCTAG_OK)
  {
    return ERR_PROTO;
  }

  if (lvWriteTerminator)
  {
    ctx->subCtx.t5t.txrxBuf[0] = NDEF_TERMINATOR_TLV_T;
    if (NFCTAG_WriteData(NFC07A1_NFCTAG_INSTANCE, ctx->subCtx.t5t.txrxBuf, (offset + len), NDEF_TERMINATOR_TLV_LEN) != NFCTAG_OK)
    {
      return ERR_PROTO;
    }
  }

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagWriteRawMessageLen(ndefContext *ctx, uint32_t rawMessageLen, bool writeTerminator)
{
  uint8_t    TLV[8U];
  ndefStatus result;
  uint8_t    len;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  if ((ctx->state != NDEF_STATE_INITIALIZED) && (ctx->state != NDEF_STATE_READWRITE))
  {
    return ERR_WRONG_STATE;
  }

  if (writeTerminator && (rawMessageLen != 0U) && ((ctx->messageOffset + rawMessageLen) < ctx->areaLen))
  {
    /* Write T5T TLV terminator */
    len = 0U;
    TLV[len] = NDEF_TERMINATOR_TLV_T; /* TLV terminator */
    len++;
    result = ndefT5TagWriteBytes(ctx, ctx->messageOffset + rawMessageLen, TLV, len, true, false);
    if (result != ERR_NONE)
    {
      return result;
    }
  }

  /* Prepare L buffer content - Don't need to write T byte so skip it */
  len = 0U;
  if (rawMessageLen <= NDEF_SHORT_VFIELD_MAX_LEN)
  {
    TLV[len] = (uint8_t) rawMessageLen;
    len++;
  }
  else
  {
    TLV[len] = (uint8_t)(NDEF_SHORT_VFIELD_MAX_LEN + 1U);
    len++;
    TLV[len] = (uint8_t) (rawMessageLen >> 8U);
    len++;
    TLV[len] = (uint8_t) rawMessageLen;
    len++;
  }
  if (writeTerminator && (rawMessageLen == 0U))
  {
    TLV[len] = NDEF_TERMINATOR_TLV_T; /* TLV terminator */
    len++;
  }

  result = ndefT5TagWriteBytes(ctx, ctx->subCtx.t5t.TlvNDEFOffset + NDEF_T5T_TLV_T_LEN, TLV, len, writeTerminator && (rawMessageLen == 0U), false);

  return result;
}

/*******************************************************************************/
ndefStatus ndefT5TagWriteRawMessage(ndefContext *ctx, const uint8_t *buf, uint32_t bufLen)
{
  uint32_t   len = bufLen;
  ndefStatus result;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T) || (buf == NULL))
  {
    return ERR_PARAM;
  }

  /* TS T5T v1.0 7.5.3.1/2: T5T NDEF Detect should have been called before NDEF write procedure */
  /* Warning: current tag content must not be changed between NDEF Detect procedure and NDEF Write procedure*/

  /* TS T5T v1.0 7.5.3.3: check write access condition */
  if ((ctx->state != NDEF_STATE_INITIALIZED) && (ctx->state != NDEF_STATE_READWRITE))
  {
    /* Conclude procedure */
    return ERR_WRONG_STATE;
  }

  /* TS T5T v1.0 7.5.3.3: verify available space */
  result = ndefT5TagCheckAvailableSpace(ctx, bufLen);
  if (result != ERR_NONE)
  {
    /* Conclude procedure */
    return ERR_PARAM;
  }
  /* TS T5T v1.0 7.5.3.4: reset L-Field to 0 */
  /* and update ctx->messageOffset according to L-field len */
  result = ndefT5TagBeginWriteMessage(ctx, bufLen);
  if (result != ERR_NONE)
  {
    ctx->state = NDEF_STATE_INVALID;
    /* Conclude procedure */
    return result;
  }
  if (bufLen != 0U)
  {
    /* TS T5T v1.0 7.5.3.5: write new NDEF message and write terminator TLV is enough space for it*/
    result = ndefT5TagWriteBytes(ctx, ctx->messageOffset, buf, len, true, ndefT5TagCheckAvailableSpace(ctx, bufLen + 1U) == ERR_NONE);
    if (result != ERR_NONE)
    {
      /* Conclude procedure */
      ctx->state = NDEF_STATE_INVALID;
      return result;
    }
    /* TS T5T v1.0 7.5.3.6 & 7.5.3.7: update L-Field and write Terminator TLV */
    result = ndefT5TagEndWriteMessage(ctx, len, false);
    if (result != ERR_NONE)
    {
      /* Conclude procedure */
      ctx->state = NDEF_STATE_INVALID;
      return result;
    }
  }
  return result;
}

/*******************************************************************************/
ndefStatus ndefT5TagCheckAvailableSpace(const ndefContext *ctx, uint32_t messageLen)
{
  uint32_t lLen;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  if (ctx->state == NDEF_STATE_INVALID)
  {
    return ERR_WRONG_STATE;
  }

  lLen = ( messageLen > NDEF_SHORT_VFIELD_MAX_LEN) ? NDEF_T5T_TLV_L_3_BYTES_LEN : NDEF_T5T_TLV_L_1_BYTES_LEN;

  if ((messageLen + ctx->subCtx.t5t.TlvNDEFOffset + NDEF_T5T_TLV_T_LEN + lLen) > (ctx->areaLen + ctx->cc.t5t.ccLen))
  {
    return ERR_NOMEM;
  }
  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagBeginWriteMessage(ndefContext *ctx, uint32_t messageLen)
{
  ndefStatus ret;
  uint32_t   lLen;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  if ((ctx->state != NDEF_STATE_INITIALIZED) && (ctx->state != NDEF_STATE_READWRITE))
  {
    return ERR_WRONG_STATE;
  }

  /* TS T5T v1.0 7.5.3.4: reset L-Field to 0 */
  ret = ndefT5TagWriteRawMessageLen(ctx, 0U, true);
  if (ret != ERR_NONE)
  {
    /* Conclude procedure */
    ctx->state = NDEF_STATE_INVALID;
    return ret;
  }

  lLen                = (messageLen > NDEF_SHORT_VFIELD_MAX_LEN) ? NDEF_T5T_TLV_L_3_BYTES_LEN : NDEF_T5T_TLV_L_1_BYTES_LEN;
  ctx->messageOffset  = ctx->subCtx.t5t.TlvNDEFOffset;
  ctx->messageOffset += NDEF_T5T_TLV_T_LEN; /* T Length */
  ctx->messageOffset += lLen;               /* L Length */
  ctx->state          = NDEF_STATE_INITIALIZED;

  return ERR_NONE;
}

/*******************************************************************************/
ndefStatus ndefT5TagEndWriteMessage(ndefContext *ctx, uint32_t messageLen, bool writeTerminator)
{
  ndefStatus ret;

  if ((ctx == NULL) || (ctx->type != NDEF_DEV_T5T))
  {
    return ERR_PARAM;
  }

  if (ctx->state != NDEF_STATE_INITIALIZED)
  {
    return ERR_WRONG_STATE;
  }

  /* TS T5T v1.0 7.5.3.6 & 7.5.3.7: update L-Field and write Terminator TLV */
  ret = ndefT5TagWriteRawMessageLen(ctx, messageLen, writeTerminator);
  if (ret != ERR_NONE)
  {
    /* Conclude procedure */
    ctx->state = NDEF_STATE_INVALID;
    return ret;
  }
  ctx->messageLen = messageLen;
  ctx->state      = (ctx->messageLen == 0U) ? NDEF_STATE_INITIALIZED : NDEF_STATE_READWRITE;
  return ERR_NONE;
}

