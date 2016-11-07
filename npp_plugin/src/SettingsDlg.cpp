#pragma warning( disable : 4005)

#include "SettingsDlg.h"
#include "resource.h"
#include "iCPlugin.h"
#include <vector>
#include <string>
#include <sstream>



extern NppData nppData;
extern iCPlugin thePlugin;

INT_PTR CALLBACK SettingsDlgProc(
								 HWND   hDlg, 
								 UINT   uMessage, 
								 WPARAM wParam, 
								 LPARAM lParam)
{
	HWND MCUList = GetDlgItem(hDlg, IDC_LIST_MCU);
	HWND PortList = GetDlgItem(hDlg, IDC_LIST_PORT);
	if ( uMessage == WM_COMMAND )
	{
		switch ( LOWORD(wParam) ) 
		{
		case IDC_LIST_MCU:
			if ( HIWORD(wParam) == BN_CLICKED )
			{
				//SettingsDlg_OnChBracketsAutoCompleteClicked(hDlg);
			}
			break;

		case IDC_LIST_PORT:
			if ( HIWORD(wParam) == BN_CLICKED )
			{
				//SettingsDlg_OnChBracketsDoTagClicked(hDlg);
			}
			break;

		case IDOK:
			{
				unsigned int port = SendMessage(PortList, CB_GETCURSEL, 0, 0);
				thePlugin.select_port_index(port);

				EndDialog(hDlg, 1); // OK - returns 1
				return 1;
			}
			

		case IDCANCEL:
			{
				EndDialog(hDlg, 0); // Cancel - returns 0
				return 1;
			}
			

		default:
			break;
		}
	}

	else if ( uMessage == WM_NOTIFY )
	{

	}

	else if ( uMessage == WM_INITDIALOG )
	{

		//::MessageBox(0, TEXT("WM_INITDIALOG"), TEXT(""), MB_OK);

		//Fill the MCU listbox
		SendMessage(MCUList, CB_ADDSTRING, 0, (LPARAM)L"atmega168");
		SendMessage(MCUList, CB_SETCURSEL, 0, 0);

		//Fill the ports list box
		//HWND PortList = GetDlgItem(hDlg, IDC_LIST_PORT);
		thePlugin.build_ports_list();//force rebuild each time the dialog is initialized
		const WStringList& port_list = thePlugin.get_ports_list();

		

		for(WStringList::const_iterator i=port_list.begin();i!=port_list.end();i++)
		{
			SendMessage(PortList, CB_ADDSTRING, 0, (LPARAM)((*i).c_str()));
		}
		int port_index = thePlugin.get_port_index();

		if(port_index < 0) port_index = 0;
		SendMessage(PortList, CB_SETCURSEL, (WPARAM)port_index, 0);


		AnyWindow_CenterWindow(hDlg, nppData._nppHandle, FALSE);
	}

	return 0;
}

BOOL AnyWindow_CenterWindow(HWND hWnd, HWND hParentWnd, BOOL bRepaint)
{
	RECT rectParent;
	RECT rect;
	INT  height, width;
	INT  x, y;

	GetWindowRect(hParentWnd, &rectParent);
	GetWindowRect(hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	x = ((rectParent.right - rectParent.left) - width) / 2;
	x += rectParent.left;
	y = ((rectParent.bottom - rectParent.top) - height) / 2;
	y += rectParent.top;
	return MoveWindow(hWnd, x, y, width, height, bRepaint);
}