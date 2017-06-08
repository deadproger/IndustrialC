#pragma warning( disable : 4005)

#include "SettingsDlg.h"
#include "resource.h"
#include "iCPlugin.h"
#include <vector>
#include <string>
#include <sstream>

const unsigned int MAX_MCU_NAME_LENGTH = 32;
const unsigned int MAX_PROG_NAME_LENGTH = 32;

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
	HWND ProgList = GetDlgItem(hDlg, IDC_LIST_PROG);
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

				unsigned int mcu = SendMessage(MCUList, CB_GETCURSEL, 0, 0);
				//WCHAR mcu_name_buf[MAX_MCU_NAME_LENGTH];
				//SendMessage(MCUList, CB_GETLBTEXT, mcu, (LPARAM)mcu_name_buf);
				thePlugin.select_mcu(mcu);

				unsigned int prog = SendMessage(ProgList, CB_GETCURSEL, 0, 0);
				//WCHAR prog_name_buf[MAX_PROG_NAME_LENGTH];
				//SendMessage(ProgList, CB_GETLBTEXT, prog, (LPARAM)prog_name_buf);
				thePlugin.select_programmer(prog);

				thePlugin.SaveSettings();

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
		thePlugin.LoadSettings();
		/*std::wstringstream ws;
		ws<<"mcu_index: "<<thePlugin.get_mcu_index()<<std::endl<<
			"port_index: "<<thePlugin.get_port_index()<<std::endl<<
			"prog_index: "<<thePlugin.get_prog_index()<<std::endl;
		MessageBox(NULL, ws.str().c_str(), _T(""), MB_OK);*/

		//Fill the MCU listbox
		const WStringList& mcu_list = thePlugin.get_mcu_list();
		for(WStringList::const_iterator i=mcu_list.begin();i!=mcu_list.end();i++)
		{
			SendMessage(MCUList, CB_ADDSTRING, 0, (LPARAM)((*i).c_str()));
		}
		SendMessage(MCUList, CB_SETCURSEL, (WPARAM)thePlugin.get_mcu_index(), 0);
		
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

		//Fill the programmer listbox
		const WStringList& prog_list = thePlugin.get_prog_list();
		for(WStringList::const_iterator i=prog_list.begin();i!=prog_list.end();i++)
		{
			SendMessage(ProgList, CB_ADDSTRING, 0, (LPARAM)((*i).c_str()));
		}
		SendMessage(ProgList, CB_SETCURSEL, (WPARAM)thePlugin.get_prog_index(), 0);

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