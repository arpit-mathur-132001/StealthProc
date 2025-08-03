/********************************************************************************
*																				*
* File Name:																	*
* 	functions.c																	*
*																				*
* Abstract:																		*
* 	This file contains definitions of all the dispatch routines.				*
* 	It also contains definitions of generic functions.							*
*																				*
* Revision History:																*
* 	Date:	02 August 2025														*
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
//	These are compiler directive which specifies 
//  whether the code of the function can be paged out of memory or not.
// 
#pragma alloc_text(PAGE, DispatchCreate)
#pragma alloc_text(PAGE, DispatchDeviceControl)


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N  P R O T O T Y P E S.
/////////////////////////////////////////////////////////////////////

//***********************************************************************************
// 
//	Function:
//		DispatchCreate
//
//	Parameters:
//		[IN]  PDEVICE_OBJECT pDeviceObject
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
)
{
    UNREFERENCED_PARAMETER(pDeviceObject);
    UNREFERENCED_PARAMETER(pIrp);

    NTSTATUS NtStatus = STATUS_SUCCESS;

    #ifdef DBG_ALL_DISPATCHCREATE
    DbgPrint("DispatchCreate: Entry\r\n");
    #endif

    #ifdef DBG_ALL_DISPATCHCREATE
    DbgPrint("DispatchCreate: Exit\r\n");
    #endif

    return NtStatus;
}


//***********************************************************************************
// 
//	Function:
//		DispatchDeviceControl
//
//	Parameters:
//		[IN]  PDEVICE_OBJECT pDeviceObject
//		Our device object.
//	   
//		[IN/OUT]  PIRP pIrp
//		The IO request packet to process.
//
//	Routine Description:
//	    Handles device control requests, processes IOCTLs, and invokes HideProcess 
//      for process name input.
//
//	Return Value:
//		STATUS_SUCCESS.
//		If the function succeeds the return value is STATUS_SUCCESS.
//		Else the return value is some error status.
//
//***********************************************************************************
NTSTATUS 
DispatchDeviceControl(
    IN  PDEVICE_OBJECT pDeviceObject,
    IN OUT  PIRP pIrp
) 
{
    UNREFERENCED_PARAMETER(pDeviceObject);

    //
    //  Get current IRP stack location to access input buffer, its length and IOCTL code.
    //
    PIO_STACK_LOCATION pIoStackIrp = IoGetCurrentIrpStackLocation(pIrp);

    ULONG ulBytesProcessed = 0;
    NTSTATUS NtStatus = STATUS_SUCCESS;
    char* pszInputBuffer = NULL;
    ULONG ulInputBufferLength = pIoStackIrp->Parameters.DeviceIoControl.InputBufferLength;

    #ifdef DBG_ALL_DISPATCHDEVICECONTROL
    DbgPrint("DispatchDeviceControl: Entry\r\n");
    #endif

    switch (pIoStackIrp->Parameters.DeviceIoControl.IoControlCode) 
    {
        case IOCTL_GET_PROCESSNAME:

            //
            //  Ensure the buffer size is valid.
            //
            if (ulInputBufferLength == 0 || ulInputBufferLength > PAGE_SIZE) 
            {
                NtStatus = STATUS_INVALID_BUFFER_SIZE;
                
                #ifdef DBG_ERR_DISPATCHDEVICECONTROL
                DbgPrint("DispatchDeviceControl: Invalid buffer size\r\n");
                #endif

                break;
            }

            //
            //  Ensure SystemBuffer is accessible.
            //
            pszInputBuffer = (char*)pIrp->AssociatedIrp.SystemBuffer;
            if (pszInputBuffer == NULL) 
            {
                NtStatus = STATUS_INVALID_PARAMETER;
                
                #ifdef DBG_ERR_DISPATCHDEVICECONTROL
                DbgPrint("DispatchDeviceControl: Invalid input buffer\r\n");
                #endif

                break;
            }

            //
            //  Ensure safe access to the buffer
            //
            __try 
            {
                #ifdef DBG_ALL_DISPATCHDEVICECONTROL
                DbgPrint("DispatchDeviceControl: Received process name: %s\r\n", pszInputBuffer);
                #endif

                //
                //  Call HideProcess with the process name
                //
                HideProcess(pszInputBuffer);
                
                ulBytesProcessed = ulInputBufferLength;
                NtStatus = STATUS_SUCCESS;

            }
            __except (EXCEPTION_EXECUTE_HANDLER) 
            {
                NtStatus = GetExceptionCode();
                
                #ifdef DBG_ERR_DISPATCHDEVICECONTROL
                DbgPrint("DispatchDeviceControl: Exception occurred: 0x%X\r\n", NtStatus);
                #endif
            }

            break;

        default:

            NtStatus = STATUS_INVALID_DEVICE_REQUEST;
            break;
    }

    pIrp->IoStatus.Status = NtStatus;
    pIrp->IoStatus.Information = ulBytesProcessed; // Number of bytes processed
    
    //
    //  Complete IRP to indicate that processing is finished.
    //
    IoCompleteRequest(
        pIrp, 
        IO_NO_INCREMENT // No priority boost.
    );

    #ifdef DBG_ALL_DISPATCHDEVICECONTROL
    DbgPrint("DispatchDeviceControl: Exit\r\n");
    #endif

    return NtStatus;
}