/********************************************************************************
*																				*
* File Name:																	*
* 	StealthProc.h																*
*																				*
* Abstract:																		*
* 	This file is the main file for the driver.									*
* 	It implements the driver load/unload routines.								*
*																				*
* Revision History:																*
* 	Date:	02 August 2025														*
* 	Author: Arpit Mathur														*
* 	Desc:	Created																*
*																				*
********************************************************************************/


/////////////////////////////////////////////////////////////////////
//	P R A G M A S.
/////////////////////////////////////////////////////////////////////
#pragma once


/////////////////////////////////////////////////////////////////////
//	E X T E R N S.
/////////////////////////////////////////////////////////////////////
extern ULONG ulPidOffset, ulNameOffset, ulListEntryOffset;


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N  P R O T O T Y P E S.
/////////////////////////////////////////////////////////////////////

//*******************************************************************
//
//	Function:
//		DriverEntry.
//
//	Parameters:
//		[IN] PDRIVER_OBJECT pDriverObject.
//		Driver object.
//
//		[IN] PUNICODE_STRING pusRegistryPath.
//		Registry path which stores the driver configurations.
//
//	Description:
//		This function is the initialization routine of the driver.
//
//	Returns:
//		NTSTATUS.
//		If the function succeeds the return value is STATUS_SUCCESS.
//		Else the return value is some error status.
//
//*******************************************************************
NTSTATUS
DriverEntry(
	IN  PDRIVER_OBJECT pDriverObject,
	IN  PUNICODE_STRING pusRegistryPath
);


//*******************************************************************
//
//	Function:
//		DriverUnload.
//
//	Parameters:
//		[IN] PDRIVER_OBJECT pDriverObject.
//		pDriverObject for unload operation.
//
//	Description:
//		This function is the unload routine of the driver.
//
//	Returns:
//		NTSTATUS.
//		If the function succeeds the return value is STATUS_SUCCESS.
//		Else the return value is some error status.
//
//*******************************************************************
VOID
DriverUnload(
	IN  PDRIVER_OBJECT pDriverObject
);