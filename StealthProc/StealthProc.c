/********************************************************************************
*																				*
* File Name:																	*
* 	StealthProc.c																*
*																				*
* Abstract:																		*
* 	This file is the main file for the driver.									*
* 	It implements the driver load/unload routines.								*
*																				*
* Revision History:																*
* 	Date:	28 March 2025														*
* 	Author: Arpit Mathur														*
* 	Desc:	Created																*
*																				*
********************************************************************************/


/////////////////////////////////////////////////////////////////////
//	H E A D E R S.
/////////////////////////////////////////////////////////////////////
#include "precomp.h"


/////////////////////////////////////////////////////////////////////
//	P R A G M A S.
/////////////////////////////////////////////////////////////////////

//
//	These compiler directives tell the Operating System how to load the
//	driver into memory. The "INIT" section is discardable as we only
//	need the DriverEntry upon initialization, then it can be discarded.
//	"PAGE" directive tells that the code can be paged out to disk when not in use, saving non-paged memory.
//  This directive places the Unload function in the PAGE section of the driver's memory.
// 
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DriverUnload)


/////////////////////////////////////////////////////////////////////
//	G L O B A L S.
/////////////////////////////////////////////////////////////////////

ULONG ulPidOffset = 0, ulNameOffset = 0, ulListEntryOffset = 0;


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N  D E F I N I T I O N S.
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
)
{
	UNREFERENCED_PARAMETER(pusRegistryPath);

	NTSTATUS NtStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject;
	UNICODE_STRING usDriverName, usDosDeviceName;

	#ifdef DBG_ALL_DRIVERENTRY
	DbgPrint("DriverEntry: Entry\r\n");
	#endif

	//
	//	Initialize Driver name.
	//
	RtlInitUnicodeString(
		&usDriverName,
		L"\\Device\\StealthProc"
	);

	//
	//	Initialize DosDevice name.
	//
	RtlInitUnicodeString(
		&usDosDeviceName,
		L"\\DosDevices\\StealthProcUsr"
	);

	//
	//	Create a device object.
	//
	NtStatus = IoCreateDevice(
		pDriverObject,
		0,
		&usDriverName,
		FILE_DEVICE_UNKNOWN, // This device does not correspond to any of the predefined types.
		FILE_DEVICE_SECURE_OPEN, // Perform security checks on device name during open request.
		FALSE,
		&pDeviceObject
	);

	if (STATUS_SUCCESS == NtStatus)
	{
		//
		//	The "MajorFunction" is a list of function pointers for entry points into the driver.
		// 
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;

		//
		//	Required to unload the driver dynamically. 
		//	If this function is missing the driver cannot be dynamically unloaded.
		// 
		pDriverObject->DriverUnload = DriverUnload;

		//
		//	We are not required to clear this flag in the DriverEntry as the I/O Manager will
		//	clear it for us, but we will anyway. Creating a device in any other location we
		//	would need to clear it.
		//
		pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

		//		
		//	Create a Symbolic Link to the device. StealthProcUsr -> \Device\StealthProc
		//
		NtStatus = IoCreateSymbolicLink(
			&usDosDeviceName,
			&usDriverName
		);

		//
		//	Initialize necessary offsets in the EPROCESS structure.
		//
		if (!InitializeOffsets())
		{
			#ifdef DBG_ERR_DRIVERENTRY
			DbgPrint("DriverEntry: Failed to initialize offsets.\r\n");
			#endif

			return STATUS_UNSUCCESSFUL;
		}
	}

	#ifdef DBG_ALL_DRIVERENTRY
	DbgPrint("DriverEntry: Exit\r\n");
	#endif

	return NtStatus;
}


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
)
{
	UNICODE_STRING usDosDeviceName;
	NTSTATUS NtStatus = STATUS_SUCCESS;

	#ifdef DBG_ALL_DRIVERUNLOAD
	DbgPrint("DriverUnload: Entry\r\n");
	#endif

	RtlInitUnicodeString(
		&usDosDeviceName,
		L"\\DosDevices\\StealthProcUsr"
	);

	NtStatus = IoDeleteSymbolicLink(&usDosDeviceName);
	if (STATUS_SUCCESS != NtStatus)
	{
		#ifdef DBG_ERR_DRIVERUNLOAD
		DbgPrint("DriverUnload: IoDeleteSymbolicLink() failed with NtStatus(0x%08X)\r\n", NtStatus);
		#endif
	}

	IoDeleteDevice(pDriverObject->DeviceObject);

	#ifdef DBG_ALL_DRIVERUNLOAD
	DbgPrint("DriverUnload: Exit\r\n");
	#endif
}