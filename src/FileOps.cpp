/*------------------------------------------
   FILEOPS.CPP -- Write Data Files
  ------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <time.h>
#include "maths.h"

#define	MAXBUFFER	10000
//#define	RECORDDUMP					// dump all records to test.cal

static	OPENFILENAME	ofn ;
static	char		*szFilter[] = { "Text Files (*.TXT)",	"*.txt",
					"All Files (*.*)",	"*.*",
					"" } ;

void FileInitialize (HWND hwnd)
     {
     ofn.lStructSize	   = sizeof (OPENFILENAME) ;
     ofn.hwndOwner	   = hwnd ;
     ofn.hInstance	   = NULL ;
     ofn.lpstrFilter	   = szFilter [0] ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex	   = 0 ;
     ofn.lpstrFile	   = NULL ;	     // Set in Open and Close functions
     ofn.nMaxFile	   = _MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;	     // Set in Open and Close functions
     ofn.nMaxFileTitle	   = _MAX_FNAME + _MAX_EXT ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.lpstrTitle	   = NULL ;
     ofn.Flags		   = 0 ;	     // Set in Open and Close functions
     ofn.nFileOffset	   = 0 ;
     ofn.nFileExtension    = 0 ;
     ofn.lpstrDefExt	   = "txt" ;
     ofn.lCustData	   = 0L ;
     ofn.lpfnHook	   = NULL ;
     ofn.lpTemplateName    = NULL ;
     }

BOOL FileSaveDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     ofn.hwndOwner	   = hwnd ;
     ofn.lpstrFile	   = lpstrFileName ;
     ofn.lpstrFileTitle    = lpstrTitleName ;
     ofn.Flags		   = OFN_OVERWRITEPROMPT ;
     ofn.lpstrDefExt	   = "txt" ;
     ofn.lpstrFilter	   = szFilter [1] ;

     return GetSaveFileName (&ofn) ;
     }

/**************************************************************************
	write the datafile. 
**************************************************************************/

BOOL FileWrite (HWND hwnd, LPSTR lpstrFileName, char *lpText, char *Time, char *Name)
    {
    FILE	*hFile;
    int		i;
    DWORD	ErrorValue;
    char	s[240];

    if ((hFile = fopen (lpstrFileName, "w+")) < 0)
	{
	ErrorValue = GetLastError();
	sprintf (s, "Could not open file %s! Error code %ld", lpstrFileName, ErrorValue) ;
	MessageBox(hwnd, s, "Write File", MB_ICONEXCLAMATION | MB_OK);
	return FALSE ;
	}

    fprintf(hFile, "%s\n", Time);
    fprintf(hFile, "%s\n", Name);
    for (i = 0; i < NbDigits/50; i++)
	fprintf(hFile, "%10.10s  %10.10s  %10.10s  %10.10s  %10.10s  : %d\n", 
	    lpText + i * 50, lpText + i * 50 + 10, lpText + i * 50 + 20, lpText + i * 50 + 30, lpText + i * 50 + 40, (i+1)*50);

    fclose (hFile) ;
    return TRUE ;
    }

/**************************************************************************
	write the datafile. check if description has multilines and if so, add quotes.
**************************************************************************/

int	SaveData(HWND hwnd, char *szFileName, char *szAppName, char *lpText, char *Time, char *Name)
    {
    char    s[240];

    FileInitialize(hwnd);
    if (FileSaveDlg (hwnd, szFileName, szAppName))
	{
	if (!FileWrite (hwnd, szFileName, lpText, Time, Name))
	    {
	    sprintf (s, "Could not write file %s!", szFileName) ;
	    MessageBox(hwnd, s, "Constant File", MB_ICONEXCLAMATION | MB_OK);
	    szFileName  [0] = '\0';
	    return -1;
	    }
	}
    return 0;
    }

