// ==============================================================
// Copyright (c) 1986 - 2020 Xilinx Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// ==============================================================

#ifndef XV_DEMOSAIC_H
#define XV_DEMOSAIC_H

#define XPAR_XV_DEMOSAIC_NUM_INSTANCES 1

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xv_demosaic_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
/**
* This typedef contains configuration information for the demosaic core
* Each core instance should have a configuration structure associated.
*/
typedef struct {
  u16 DeviceId;             /**< Unique ID of device */
  UINTPTR BaseAddress;      /**< The base address of the core instance. */
  u16 PixPerClk;            /**< Samples Per Clock */
  u16 MaxWidth;             /**< Maximum columns supported by core instance */
  u16 MaxHeight;            /**< Maximum rows supported by core instance */
  u16 MaxDataWidth;         /**< Maximum Data width of each channel */
  u16 Algorithm;            /**< Interpolation method */
} XV_demosaic_Config;
#endif

/**
* Driver instance data. An instance must be allocated for each core in use.
*/
typedef struct {
    XV_demosaic_Config Config;    /**< Hardware Configuration */
    u32 IsReady;                  /**< Device is initialized and ready */
} XV_demosaic;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XV_demosaic_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XV_demosaic_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XV_demosaic_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XV_demosaic_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XV_demosaic_Initialize(XV_demosaic *InstancePtr, u16 DeviceId);
XV_demosaic_Config* XV_demosaic_LookupConfig(u16 DeviceId);
int XV_demosaic_CfgInitialize(XV_demosaic *InstancePtr,
                              XV_demosaic_Config *ConfigPtr,
                              UINTPTR EffectiveAddr);
#else
int XV_demosaic_Initialize(XV_demosaic *InstancePtr, const char* InstanceName);
int XV_demosaic_Release(XV_demosaic *InstancePtr);
#endif

void XV_demosaic_Start(XV_demosaic *InstancePtr);
u32 XV_demosaic_IsDone(XV_demosaic *InstancePtr);
u32 XV_demosaic_IsIdle(XV_demosaic *InstancePtr);
u32 XV_demosaic_IsReady(XV_demosaic *InstancePtr);
void XV_demosaic_EnableAutoRestart(XV_demosaic *InstancePtr);
void XV_demosaic_DisableAutoRestart(XV_demosaic *InstancePtr);

void XV_demosaic_Set_HwReg_width(XV_demosaic *InstancePtr, u32 Data);
u32 XV_demosaic_Get_HwReg_width(XV_demosaic *InstancePtr);
void XV_demosaic_Set_HwReg_height(XV_demosaic *InstancePtr, u32 Data);
u32 XV_demosaic_Get_HwReg_height(XV_demosaic *InstancePtr);
void XV_demosaic_Set_HwReg_bayer_phase(XV_demosaic *InstancePtr, u32 Data);
u32 XV_demosaic_Get_HwReg_bayer_phase(XV_demosaic *InstancePtr);

void XV_demosaic_InterruptGlobalEnable(XV_demosaic *InstancePtr);
void XV_demosaic_InterruptGlobalDisable(XV_demosaic *InstancePtr);
void XV_demosaic_InterruptEnable(XV_demosaic *InstancePtr, u32 Mask);
void XV_demosaic_InterruptDisable(XV_demosaic *InstancePtr, u32 Mask);
void XV_demosaic_InterruptClear(XV_demosaic *InstancePtr, u32 Mask);
u32 XV_demosaic_InterruptGetEnabled(XV_demosaic *InstancePtr);
u32 XV_demosaic_InterruptGetStatus(XV_demosaic *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
