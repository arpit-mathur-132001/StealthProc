/********************************************************************************
*																				*
* File Name:																	*
* 	6fings.h																	*
*																				*
* Abstract:																		*
* 	This file is the main file for the driver.									*
* 	It implements the driver load/unload routines.								*
*																				*
* Revision History:																*
* 	Date:	13 December 2024													*
* 	Author: Arpit Mathur														*
* 	Desc:	Created																*
*																				*
********************************************************************************/


/////////////////////////////////////////////////////////////////////
//	P R A G M A S.
/////////////////////////////////////////////////////////////////////
#pragma once


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N  P R O T O T Y P E S.
/////////////////////////////////////////////////////////////////////

//***********************************************************************************
// 
//	Function:
//		DispatchCreate
//
//	Parameters:
//		[IN]  DEVICE_OBJECT* pDeviceObject
//		Our device object.
//	   
//		[IN/OUT]  IRP* pIrp
//		The IO request packet to process.
//
//	Routine Description:
//		Create dispatch routine. All the requests are completed
//		successfuly.
//
//	Return Value:
//		STATUS_SUCCESS.
//
//***********************************************************************************
NTSTATUS
DispatchCreate(
	IN  PDEVICE_OBJECT pDeviceObject,
	IN OUT  PIRP pIrp
);


//***********************************************************************************
// 
//	Function:
//		DispatchDeviceControl
//
//	Parameters:
//		[IN]  DEVICE_OBJECT* pDeviceObject
//		Our device object.
//	   
//		[IN/OUT]  PIRP pIrp
//		The IO request packet to process.
//
//	Routine Description:
//		Create dispatch routine. All the requests are completed
//		successfuly.
//
//	Return Value:
//		STATUS_SUCCESS.
//
//***********************************************************************************
NTSTATUS
DispatchDeviceControl(
	IN  PDEVICE_OBJECT pDeviceObject,
	IN OUT  PIRP pIrp
);