// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls


#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//#include <windows.h>
//#include <afxsock.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <afx.h>
#include <afxdb.h> 
#include <odbcinst.h>
#include "SendTcpSocket.h"

#import <msxml6.dll>
using namespace MSXML2;

#include <io.h>
#include "VistorRes.h"

#include "faceCompare.h"
#include "TaiSDK.h"
#pragma comment(lib, "TaiSDK.lib")

#include "cJSON.h"

#include "CaptureUsbVideo.h"
#include "afxinet.h"
#include "afxtempl.h"

#include "ID2Reader.h"
#pragma comment(lib, "ID2Reader.lib")

#include "myZlib.h"

#pragma comment( lib, "myZlib.lib")

#ifdef _DEBUG
#pragma comment(lib, "XCGUId.lib")
#else
#pragma comment(lib, "XCGUI.lib")
#endif
#include "xcgui.h"
#pragma comment(lib, "CaptureVideo.lib")

void WINAPI XWnd_RedrawWnd(HWINDOW hWindow,BOOL bImmediately=FALSE);
// TODO: reference additional headers your program requires here
extern void W_2_S(wchar_t *pws, char *psz, int pwslen);
extern void S_2_W(wchar_t *pws, char *psz, int slen);
extern bool getNodeXmlInfo(char *xmlbuf,char *pic,char *node);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
extern "C" _declspec(dllimport) void _stdcall DrawBarCode(HDC hDC, char *pszCode, char *pszFile);
extern void GetBarCode(char *szBarCode);
extern "C" _declspec(dllimport) bool _stdcall StartClip(HDC hDC, const POINT &point);
extern "C" _declspec(dllimport) HBITMAP _stdcall StopClip(HDC hDC, const POINT &point);
extern "C" _declspec(dllimport) bool _stdcall SaveBitmapToFile(HBITMAP hBitmap, char *pszFile);
extern "C" _declspec(dllimport) void _stdcall EndClip();
extern "C" _declspec(dllimport) void _stdcall TrackDraw(HDC hDC, const POINT &point);

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
