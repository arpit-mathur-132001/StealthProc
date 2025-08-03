/********************************************************************************
*																				*
* File Name:																	*
* 	misc.h																		*
*																				*
* Abstract:																		*
* 	This file contains debug settings.											*
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
//	M A C R O S.
/////////////////////////////////////////////////////////////////////
#define DBG_ALL TRUE
#define DBG_ERR TRUE


//
//	Debug particular function(s)
//
//#define DBG_ALL_DRIVERENTRY TRUE
//#define DBG_ERR_DRIVERENTRY TRUE


#ifdef DBG_ALL

//
//	StealthProc.c
//
#define DBG_ALL_DRIVERENTRY TRUE
#define DBG_ALL_DRIVERUNLOAD TRUE

//
//	functions.c
//
#define DBG_ALL_DISPATCHCREATE TRUE
#define DBG_ALL_DISPATCHDEVICECONTROL TRUE

//
//	misc.c
//
#define DBG_ALL_INITIALIZEOFFSETS TRUE
#define DBG_ALL_CALCPROCESSNAMEOFFSET TRUE
#define DBG_ALL_CALCPIDOFFSET TRUE
#define DBG_ALL_HIDEPROCESS TRUE

#endif


#ifdef DBG_ERR

//
//	StealthProc.c
//
#define DBG_ERR_DRIVERENTRY TRUE
#define DBG_ERR_DRIVERUNLOAD TRUE

//
//	functions.c
//
#define DBG_ERR_DISPATCHCREATE TRUE
#define DBG_ERR_DISPATCHDEVICECONTROL TRUE

//
//	misc.c
//
#define DBG_ERR_INITIALIZEOFFSETS TRUE
#define DBG_ERR_CALCPROCESSNAMEOFFSET TRUE
#define DBG_ERR_CALCPIDOFFSET TRUE
#define DBG_ERR_HIDEPROCESS TRUE

#endif