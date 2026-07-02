/*-----------------------------------------
	CONSTANT.CPP 
	Constant Generation Program Written by Paul de Leeuw
  -----------------------------------------*/

#include <windows.h>
#include <Winuser.h>
#include <commdlg.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "maths.h"
#include "resource.h"

extern	void PiMain (HWND hwnd, char *output, char *name, char *time);  
extern	void PhiMain (HWND hwnd, char *output, char *name, char *time);  
extern	void SqrtMain (HWND hwnd, char *output, char *name, char *time);  
extern	void Log2Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log3Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log4Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log5Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log6Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log7Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log8Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log9Main (HWND hwnd, char *output, char *name, char *time);  
extern	void Log10Main (HWND hwnd, char *output, char *name, char *time);  
extern	void eMain (HWND hwnd, char *output, char *name, char *time);  
extern	void eFastMain (HWND hwnd, char *output, char *name, char *time);  

extern	BOOL ScrollImage(HWND hwnd, int nScrollBar, WORD wScrollCode);
extern	void InitializeScrollBars(HWND, RECT *, RECT *);

extern	void InitText(void);
extern	int  SaveData(HWND hwnd, char *szFileName, char *szAppName, char *lpText, char *Time, char *Name);

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutBoxDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SelectConstantDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DisplayDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL	CopyToClipboard(HWND hwnd, const char *lpText);

char 	szAppName [100] = "Constant";
static	HANDLE	hBitmap = NULL;					// opening bitmap
static	HDC	hdcMem;
static	BITMAP	bm;
static	LOGFONT	lf;

enum	JustifyKind	{LEFT, CENTRE, RIGHT, JUSTIFY};
static	JustifyKind	Justification;

long NbDigits = 10000;

//#define min(a,b) (((a) < (b)) ? (a) : (b))
//#define max(a,b) (((a) > (b)) ? (a) : (b))
#define RWIDTH(rect) (rect.right - rect.left)	// IB 2009-04-14
#define RHEIGHT(rect) (rect.bottom - rect.top)

int	type = 0;   // 0 = Pi, 1 = e, 2 = e using FFT, 3 = log 2, 4 = sqrt 2

char		szFileName[MAX_PATH] = " ";
char		Name[160];
char		Time[160];

PAINTSTRUCT 	ps;
RECT 		r;
										// Styles of app. window 
DWORD		dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
//DWORD		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
int		AppIndex;
static	char    *lpText ;
static	short	nNumLines;

/*-----------------------------------------
	Main Windows Entry Point
  -----------------------------------------*/

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
     {
     HWND     hwnd;
     MSG      msg;
     WNDCLASS wndclass;

     if (!hPrevInstance)
	  {
	  wndclass.style	 = CS_HREDRAW | CS_VREDRAW;
//	  wndclass.style	 = CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
	  wndclass.lpfnWndProc	 = WndProc;
	  wndclass.cbClsExtra	 = 0;
	  wndclass.cbWndExtra	 = 0;
	  wndclass.hInstance	 = hInstance;
//	  wndclass.hIcon	 = LoadIcon (NULL, IDI_APPLICATION);
	  wndclass.hIcon	 = LoadIcon (hInstance, szAppName);
	  wndclass.hCursor	 = LoadCursor (NULL, IDC_ARROW);
	  wndclass.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
	  wndclass.lpszMenuName  = szAppName;
	  wndclass.lpszClassName = szAppName;

	  RegisterClass (&wndclass);
	  }
     
     hwnd = CreateWindow (szAppName, "Paul's Constant Generator",
     			  dwStyle, 
			  160, 120, 				// nice initial location
			  500, 300, 				// nice initial size
			  NULL, NULL, hInstance, NULL);

     ShowWindow (hwnd, nCmdShow);
     UpdateWindow (hwnd);

     while (GetMessage (&msg, NULL, 0, 0))
	  {
	  TranslateMessage (&msg);
	  DispatchMessage (&msg);
	  }

     return (int)msg.wParam;
     }

/*-----------------------------------------
	Just a quicky
  -----------------------------------------*/

void DoCaption (HWND hwnd, char *szTitleName)
     {
     char szCaption [255];

     wsprintf (szCaption, "%s", (LPSTR) (szTitleName));
     SetWindowText (hwnd, szCaption);
     }
     
/*-----------------------------------------
	Draw Opening Bitmap
  -----------------------------------------*/

void	DrawBitmap(HWND hwnd, HDC hdc, HANDLE hBitmap)

    {
    DWORD	ErrorCode;
    char	s[180];

    hdcMem = CreateCompatibleDC(hdc);
    if (SelectObject(hdcMem, hBitmap) == NULL)
	{
	ErrorCode = GetLastError();
	sprintf(s, "SelectObject Error type = %ld", ErrorCode);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	}
    SetMapMode(hdcMem, GetMapMode(hdc));
    if (GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bm) == 0)
	{
	ErrorCode = GetLastError();
	sprintf(s, "GetObject Error type = %ld", ErrorCode);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	}
    if (hBitmap)
	{
	DeleteObject(hBitmap);
	hBitmap = NULL;
	}
    MoveWindow (hwnd, 160, 120, bm.bmWidth, bm.bmHeight, TRUE);
    ShowWindow (hwnd, SW_SHOWNORMAL);
    InvalidateRect(hwnd, NULL, FALSE);      
    }

/*-----------------------------------------
	Let's Process a few messages
  -----------------------------------------*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static HINSTANCE	hInst;
    static HCURSOR	hCursor;
    static HBRUSH	hBrush;
    HDC			hdc;
    DWORD		ErrorCode;
    static HWND		hScroll;
    PAINTSTRUCT		ps;
    char		s[180];

    switch (message)
	{
	case WM_CREATE:
	    hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	    hdc = GetDC(hwnd);
	    hBitmap = LoadImage(hInst, MAKEINTRESOURCE(CONSTANT_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR/*LR_LOADFROMFILE*/);
	    DrawBitmap(hwnd, hdc, hBitmap);		// cute opening bitmap
	    return 0;

	case WM_INITMENUPOPUP:

	    switch (lParam)
		{
		case 0:	   // File menu
		    EnableMenuItem ((HMENU)wParam, IDM_EXIT, MF_ENABLED);
		    break;

		case 1:	   // Options menu
		    //			 EnableMenuItem ((HMENU)wParam, ID_SELECTOUTPUT, MF_ENABLED);
		    break;
		}
	    return 0;

	  case WM_COMMAND:
	       switch (wParam)
		    {
		    case IDM_SAVEAS:    
			 if ((SaveData(hwnd, szFileName, szAppName, lpText, Time, Name)) < 0)
			     {
			     MessageBox (hwnd, "File Save Failed!", szAppName, MB_ICONEXCLAMATION | MB_OK);
			     }
			return 0;

		    case IDM_EXIT:
			SendMessage (hwnd, WM_CLOSE, 0, 0L);
			return 0;

//		    case IDM_PRINT:
//			MessageBox (hwnd, "Printing hasn't been implemented yet!", szAppName, MB_ICONEXCLAMATION | MB_OK);
//			return 0;

		    case IDM_SELECTCONSTANT:
			if (DialogBox (hInst, "SelectConstantDlg", hwnd, SelectConstantDlg) != IDOK)
			    return 0;

			lpText = new char [NbDigits * 2];	// double to allow formatting options
			if(lpText == NULL)
			    {
			    MessageBox(hwnd, "Can't allocate memory for output", "Constant", MB_OK);
			    return 0;
			    }

			switch (type)
			    {
			    case 0:
				PiMain(hwnd, lpText, Name, Time);
				break;
			    case 1:
				eMain(hwnd, lpText, Name, Time);
				break;
			    case 2:
				eFastMain(hwnd, lpText, Name, Time);
				break;
			    case 3:
				PhiMain(hwnd, lpText, Name, Time);
				break;
			    case 4:
				SqrtMain(hwnd, lpText, Name, Time);
				break;
			    case 5:
				Log2Main(hwnd, lpText, Name, Time);
				break;
			    case 6:
				Log3Main(hwnd, lpText, Name, Time);
				break;
			    case 7:
				Log4Main(hwnd, lpText, Name, Time);
				break;
			    case 8:
				Log5Main(hwnd, lpText, Name, Time);
				break;
			    case 9:
				Log6Main(hwnd, lpText, Name, Time);
				break;
			    case 10:
				Log7Main(hwnd, lpText, Name, Time);
				break;
			    case 11:
				Log8Main(hwnd, lpText, Name, Time);
				break;
			    case 12:
				Log9Main(hwnd, lpText, Name, Time);
				break;
			    case 13:
				Log10Main(hwnd, lpText, Name, Time);
				break;
			    }

			DialogBox (hInst, "DisplayDlg", hwnd, DisplayDlg);
			return 0;

			      // Messages from Help menu

		    case IDM_ABOUT:

			 DialogBox (hInst, "AboutBoxDlg", hwnd, AboutBoxDlg);
			 return 0;
		    }
	       break;

	  case WM_SIZE:

		InvalidateRect(hwnd, NULL, FALSE);      
		return 0;

	  case WM_PAINT:

		BeginPaint(hwnd, &ps);
		if (BitBlt(ps.hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY) == 0)
		    {
		    ErrorCode = GetLastError();
		    sprintf(s, "Error type = %ld", ErrorCode);
		    MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
		    }
//		sprintf(s, "Image size = %d, %d", bm.bmWidth, bm.bmHeight);
//		MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
		MoveWindow (hwnd, 160, 120, bm.bmWidth, bm.bmHeight, TRUE);
		ShowWindow (hwnd, SW_SHOWNORMAL);
		EndPaint(hwnd, &ps);
		return 0;

        case WM_KEYDOWN:					// Handle any keyboard messages
            switch (wParam) 
                {
                case VK_F1:						// Help    
		    PostMessage (hwnd, WM_COMMAND, IDM_ABOUT,   0L);
                    break;
                case 'a':						// Select Constant to display
                case 'A': 
		     SendMessage (hwnd, WM_COMMAND, IDM_SAVEAS, 0L);
                     break;
                case 'c':						// Select Constant to display
                case 'C': 
		     SendMessage (hwnd, WM_COMMAND, IDM_SELECTCONSTANT, 0L);
                     break;
/*
                case 'e':						// Edit Appointment
                case 'E': 
		     SendMessage (hwnd, WM_COMMAND, IDM_ADD_APPOINTMENT, 0L);
                     break;
                case 'o':						// open file  
                case 'O': 
		     SendMessage (hwnd, WM_COMMAND, IDM_OPEN, 0L);
                     break;
*/
                case VK_RETURN:						// Let's get out of here
                case VK_ESCAPE: 
		    SendMessage (hwnd, WM_CLOSE, 0, 0L);
                    break;
                }
            break;

	  case WM_DESTROY :
		PostQuitMessage (0);
	       	return 0;
	  }
     return DefWindowProc (hwnd, message, wParam, lParam);
     }

/**************************************************************************
	Select Constant Dialog Box
**************************************************************************/

INT_PTR CALLBACK    SelectConstantDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static     int	temp;
     static     UINT	tempParam;
     BOOL		bTrans ;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = type;
	        switch (type)
		    {
		    case 0:
			tempParam = IDC_PI;
			break;
		    case 1:
			tempParam = IDC_E;
			break;
		    case 2:
			tempParam = IDC_EFAST;
			break;
		    case 3:
			tempParam = IDC_PHI;
			break;
		    case 4:
			tempParam = IDC_SQRT2;
			break;
		    case 5:
			tempParam = IDC_LOG2;
			break;
		    case 6:
			tempParam = IDC_LOG3;
			break;
		    case 7:
			tempParam = IDC_LOG4;
			break;
		    case 8:
			tempParam = IDC_LOG5;
			break;
		    case 9:
			tempParam = IDC_LOG6;
			break;
		    case 10:
			tempParam = IDC_LOG7;
			break;
		    case 11:
			tempParam = IDC_LOG8;
			break;
		    case 12:
			tempParam = IDC_LOG9;
			break;
		    case 13:
			tempParam = IDC_LOG10;
			break;
		    }
		CheckRadioButton(hDlg, IDC_PI, IDC_LOG10, tempParam);
		SetFocus(GetDlgItem(hDlg, tempParam));
		SetDlgItemInt(hDlg, IDC_NUMDIGITS, NbDigits, TRUE);
	        return FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_PI:
		    case IDC_E:
		    case IDC_EFAST:
		    case IDC_PHI:
		    case IDC_SQRT2:
		    case IDC_LOG2:
		    case IDC_LOG3:
		    case IDC_LOG4:
		    case IDC_LOG5:
		    case IDC_LOG6:
		    case IDC_LOG7:
		    case IDC_LOG8:
		    case IDC_LOG9:
		    case IDC_LOG10:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_PI:
				temp = 0;
				break;
			    case IDC_E:
				temp = 1;
				break;
			    case IDC_EFAST:
				temp = 2;
				break;
			    case IDC_PHI:
				temp = 3;
				break;
			    case IDC_SQRT2:
				temp = 4;
				break;
			    case IDC_LOG2:
				temp = 5;
				break;
			    case IDC_LOG3:
				temp = 6;
				break;
			    case IDC_LOG4:
				temp = 7;
				break;
			    case IDC_LOG5:
				temp = 8;
				break;
			    case IDC_LOG6:
				temp = 9;
				break;
			    case IDC_LOG7:
				temp = 10;
				break;
			    case IDC_LOG8:
				temp = 11;
				break;
			    case IDC_LOG9:
				temp = 12;
				break;
				break;
			    case IDC_LOG10:
				temp = 13;
				break;
			    }

			CheckRadioButton(hDlg, IDC_PI, IDC_LOG10, (int) LOWORD(wParam));
		        return TRUE ;
		    case IDOK:
			type = temp;
			NbDigits = GetDlgItemInt(hDlg, IDC_NUMDIGITS, &bTrans, TRUE);
			if (NbDigits < 100)
			    NbDigits = 100;
			if (NbDigits % 100 != 0)
			    NbDigits = ((NbDigits/100) + 1) * 100;	// round up to the nearest 100
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }

/**************************************************************************
	About Constant Dialog Box
**************************************************************************/

INT_PTR CALLBACK    AboutBoxDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     switch (message)
	  {
	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
			EndDialog (hDlg, TRUE);
			return TRUE;
		   }
		   break;
	    }
      return FALSE ;
      }

/**************************************************************************
	Justify Text in edit box
**************************************************************************/

int	Justify (HDC hdc, LPSTR lpText, int x, int y, RECT *prc, JustifyKind nAlign)
    {
    LPSTR   lpBegin, lpEnd;
    int	    xStart, yStart, nBreakCount;
    SIZE    sz; 

    prc->right -= prc->left;
    prc->left = 0;
    prc->bottom -= prc->top;
    prc->top = 0;

//    yStart = prc->top;
    yStart = y;
    do 			   // for each text line
	{
	nBreakCount = 0 ;
	while (*lpText == ' ')   // skip over leading blanks
	    lpText++ ;
	lpBegin = lpText ;

	do			   // until the line is known
	    {
	    lpEnd = lpText ;

	    while (*lpText != '\0' && *lpText++ != ' ');
	    if (*lpText == '\0')
		break;
	    // for each space, calculate extents
	    nBreakCount++ ;
	    SetTextJustification (hdc, 0, 0);
	    GetTextExtentPoint32 (hdc, lpBegin, (int)(lpText - lpBegin - 1), &sz);
	    }
	    while ((int) sz.cx < (prc->right - prc->left));

	    nBreakCount-- ;
	    while (*(lpEnd - 1) == ' ')	// eliminate trailing blanks
		{
		lpEnd--;
		nBreakCount--;
		}

	    if (*lpText == '\0' || nBreakCount <= 0)
		lpEnd = lpText;

	    SetTextJustification (hdc, 0, 0) ;
	    GetTextExtentPoint32 (hdc, lpBegin, (int)(lpEnd - lpBegin), &sz);

	    switch (nAlign)		// use alignment for xStart
		{
		case LEFT:
//		    xStart = prc->left ;
		    xStart = x;
		    break ;

		case RIGHT:
		    xStart = prc->right - sz.cx;
		    break ;

		case CENTRE:
		    xStart = (prc->right + prc->left - sz.cx) / 2;
		    break ;

		case JUSTIFY:
		    if (*lpText != '\0' && nBreakCount > 0)
			SetTextJustification (hdc, prc->right - prc->left - sz.cx, nBreakCount);
		    xStart = prc->left;
		    break;
		}

	    TextOut (hdc, xStart, yStart, lpBegin, (int)(lpEnd - lpBegin));
	    yStart += sz.cy;
	    lpText = lpEnd;
	}
	while (*lpText && yStart < prc->bottom - prc->top - sz.cy);
    return yStart;							// return current y position
    }

/*---------------------------------------------------------------------
	Insert Text into constant dialogue box
  -------------------------------------------------------------------*/

void	InitText(void)
    {
    lf.lfEscapement	= 0;
    lf.lfOrientation	= 0;
    lf.lfQuality	= DEFAULT_QUALITY;
    lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
    lf.lfWidth		= 0;
    lf.lfItalic		= 0;
    lf.lfUnderline	= 0;
    lf.lfStrikeOut	= 0;
    lf.lfOrientation	= 0; 
    lf.lfWeight		= FW_NORMAL; 
    }

void	InsertText(HDC hdc, char *szText, int x, int y, int size, DWORD colour)
    {
    HFONT   font;

    SetBkColor(hdc, colour);
    lf.lfHeight	= size;
    font = CreateFontIndirect (&lf);
    SelectObject (hdc, font) ;
    TextOut (hdc, x, y, szText, (int)strlen(szText)) ;
    DeleteObject(font);
    }

int	JustifyText(HDC hdc, char *szText, int x, int y, RECT *prc, int size, JustifyKind Justification, DWORD colour)
    {
    HFONT   font;
    int	    yPos;

    SetBkColor(hdc, colour);
    lf.lfHeight	= size;
    font = CreateFontIndirect (&lf);
    SelectObject (hdc, font) ;
    yPos = Justify (hdc, szText, x, y, prc, Justification);
    DeleteObject(font);
    return yPos;
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : IsAnyScrollBar(void)                                       *
 *                                                                          *
 *  PURPOSE    : Are there any scroll bars?                                 *
 *                                                                          *
 ****************************************************************************/

BOOL	IsAnyScrollBar (HWND hwnd)
    {
    DWORD dwStyle;

    dwStyle = GetWindowLong (hwnd, GWL_STYLE);
    return (dwStyle & WS_HSCROLL || dwStyle & WS_VSCROLL);
    }

/**************************************************************************
	Display the Constants Dialog Box
**************************************************************************/

INT_PTR CALLBACK    DisplayDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     RECT	    rect, ClientRect;
     HDC	    hdc;
     TEXTMETRIC	    tm ;
     static short   nPosition, cxChar, cyChar, cyClient, xScroll, maxrows;
     int	    row;
     char	    data[200];
     HWND	    hCtrl;
     SCROLLINFO	    ScrollInfo;

     switch (message)
	  {
	  case WM_INITDIALOG:
	      	hdc = GetDC (hDlg);
		GetTextMetrics (hdc, &tm);
		cxChar = (short)tm.tmAveCharWidth;
		cyChar = (short)(tm.tmHeight + tm.tmExternalLeading);
		GetClientRect (hDlg, &ClientRect);
		ClientRect.left += cxChar;
		rect.left = ClientRect.left;
		rect.right = ClientRect.right;
		rect.bottom = cyChar * (1+NbDigits/50);
		rect.top = ClientRect.top;
		InitializeScrollBars(hDlg, &rect, &ClientRect);
		ReleaseDC (hDlg, hdc);
	        return FALSE;

	  case WM_PAINT:
	        {
		PAINTSTRUCT 	ps;
		int		i;
		HBRUSH		hBrush;
		DWORD		colour;
		int		VertScrollPos, HorScrollPos;
		static	int	yPos = 0;

		VertScrollPos = GetScrollPos (hDlg, SB_VERT);
		HorScrollPos = GetScrollPos (hDlg, SB_HORZ);
		ScrollInfo.cbSize = sizeof(SCROLLINFO);
		ScrollInfo.fMask = SIF_ALL;
		GetScrollInfo(hDlg, SB_VERT, &ScrollInfo);

		BeginPaint(hDlg, &ps);
										// set background colour
		colour = RGB(150,255,150);
		GetClientRect(hDlg, &rect);
		hBrush = CreateSolidBrush(colour);
		FillRect(ps.hdc, &rect, hBrush);
		DeleteObject(hBrush);
			
		yPos = 10;
		GetClientRect(hDlg, &ClientRect);
		maxrows = (short)(ClientRect.bottom - ClientRect.top) / cyChar - 8;
		row = (int)((float)(ScrollInfo.nPos) / (float)(ScrollInfo.nMax - ScrollInfo.nPage) * (float)(NbDigits/50 - maxrows));
		yPos = JustifyText(ps.hdc, Time, 100, 1 + yPos, &ClientRect, 16, LEFT, RGB(200,250,250));
		yPos = JustifyText(ps.hdc, Name, 100, 1 + yPos, &ClientRect, 16, LEFT, RGB(250,200,250));
		for (i = row; i < NbDigits/50 && i < row + maxrows; i++)		// print time labels
		    {
		    sprintf_s(data, 150, "%10.10s  %10.10s  %10.10s  %10.10s  %10.10s  : %d", 
				lpText + i * 50, lpText + i * 50 + 10, lpText + i * 50 + 20, lpText + i * 50 + 30, lpText + i * 50 + 40, (i+1)*50);

		    yPos = JustifyText(ps.hdc, data, 100, 1 + yPos, &ClientRect, 16, LEFT, RGB(250,250,200));
		    }
		EndPaint(hDlg, &ps);
		}
	        return TRUE ;

          case WM_VSCROLL:
		ScrollImage(hDlg, SB_VERT, LOWORD(wParam));
		InvalidateRect(hDlg, NULL, TRUE);      
                break;

          case WM_HSCROLL:
		ScrollImage(hDlg, SB_HORZ, LOWORD(wParam));
		InvalidateRect(hDlg, NULL, TRUE);      
		break;

	  case WM_MOUSEWHEEL:
	      if (IsAnyScrollBar(hDlg))
		  {
		  if((short) HIWORD(wParam) < 0)
		      SendMessage (hDlg, WM_VSCROLL, SB_LINEDOWN, 0L);
		  else
		      SendMessage (hDlg, WM_VSCROLL, SB_LINEUP, 0L);
		  }
	      break;

	  case WM_SETFOCUS:
		hCtrl = GetDlgItem (hDlg, IDC_SAVEAS);				// scroll buttons
		SetFocus (hCtrl) ;
		break;

	  case WM_DESTROY :
		EndDialog (hDlg, TRUE);
		return FALSE;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
			EndDialog (hDlg, TRUE);
			return TRUE;
		    case IDC_SAVEAS:
			 if ((SaveData(hDlg, szFileName, szAppName, lpText, Time, Name)) < 0)
			     {
			     MessageBox (hDlg, "File Save Failed!", szAppName, MB_ICONEXCLAMATION | MB_OK);
			     }
			return TRUE;
		    case IDC_COPY:
			CopyToClipboard(hDlg, lpText);
			return TRUE;
		   }
		   break;
	    }
      return FALSE ;
      }

/**************************************************************************
	Copy the raw data to a clipboard
**************************************************************************/

BOOL	CopyToClipboard(HWND hwnd, const char *lpText)
    {
    HGLOBAL hMem;
    char    *pMem;
    size_t  len;

    if (lpText == NULL)
	return FALSE;

    len = strlen(lpText) + 1;

    hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (hMem == NULL)
	return FALSE;

    pMem = (char *)GlobalLock(hMem);
    if (pMem == NULL)
	{
	GlobalFree(hMem);
	return FALSE;
	}

    memcpy(pMem, lpText, len);

    GlobalUnlock(hMem);

    if (!OpenClipboard(hwnd))
	{
	GlobalFree(hMem);
	return FALSE;
	}

    EmptyClipboard();

    SetClipboardData(CF_TEXT, hMem);

    CloseClipboard();

    // Do NOT GlobalFree(hMem)!
    // The clipboard now owns the memory.

    return TRUE;
    }
