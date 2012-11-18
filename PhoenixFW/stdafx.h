// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0C568B09_4D19_40F6_9213_484EB885C0B1__INCLUDED_)
#define AFX_STDAFX_H__0C568B09_4D19_40F6_9213_484EB885C0B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "atlbase.h"

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib") 
#pragma warning(disable: 4996)
#include "../common/PMacRes.h"
#include "../common/TypeStruct.h"
#include "../common/iocommon.h"
#include "PRuleFile.h"
#include "PIOControl.h"
#include "ptutils.h"

extern CPRuleFile g_RuleFile;
extern CPIOControl *g_pIoControl;


CString GetFilePath(TCHAR *sFilename);
CString GetFileName(TCHAR *sFilename);

BOOL InstallProvider(WCHAR *pwszPathName);
BOOL RemoveProvider();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0C568B09_4D19_40F6_9213_484EB885C0B1__INCLUDED_)
