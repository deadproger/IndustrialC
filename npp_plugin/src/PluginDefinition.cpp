//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#pragma warning( disable : 4005)

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "iCPlugin.h"
#include "resource.h"
#include "Docking.h"

#include <cstdlib>

extern iCPlugin thePlugin;

//extern HWND output_dlg_wnd;
//extern bool output_dlg_docked;
//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	/*if(output_dlg_docked)
	{
		DestroyWindow(output_dlg_wnd);
		output_dlg_docked = false;
	}*/
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
	setCommand(0, TEXT(""), NULL, NULL, false);
    setCommand(1, TEXT("Build"), build, NULL, false);
    setCommand(2, TEXT("Upload"), upload, NULL, false);
	setCommand(3, TEXT("Settings"), settings, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void build()
{
	build_();
}

void upload()
{
	if(0 != build_())
		return;

	//Check if setting have been set
	int port_index = thePlugin.get_port_index();
	if(port_index < 0 || 0 == thePlugin.get_prog_name().size())
	{
		settings();
	}

	std::wostringstream wss;
	std::wstring port = thePlugin.get_ports_list()[thePlugin.get_port_index()];

	TCHAR* buffer = new TCHAR[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETNAMEPART, MAX_PATH, (LPARAM)buffer);
	std::wstring file(buffer);

	DWORD exit_code = 0;
	std::wostringstream cmd;

	const std::wstring& mcu_name = thePlugin.get_mcu_name();

	//Upload
	//avrdude -carduino -patmega168 -P<port> -b19200 -Uflash:w:"<file>.hex":i
	cmd.str(_T(""));cmd.clear();
	cmd	<< _T("avrdude -c")
		<< thePlugin.get_prog_name().c_str() 
		<<_T(" -p")<< mcu_name.c_str() <<_T(" -P") 
		<< port.c_str() << _T(" -b19200 -Uflash:w:\"") << file.c_str() << _T(".hex\":i");
	thePlugin.CreateChildProcess(&exit_code, cmd.str().c_str());

	if(0 != exit_code)
	{
		thePlugin.write_to_output(_T("Upload failed\r\n"));
		return;
	}
}

void settings()
{
	DialogBox(thePlugin.getDllModule(),
		MAKEINTRESOURCE(IDD_DIALOG_OPT),
		nppData._nppHandle,
		SettingsDlgProc );
}

int build_()
{
	//Get COM port index
	/*int port_index = thePlugin.get_port_index();
	if(port_index < 0)
	{
		settings();
	}*/

	if(0 == thePlugin.get_mcu_name().size())
	{
		settings();
	}

	::SendMessage(nppData._nppHandle, NPPM_SAVEALLFILES, 0, 0);

	TCHAR* buffer = new TCHAR[MAX_PATH];

	//Check file extension
	::SendMessage(nppData._nppHandle, NPPM_GETEXTPART, MAX_PATH, (LPARAM)buffer);
	if(0!=wcscmp(buffer, TEXT(".ic")))
	{
		::MessageBox(0, TEXT("Select an .ic file to build"), TEXT("Error"), MB_OK);
		delete buffer;
		return 1;
	}

	::SendMessage(nppData._nppHandle, NPPM_GETNAMEPART, MAX_PATH, (LPARAM)buffer);
	std::wstring file(buffer);

	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTDIRECTORY, MAX_PATH, (LPARAM)buffer);
	std::wstring dir(buffer);

	delete buffer;

	//thePlugin.dock_output_dlg();
	thePlugin.clear_output();

	DWORD exit_code = 0;
	std::wostringstream cmd;

	//Change dir
	SetCurrentDirectory(dir.c_str());

	//Preprocess
	//avr-g++ -x c -o"<dir>/<file>.iprepr" -E "<dir>/<file>.ic"
	cmd.str(_T(""));cmd.clear();
	cmd	<< _T("avr-g++ -x c -o\"") 
		<< file.c_str() << _T(".iprepr\" -E \"") 
		<< file.c_str() << _T(".ic\"");
	thePlugin.CreateChildProcess(&exit_code, cmd.str().c_str());

	if(0 != exit_code)
	{
		thePlugin.write_to_output(_T("Initial preprocessing failed, terminating\r\n"));
		return 1;;
	}

	Sleep(200);

	//Translate
	//industrialc -o "<dir>/<file>.cpp" "<dir>/<file>.iprepr"
	cmd.str(_T(""));cmd.clear();
	cmd	<< _T("industrialc -o \"") // note the space after -o is important
		<< file.c_str() << _T(".cpp\" \"") 
		<< file.c_str() << _T(".iprepr\"");
	thePlugin.CreateChildProcess(&exit_code, cmd.str().c_str());

	DeleteFile((file+std::wstring(_TEXT(".iprepr"))).c_str());

	if(0 != exit_code)
	{
		thePlugin.write_to_output(_T("Translation failed, terminating\r\n"));
		return 1;;
	}

	Sleep(200);

	const std::wstring& mcu_name = thePlugin.get_mcu_name();

	//Build
	//avr-gcc -Os -mmcu=atmega168 -DF_CPU=16000000UL -o "<dir>/<file>.elf" "<dir>/<file>.cpp"
	cmd.str(_T(""));cmd.clear();
	cmd	<< _T("avr-gcc -Os -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=")
		<< mcu_name.c_str() <<_T(" -DF_CPU=16000000UL -o \"") 
		<< file.c_str() << _T(".elf\" \"") 
		<< file.c_str() << _T(".cpp\"");
	thePlugin.CreateChildProcess(&exit_code, cmd.str().c_str());

	if(!thePlugin.get_keep_cpp())
		DeleteFile((file+std::wstring(_TEXT(".cpp"))).c_str());

	if(0 != exit_code)
	{
		thePlugin.write_to_output(_T("Build failed, terminating\r\n"));
		return 1;;
	}

	Sleep(200);

	//Produce hex
	//avr-objcopy -O ihex -R .eeprom "<dir>/<file>.elf" "<dir>/<file>.hex"
	cmd.str(_T(""));cmd.clear();
	cmd	<< _T("avr-objcopy -O ihex -R .eeprom  \"") 
		<< file.c_str() << _T(".elf\" \"") 
		<< file.c_str() << _T(".hex\"");
	thePlugin.CreateChildProcess(&exit_code, cmd.str().c_str());

	DeleteFile((file+std::wstring(_TEXT(".elf"))).c_str());

	if(0 != exit_code)
	{
		thePlugin.write_to_output(_T("Failed to produce hex, terminating\r\n"));
		return 1;
	}

	Sleep(200);

	cmd.str(_T(""));cmd.clear();
	cmd<<"Build successfull, " << file.c_str() << _T(".hex generated\r\n");
	thePlugin.write_to_output(cmd.str().c_str());

	return 0;
}