/*
 * Copyright (c) 2018, Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file Types.h
 * @brief Types for EFI_NVMDIMMS_CONFIG_PROTOCOL to configure and manage DCPMMs. These types don't compile with VFR compiler and are kept separate.
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include "NvmTypes.h"
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>

#ifdef __GNUC__
 // Need this to build for UEFI/Linux, as size_t is not found by default.
#include <stddef.h>
#endif

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define PMEM_MODULE_NAME L"Intel(R) DCPM "  //short version for drivers list
#define PMEM_MODULE_NAME_SEARCH L"Intel(R),DCPM" //comma separated search string

/*This should match the error_type definition in nvm_management.h*/
#define ERROR_INJ_POISON                0X01
#define ERROR_INJ_TEMPERATURE           0X02
#define ERROR_INJ_PACKAGE_SPARING       0X03
#define ERROR_INJ_PERCENTAGE_REMAINING  0X04
#define ERROR_INJ_FATAL_MEDIA_ERR       0X05
#define ERROR_INJ_DIRTY_SHUTDOWN        0X06
#define ERROR_INJ_TYPE_INVALID          0x08

/**
  The device path type for our driver and HII driver.
**/
typedef struct {
  VENDOR_DEVICE_PATH VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL End;
} VENDOR_END_DEVICE_PATH;

#define PMEMDEV_INITIALIZER(DEV) \
      InitializeListHead(DEV.Dimms); \
      InitializeListHead(DEV.UninitializedDimms); \
      InitializeListHead(DEV.ISs); \
      InitializeListHead(DEV.Namespaces);

#define DIMM_BSR_MAJOR_NO_POST_CODE 0x0
#define DIMM_BSR_MAJOR_CHECKPOINT_INIT_FAILURE 0xA1 // Unrecoverable FW error
#define DIMM_BSR_MAJOR_CHECKPOINT_CPU_EXCEPTION 0xE1 // CPU exception
#define DIMM_BSR_MAJOR_CHECKPOINT_INIT_COMPLETE 0xF0 // FW initialization complete
#define DIMM_BSR_MAILBOX_READY  0x1
#define DIMM_BSR_MAILBOX_NOT_READY 0x0
#define DIMM_BSR_MEDIA_NOT_TRAINED 0x0
#define DIMM_BSR_MEDIA_TRAINED 0x1
#define DIMM_BSR_MEDIA_ERROR 0x2
#define DIMM_BSR_MEDIA_DISABLED 0x1

// @todo Remove FIS 1.4 backwards compatibility workaround
// FIS <= 1.4
#define DIMM_BSR_AIT_DRAM_READY 0x1

// FIS >= 1.5
#define DIMM_BSR_AIT_DRAM_NOTTRAINED 0x0
#define DIMM_BSR_AIT_DRAM_TRAINED_NOTLOADED 0x1
#define DIMM_BSR_AIT_DRAM_ERROR 0x2
#define DIMM_BSR_AIT_DRAM_TRAINED_LOADED_READY 0x3

#define DIMM_BSR_OIE_ENABLED 0x1
#define DIMM_BSR_FW_ASSERT 0x1
#define DIMM_BSR_REBOOT_REQUIRED 0x1
#define DIMM_BSR_MEDIA_INTERFACE_ENGINE_STALLED 0x01

#define REGISTER_BSR_STR  L"BSR"
#define REGISTER_OS_STR   L"OS"

// @todo Remove FIS 1.4 backwards compatibility workaround
typedef union {
  UINT64 AsUint64;
  struct {
    UINT64 Major : 8;
    UINT64 Minor : 8;
    UINT64 MR : 2;
    UINT64 DT : 1;
    UINT64 PCR : 1;
    UINT64 MBR : 1;
    UINT64 WTS : 1;
    UINT64 FRCF : 1;
    UINT64 CR : 1;
    UINT64 MD: 1;
    UINT64 OIE: 1;
    UINT64 OIWE: 1;
    UINT64 Rsvd : 5;
    UINT64 Assertion : 1;
    UINT64 MI_Stalled: 1;
    UINT64 DR: 1;
    UINT64 Rsvd1 : 29;
  } Separated_FIS_1_4;
  struct {
    UINT64 Major : 8;       //7:0
    UINT64 Minor : 8;       //15:8
    UINT64 MR : 2;          //17:16
    UINT64 DT : 1;          //18
    UINT64 PCR : 1;         //19
    UINT64 MBR : 1;         //20
    UINT64 WTS : 1;         //21
    UINT64 FRCF : 1;        //22
    UINT64 CR : 1;          //23
    UINT64 MD: 1;           //24
    UINT64 OIE: 1;          //25
    UINT64 OIWE: 1;         //26
    UINT64 DR: 2;           //28:27
    UINT64 RR: 1;           //29
    UINT64 Rsvd: 34;        //63:30
  } Separated_Current_FIS;
} DIMM_BSR;

/**
  Contains SMART and Health attributes of a DIMM
**/
typedef struct _SENSOR_INFO {
  BOOLEAN PercentageRemainingValid;     ///< Indicates if PercentageRemaining is valid
  BOOLEAN MediaTemperatureValid;        ///< Indicates if MediaTemperature is valid
  BOOLEAN ControllerTemperatureValid;   ///< Indicates if ControllerTemperature is valid
  BOOLEAN MediaTemperatureTrip;         ///< Indicates if Media Temperature alarm threshold has tripped
  BOOLEAN ControllerTemperatureTrip;    ///< Indicates if Controller Temperature alarm threshold has tripped
  BOOLEAN PercentageRemainingTrip;      ///< Indicates if Percentage Remaining alarm threshold has tripped
  INT16 MediaTemperature;               ///< Current Media Temperature in C
  INT16 ControllerTemperature;          ///< Current Controller Temperature in C
  UINT8 PercentageRemaining;            ///< Remaining module's life as a percentage value of factory expected life span (0-100)
  UINT32 DirtyShutdowns;                ///< Dirty Shutdowns count
  UINT8 LastShutdownStatus;             ///< Last Shutdown Status. See FIS field LSS for additional details
  UINT32 PowerOnTime;                   ///< Lifetime DIMM has been powered on in seconds. See FIS field POT for additional details
  UINT32 UpTime;                        ///< DIMM uptime in seconds since last AC cycle. See FIS field UT for additional details
  UINT64 PowerCycles;                   ///< Number of DIMM power cycles. See FIS field PC for additional details
  UINT8 HealthStatus;                   ///< Overall health summary as specified by @ref HEALTH_STATUS. See FIS field HS for additional details.
  UINT16 HealthStatusReason;            ///< Indicates why the module is in the current HealthStatus as specified by @ref HEALTH_STATUS_REASONS. See FIS field HSR for additional details.
  UINT32 MediaErrorCount;               ///< Total count of media errors found in Error Log
  UINT32 ThermalErrorCount;             ///< Total count of thermal errors found in Error Log
  INT16 ContrTempShutdownThresh;        ///< Controller temperature shutdown threshold in C
  INT16 MediaTempShutdownThresh;        ///< Media temperature shutdown threshold in C
  INT16 MediaThrottlingStartThresh;     ///< Media throttling start temperature threshold in C
  INT16 MediaThrottlingStopThresh;      ///< Media throttling stop temperature threshold in C
  INT16 ControllerThrottlingStartThresh;///< Controller throttling stop temperature threshold in C
  INT16 ControllerThrottlingStopThresh; ///< Controller throttling stop temperature threshold in C
  UINT32 UnlatchedDirtyShutdowns;       ///< Unlatched Dirty Shutdowns count
} SENSOR_INFO;

/**
  Individual sensor attributes struct
**/
typedef struct {
  UINT8 Type;
  UINT8 State;
  UINT8 Enabled;
  INT64 Value;
  UINT8 SettableThresholds;
  UINT8 SupportedThresholds;
  INT64 NonCriticalThreshold;
  INT64 CriticalLowerThreshold;
  INT64 CriticalUpperThreshold;
  INT64 FatalThreshold;
} DIMM_SENSOR;

/**
  Thermal error log info struct
**/
typedef struct _THERMAL_ERROR_LOG_PER_DIMM_INFO {
  INT16   Temperature;        //!< In celsius
  UINT8   Reported;           //!< Temperature being reported
  UINT8   Type;               //!< Which device the temperature is for
  UINT16  SequenceNum;        //!< Log entry sequence number
  UINT8   Reserved[1];
} THERMAL_ERROR_LOG_INFO;

/**
  Media error log info struct
**/
typedef struct _MEDIA_ERROR_LOG_PER_DIMM_INFO {
  UINT64  Dpa;                //!< Specifies DPA address of error
  UINT64  Pda;                //!< Specifies PDA address of the failure
  UINT8   Range;              //!< Specifies the length in address space of this error.
  UINT8   ErrorType;          //!< Indicates what kind of error was logged. See @ref ERROR_LOG_TYPES.
  UINT8   PdaValid;           //!< Indicates the PDA address is valid.
  UINT8   DpaValid;           //!< Indicates the DPA address is valid.
  UINT8   Interrupt;          //!< Indicates this error generated an interrupt packet
  UINT8   Viral;              //!< Indicates Viral was signaled for this error
  UINT8   TransactionType;    //!< Transaction type
  UINT16  SequenceNum;        //!< Log entry sequence number
  UINT8   Reserved[2];
} MEDIA_ERROR_LOG_INFO;

/**
* @defgroup ERROR_LOG_TYPES Error Log Types
  * @{
  */

#define THERMAL_ERROR   0   ///< Thermal error log type
#define MEDIA_ERROR     1   ///< Media error log type

  /** @} */

#endif /** _TYPES_H_ **/
