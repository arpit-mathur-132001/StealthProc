/********************************************************************************
*																				*
* File Name:																	*
* 	StealthProcSrvc.cpp															*
*																				*
* Abstract:																		*
* 	This file is the main file to create, start and stop service.				*
* 	Service loads/unloads the driver.											*
*																				*
* Revision History:																*
* 	Date:	03 August 2024														*
* 	Author: Arpit Mathur														*
* 	Desc:	Created																*
*																				*
********************************************************************************/


/////////////////////////////////////////////////////////////////////
//	H E A D E R S.
/////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <iostream>
#include <tchar.h>


/////////////////////////////////////////////////////////////////////
//  N A M E S P A C E S.
/////////////////////////////////////////////////////////////////////
using namespace std;


/////////////////////////////////////////////////////////////////////
//	M A C R O S.
/////////////////////////////////////////////////////////////////////

//#define DBG_ALL TRUE
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
//		None.
//
//	Routine Description:
//		Main entry point for the service application. 
//		Creates, starts, and stops the driver service.
//
//	Return Value:
//		0.
//
//***********************************************************************************
int main()
{
	SC_HANDLE hSCManager = NULL;
	SC_HANDLE hService = NULL;
	SERVICE_STATUS ss;
	BOOL bIsServiceStarted = FALSE, bIsServiceHandleClosed = FALSE, bIsServiceDeleted = FALSE, bIsServiceStopped = FALSE;

	#ifdef DBG_ALL
	cout << "main: Entry" << endl;
	#endif

	//
	//	Opens connection to SCM on the local machine.
	//
	hSCManager = OpenSCManager(
		NULL,
		NULL,
		SC_MANAGER_CREATE_SERVICE // Allows program to create and modify services.
	);

	if (hSCManager)
	{
		#ifdef DBG_ALL
		cout << "main: Got the handle to SCM" << endl;
		#endif	

		//
		//	Create and add service in SCM database.
		//
		hService = CreateService(
			hSCManager,
			_T("StealthProc"),
			_T("StealthProc Service"),
			SERVICE_START | DELETE | SERVICE_STOP, // Grants permission to start, delete and stop the service.
			SERVICE_KERNEL_DRIVER, // Service is intended to be a KM driver.
			SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE,
			_T("C:\\Windows\\System32\\drivers\\StealthProc.sys"),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);

		if (!hService)
		{
			#ifdef DBG_ALL
			cout << "main: Service already exists, try opening the service" << endl;
			#endif
			
			//
			//	Open the service.
			//
			hService = OpenService(
				hSCManager,
				_T("StealthProc"),
				SERVICE_START | DELETE | SERVICE_STOP
			);

			if (!hService)
			{
				//
				//	OpenService failed.
				//
				#ifdef DBG_ERR
				cerr << "main: Neither service exists nor service created. Error " << GetLastError() << endl;
				#endif

				goto Cleanup;
			}
		}

		if (hService)
		{
			#ifdef DBG_ALL
			cout << "main: Got the handle of the service" << endl;
			#endif

			//
			//	Start the service.
			//
			bIsServiceStarted = StartService(
				hService,
				0,
				NULL
			);

			if (FALSE == bIsServiceStarted)
			{
				//
				//	StartService failed.
				//
				#ifdef DBG_ERR
				cerr << "main: StartService failed. Error" << GetLastError() << endl;
				#endif

				goto Cleanup;
			}

			cout << "Press enter to stop and delete the service" << endl;
			int temp = getchar();

			//
			//	Send control code to stop the service.
			//
			bIsServiceStopped = ControlService(
				hService,
				SERVICE_CONTROL_STOP,
				&ss
			);

			if (FALSE == bIsServiceStopped)
			{
				#ifdef DBG_ERR
				cerr << "main: ControlService failed to stop service. Error: " << GetLastError() << endl;
				#endif			
			}
		}
	}
	else
	{
		//
		//	Did not got the handle to SCM.
		//
		#ifdef DBG_ERR
		cerr << "main: Failed to get handle to SCM. Error: " << GetLastError() << endl;
		#endif
	}

Cleanup:

	if (hService)
	{
		//
		//	Delete the service.
		//
		bIsServiceDeleted = DeleteService(hService);

		if (FALSE == bIsServiceDeleted)
		{
			#ifdef DBG_ERR
			cerr << "main: DeleteService failed. Error " << GetLastError() << endl;
			#endif
		}

		//
		//	Close handle to service.
		//
		bIsServiceHandleClosed = CloseServiceHandle(hService);

		if (FALSE == bIsServiceHandleClosed)
		{
			#ifdef DBG_ERR
			cerr << "main: CloseServiceHandle failed to close handle to service. Error " << GetLastError() << endl;
			#endif
		}
	}

	if (hSCManager)
	{
		//
		//	Close handle to SCM.
		//
		bIsServiceHandleClosed = CloseServiceHandle(hSCManager);

		if (FALSE == bIsServiceHandleClosed)
		{
			#ifdef DBG_ERR
			cerr << "main: CloseServiceHandle failed to close handle to SCM. Error " << GetLastError() << endl;
			#endif
		}
	}

	#ifdef DBG_ALL
	cout << "main: Exit" << endl;
	#endif

	return 0;
}