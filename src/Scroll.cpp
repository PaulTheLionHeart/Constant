//  -----------------------------------------------------
//   SCROLL.CPP --   Scrollbar functions.
//  -----------------------------------------------------

// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright  1994-1997  Microsoft Corporation.  All Rights Reserved.
//
//  FILE:
//    CHILD.C
//
//  PURPOSE:
//    Routines for child windows.
//
//  PLATFORMS:
//    Windows 95, Windows NT
//
//  SPECIAL INSTRUCTIONS: N/A
//

// These pragmas allow for the maximum warning level to be set
// without getting bombarded by warnings from the Windows SDK
// include files.
#pragma warning(disable:4001)   // Single-line comment warnings
#pragma warning(disable:4115)   // Named type definition in parentheses
#pragma warning(disable:4201)   // Nameless struct/union warning
#pragma warning(disable:4214)   // Bit field types other than int warnings
#pragma warning(disable:4514)   // Unreferenced inline function has been removed

// Windows Header Files:
#include <Windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>
#include <STDLIB.H>
//#include "view.h"
//#include "dib.h"
//#include "Screen.h"

// Restore the warnings--leave the single-line comment warning OFF
#pragma warning(default:4115)   // Named type definition in parentheses
#pragma warning(default:4201)   // Nameless struct/union warning
#pragma warning(default:4214)   // Bit field types other than int warnings

// C RunTime Header Files

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	hwnd;					// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

//extern	CDib	Dib;
//extern	ScreenStruct	SCREEN;			// basic dimension of screen and main window
//extern	ZoomStruct	ZOOM;			// display control info 

//////////////////////////////////////////////////////////////////////////
//  Function:  InitializeScrollBars
//////////////////////////////////////////////////////////////////////////
void InitializeScrollBars(HWND hwnd, RECT *rect, RECT *rClient)
    {
//    RECT        rClient;
    SCROLLINFO  ScrollInfo;
    BYTE	flag = FALSE;
    int		x, y;

    x = /*GetSystemMetrics(SM_CXFRAME) * 2 + */(rect->right - rect->left);
    y = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2 + (rect->bottom - rect->top);
//    y = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYMENU) + (rect->bottom - rect->top);
    ZeroMemory(&ScrollInfo, sizeof(SCROLLINFO));
    // Get windows client size.
//    SystemParametersInfo(SPI_GETWORKAREA, 0, &rClient, 0);	// let's get usable area including task bar
//    if (flag = (ZOOM.zoomflag == 'Z' || ZOOM.zoomflag == 'W'  || ZOOM.zoomflag == 'O' || ZOOM.zoomflag == 'I' || ZOOM.zoomflag == 'B'))
	{
//	x = rClient.right - 1;
//	y = rClient.bottom - 1;
	}

    // If client size is equal to dib size, then add one to client size
    // so that we don't show scroll bars.
    // However, if client size is less than dib size, subtract scroll bar
    // size form client size so that page size will be correct when
    // scroll bars are shown.
    if (rClient->bottom == (rect->bottom - rect->top))
        ++rClient->bottom;
    else if (rClient->bottom < (rect->bottom - rect->top))
        rClient->bottom -= GetSystemMetrics (SM_CYHSCROLL);
    if (rClient->right == (rect->right - rect->left))
        ++rClient->right;
    else if (rClient->right < (rect->right - rect->left))
        rClient->right -= GetSystemMetrics (SM_CXVSCROLL);

    // Initialize vertical scroll bar.
    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_ALL;
    ScrollInfo.nMin = 0;
    ScrollInfo.nMax = y;
    ScrollInfo.nPage = (flag) ? y + 1 : rClient->bottom;
    ScrollInfo.nPos = 0;
    SetScrollInfo(hwnd, SB_VERT, &ScrollInfo, TRUE);

    // Initialize horizontal scroll bar.
    ScrollInfo.nMax = x;
    ScrollInfo.nPage = (flag) ? x + 1 : rClient->right;
    SetScrollInfo(hwnd, SB_HORZ, &ScrollInfo, TRUE);
    }

//////////////////////////////////////////////////////////////////////////
//  Function:  ScrollChildWindow
//
//  Description:
//
//
//  Parameters:
//    @@@
//
//  Returns:
//    BOOL
//
//  Comments:
//
//
//////////////////////////////////////////////////////////////////////////

BOOL ScrollImage(HWND hwnd, int nScrollBar, WORD wScrollCode)
    {
    int         nPosition;
    int         nHorzScroll = 0;
    int         nVertScroll = 0;
    SCROLLINFO  ScrollInfo;

    // Get current scroll information.
    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_ALL;
    GetScrollInfo(hwnd, nScrollBar, &ScrollInfo);
    nPosition = ScrollInfo.nPos;

    // Modify scroll information based on requested
    // scroll action.
    switch (wScrollCode)
    {
        case SB_LINEDOWN:
            ScrollInfo.nPos += (ScrollInfo.nMax / 16 + 1);
            break;

        case SB_LINEUP:
            ScrollInfo.nPos -= (ScrollInfo.nMax / 16 + 1);
            break;

        case SB_PAGEDOWN:
            ScrollInfo.nPos += ScrollInfo.nPage;
            break;

        case SB_PAGEUP:
            ScrollInfo.nPos -= ScrollInfo.nPage;
            break;

        case SB_TOP:
            ScrollInfo.nPos = ScrollInfo.nMin;
            break;

        case SB_BOTTOM:
            ScrollInfo.nPos = ScrollInfo.nMax;
            break;

            // Don't do anything.
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            ScrollInfo.nPos = ScrollInfo.nTrackPos;
            break;

        case SB_ENDSCROLL:
            default:
            return FALSE;
	}

    // Make sure that scroll position is in range.
    if (0 > ScrollInfo.nPos)
        ScrollInfo.nPos = 0;
    else if (ScrollInfo.nMax - (int) ScrollInfo.nPage + 1 < ScrollInfo.nPos)
        ScrollInfo.nPos = ScrollInfo.nMax  - ScrollInfo.nPage + 1;

    // Set new scroll position.
    ScrollInfo.fMask = SIF_POS;
    SetScrollInfo(hwnd, nScrollBar, &ScrollInfo, TRUE);

    // Scroll window.
    if (SB_VERT == nScrollBar)
        nVertScroll = nPosition - ScrollInfo.nPos;
    else
        nHorzScroll = nPosition - ScrollInfo.nPos;

    ScrollWindowEx(hwnd, nHorzScroll, nVertScroll, NULL, NULL,
                   NULL, NULL, SW_INVALIDATE);

    return TRUE;
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : GetRealClientRect(HWND hwnd, LPRECT lprc)                  *
 *                                                                          *
 *  PURPOSE    : Calculates the client rectangle taking scrollbars into     *
 *               consideration.                                             *
 *                                                                          *
 ****************************************************************************/

void GetRealClientRect (HWND hwnd, PRECT lprc)

    {
    DWORD dwStyle;

    dwStyle = GetWindowLong (hwnd, GWL_STYLE);
    GetClientRect (hwnd,lprc);

    if (dwStyle & WS_HSCROLL)
        lprc->bottom += GetSystemMetrics (SM_CYHSCROLL);

    if (dwStyle & WS_VSCROLL)
        lprc->right  += GetSystemMetrics (SM_CXVSCROLL);
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : SetScrollRanges(zoomflag)				    *
 *                                                                          *
 *  PURPOSE    :                                                            *
 *                                                                          *
 ****************************************************************************/

void SetScrollRanges(HWND hwnd)

    {
    RECT        rClient;
    SCROLLINFO  ScrollInfo;
    BYTE	flag = FALSE;
    int		x, y;

//    x = ZOOM.ptSize.x - 1;
//    y = ZOOM.ptSize.y - 1;
    ZeroMemory(&ScrollInfo, sizeof(SCROLLINFO));
    // Get client size.
    GetRealClientRect(hwnd, &rClient);
//    if (flag = (zoomflag == 'Z' || zoomflag == 'W'  || zoomflag == 'O' || zoomflag == 'I' || zoomflag == 'B'))
	{
	x = rClient.right - 1;
	y = rClient.bottom - 1;
	}

    // Initialize vertical scroll bar.
    ScrollInfo.cbSize = sizeof(SCROLLINFO);
    ScrollInfo.fMask = SIF_ALL;
    ScrollInfo.nMin = 0;
    ScrollInfo.nMax = y;
    ScrollInfo.nPage = (flag) ? y + 1 : rClient.bottom;
    ScrollInfo.nPos = 0;
    SetScrollInfo(hwnd, SB_VERT, &ScrollInfo, TRUE);

    // Initialize horizontal scroll bar.
    ScrollInfo.nMax = x;
    ScrollInfo.nPage = (flag) ? x + 1 : rClient.right;
    SetScrollInfo(hwnd, SB_HORZ, &ScrollInfo, TRUE);
    }

