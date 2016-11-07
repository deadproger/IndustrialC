#ifndef _settings_dlg_h_
#define _settings_dlg_h_
//---------------------------------------------------------------------------
#include <windows.h>
#include <TCHAR.h>

BOOL AnyWindow_CenterWindow(HWND hWnd, HWND hParentWnd, BOOL bRepaint);
INT_PTR CALLBACK SettingsDlgProc(HWND, UINT, WPARAM, LPARAM);


//---------------------------------------------------------------------------
#endif
