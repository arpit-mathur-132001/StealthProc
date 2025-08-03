/********************************************************************************
*																				*
* File Name:																	*
* 	StealthProcClient.cpp														*
*																				*
* Abstract:																		*
* 	This file is the main file for the StealthProcClient.						*
* 	It implements the functionality to talk to driver.      					*
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

#include <iostream>
#include <Windows.h>

#include "..\Common\common.h"

/////////////////////////////////////////////////////////////////////
//  N A M E S P A C E S.
/////////////////////////////////////////////////////////////////////
using namespace std;


/////////////////////////////////////////////////////////////////////
//	M A C R O S.
/////////////////////////////////////////////////////////////////////
#define DBG_ALL TRUE
#define DBG_ERR TRUE


/////////////////////////////////////////////////////////////////////
//	F U N C T I O N  D E F I N I T I O N S.
/////////////////////////////////////////////////////////////////////

//***********************************************************************************
// 
//	Function:
//		main.
//
//	Parameters:
//      [IN]  int argc
//      Number of command-line arguments.
// 
//      [IN]  char* argv[]
//      Array of command-line argument strings.
//
//	Routine Description:
//      Entry point for StealthProcClient; sends process name to driver for hiding.
//
//	Return Value:
//		0 on success.
//      1 on error.
//
//***********************************************************************************
int 
main(
    IN int argc, 
    IN char* argv[]
)
{
    #ifdef DBG_ALL
    cout << "main: Entry" << endl;
    #endif

    if (argc != 2) 
    {
        #ifdef DBG_ERR
        cerr << "Usage: " << argv[0] << " <process_name>" << endl;
        #endif

        return 1;
    }

    string szProcName = argv[1];

    //
    //  Open a handle to the device.
    //
    HANDLE hDevice = CreateFile(
        L"\\\\.\\StealthProcUsr", 
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, 
        OPEN_EXISTING, 
        0, 
        NULL
    );
    if (hDevice == INVALID_HANDLE_VALUE || hDevice == NULL)
    {
        #ifdef DBG_ERR
        cerr << "main: Failed to open device, error: " << GetLastError() << endl;
        #endif

        return 1;
    }

    //
    //  Call DeviceIoControl with process name to hide.
    //
    DWORD dwBytesReturned;
    BOOL bResult = DeviceIoControl(hDevice, IOCTL_GET_PROCESSNAME, (LPVOID)szProcName.c_str(), (DWORD)(szProcName.length() + 1), NULL, 0, &dwBytesReturned, NULL);

    if (bResult)
    {
        #ifdef DBG_ALL
        cout << "main: Process name sent successfully: " << szProcName << endl;
        #endif
    }
    else
    {
        #ifdef DBG_ERR
        cerr << "main: Failed to send process name. Error: " << GetLastError() << endl;
        #endif

        return 1;
    }

    #ifdef DBG_ALL
    cout << "main: got the handle to StealthProcUsr" << endl;

    cout << "main: Exit" << endl;
    #endif

    return 0;
}