#pragma warning( disable : 4005)

#include "OutputDlg.h"
#include "resource.h"
#include "iCPlugin.h"
#include <vector>
#include <string>
#include <sstream>
#include <richedit.h>
#include <iosfwd>
#include <commctrl.h>



extern NppData nppData;
extern iCPlugin thePlugin;

/*
HWND output_dlg_wnd = NULL;
HWND output_edit_box = NULL;
bool output_dlg_docked = false;
*/

INT_PTR CALLBACK OutputDlgProc(
								 HWND   hDlg, 
								 UINT   uMessage, 
								 WPARAM wParam, 
								 LPARAM lParam)
{
	if (uMessage == WM_COMMAND)
	{
		switch (LOWORD(wParam)) 
		{
		case IDOK: // Save
			{
				//MessageBox(0, _T("IDOK"), _T("Message"), MB_OK);
				return 1;
			}
		}
	}

	else if (uMessage == WM_SYSCOMMAND)
	{
		if (wParam == SC_CLOSE)
		{
			//MessageBox(0, _T("SC_CLOSE"), _T("Message"), MB_OK);
			return 1;
		}
	}

	else if (uMessage == WM_NOTIFY)
	{
		/*if(output_edit_box)
		::SendMessage(output_edit_box, EM_SETSEL, 2, 5);*/
	}

	else if (uMessage == WM_SIZE)
	{
		//resize the edit box to fit the whole client area
		RECT  rect;
		HWND  hEd;
		::GetClientRect(hDlg, &rect);
		hEd = ::GetDlgItem(hDlg, IDC_OUTPUT_EDIT);
		::MoveWindow(hEd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE);
	}

	else if (uMessage == WM_INITDIALOG)
	{
		/*output_edit_box = GetDlgItem(hDlg, IDC_OUTPUT_EDIT);
		::SendMessage(output_edit_box, WM_SETTEXT, 0, (LPARAM)_T("Some text"));*/
	}

	else if (uMessage == WM_DESTROY)
	{
		//MessageBox(0, _T("WM_DESTROY"), _T("Message"), MB_OK);
	}

	else if (uMessage == WM_SHOWWINDOW)
	{
		if(!wParam)
		{
			DestroyWindow(hDlg);
			thePlugin.output_dlg_docked = false;
		}
	}

	return 0;
}
