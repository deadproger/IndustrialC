#include "iCPlugin.h"
#include "resource.h"
#include "OutputDlg.h"
#include "docking.h"
#include <algorithm>
#include <sstream>

extern NppData nppData;

#define  MAX_PLUGIN_NAME  60
TCHAR PLUGIN_NAME_DLL[MAX_PLUGIN_NAME + 6] = _T("industrialC.dll");

//Default MCU and Programmer lists initializers
const wchar_t* MCU_LIST[] = {_T("atmega128"), _T("atmega168"), _T("atmega328")};
const wchar_t* PROGRAMMER_LIST[] = {_T("arduino"), _T("usbasp")};

//=============================================================================
//Searches WStringList for a string, returns index if found, 0 otherwise 
//=============================================================================
int get_index(WStringList& v, const wchar_t* s)
{
	WStringList::iterator it = std::find(v.begin(), v.end(), s);
	return v.end() != it ? it - v.begin() : 0;
}

//=============================================================================
//Analogous to end() in C++11
//returns pointer to the past-the-end element in an array
//=============================================================================
template<typename T, size_t N>
T * end(T (&ra)[N]) {
	return ra + N;
}
//=============================================================================
//
//=============================================================================
iCPlugin::iCPlugin() :	selected_port_index(-1),
						output_dlg_wnd(NULL),
						output_dlg_docked(false),
						mcu_list(MCU_LIST, end(MCU_LIST)),
						programmer_list(PROGRAMMER_LIST, end(PROGRAMMER_LIST)),
						selected_mcu_index(0),
						selected_programmer_index(0)
{
	build_ports_list();
}

//=============================================================================
//Generates list of COM ports currenlt available in system
//=============================================================================
void iCPlugin::build_ports_list()
{
	//Try to open each port from 1 to 255 w/ CreateFile
	//Fill the list w/ ports that opened successfully
	com_ports_list.clear();
	for(int i=1;i<255;i++)
	{
		std::ostringstream port_name;//ASCII ostringstream
		port_name << "\\\\.\\COM" << i;//Slashes are required to open anything beyond COM9
		HANDLE port = CreateFileA(
			port_name.str().c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(INVALID_HANDLE_VALUE != port)
		{
			CloseHandle(port);
			std::wostringstream port_label;//Unicode ostringstream
			port_label << "COM"<< i;//Port name w/o slashes
			com_ports_list.push_back(port_label.str());
		}
	}

	//Check if index is still valid, reset it to zero if not
	if(static_cast<int>(com_ports_list.size()) <= selected_port_index)
	{
		selected_port_index = 0;
	}
}

//=============================================================================
//
//=============================================================================
void iCPlugin::dock_output_dlg()
{
	if(!output_dlg_docked)
	{
		output_dlg_docked = true;
		output_dlg_wnd = CreateDialog(getDllModule(),
			MAKEINTRESOURCE(IDD_OUTPUT_DLG),
			nppData._nppHandle,
			OutputDlgProc);

		tTbData	dockData = {0};
		dockData.hClient       = output_dlg_wnd;
		dockData.pszName	   = _T("Output");
		dockData.dlgID         = 0;
		dockData.uMask         = DWS_DF_CONT_BOTTOM | DWS_ICONTAB;
		dockData.pszModuleName = PLUGIN_NAME_DLL;

		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM) &dockData );
	}
}

//=============================================================================
//From NppExec
//=============================================================================
BOOL IsWindowsNT(void)
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);
	GetVersionEx(&osv);
	return (osv.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

//=============================================================================
//From NppExec
//=============================================================================
BOOL iCPlugin::CreateChildProcess( DWORD *exit_code, LPCTSTR cszCommandLine )
{
	write_to_output(_T("\r\n"));
	write_to_output(cszCommandLine);
	write_to_output(_T("\r\n"));

	if(exit_code)
		*exit_code = 0;

  TCHAR                szCmdLine[0x400];
  SECURITY_DESCRIPTOR  sd;
  SECURITY_ATTRIBUTES  sa;
  SECURITY_ATTRIBUTES* lpsa = NULL;
  PROCESS_INFORMATION  pi;
  STARTUPINFO          si;
  
  m_hStdInReadPipe = NULL;
  m_hStdInWritePipe = NULL; 
  m_hStdOutReadPipe = NULL;
  m_hStdOutWritePipe = NULL;
  
  if (IsWindowsNT())
  {
    // security stuff for NT
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
    sa.lpSecurityDescriptor = &sd;
  }
  else
  {
    sa.lpSecurityDescriptor = NULL;
  }
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  lpsa = &sa;

  // Create the Pipe and get r/w handles
  if (!CreatePipe(&m_hStdOutReadPipe, &m_hStdOutWritePipe, lpsa, 0))
  {
    output_error(_T("CreatePipe(<StdOut>) failed"));
    return FALSE;
  }
  if (m_hStdOutWritePipe == NULL)
  {
    if (m_hStdOutReadPipe != NULL)
      CloseHandle(m_hStdOutReadPipe);
    output_error(_T("hStdOutWritePipe = NULL"));
    return FALSE;
  }
  if (m_hStdOutReadPipe == NULL)
  {
    CloseHandle(m_hStdOutWritePipe);
    output_error(_T("hStdOutReadPipe = NULL"));
    return FALSE;
  }

  if (!CreatePipe(&m_hStdInReadPipe, &m_hStdInWritePipe, lpsa, 0))
  {
    output_error(_T("CreatePipe(<StdIn>) failed"));
    return FALSE;
  }
  if (m_hStdInWritePipe == NULL)
  {
    if (m_hStdInReadPipe != NULL)
      CloseHandle(m_hStdInReadPipe);
    output_error(_T("hStdInWritePipe = NULL"));
    return FALSE;
  }
  if (m_hStdInReadPipe == NULL)
  {
    CloseHandle(m_hStdInWritePipe);
    output_error(_T("hStdInReadPipe = NULL"));
    return FALSE;
  }
    
  // initialize STARTUPINFO struct
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;
  si.hStdInput = m_hStdInReadPipe;
  si.hStdOutput = m_hStdOutWritePipe;
  si.hStdError = m_hStdOutWritePipe;

  _consoleStrToWrite[0] = 0;
  _consoleStdInWritePipe = m_hStdInWritePipe;
  
  lstrcpy(szCmdLine, cszCommandLine);

  if (CreateProcess(
    NULL,
    szCmdLine,
    NULL, // security
    NULL, // security
    TRUE, // inherits handles
    0,    // creation flags
    NULL, // environment
    NULL, // current directory
    &si,  // startup info
    &pi   // process info
  ))
  {
	  CloseHandle(pi.hThread);

	  _consoleProcessHandle = pi.hProcess;
	  _consoleProcessBreak = false;
	  _consoleProcessIsRunning = true;

	  //write_to_output( _T("Process started >>>\r\n") );
	  
	  const unsigned int PROC_WAIT_TIMEOUT_MILLIS = 200;

	  // this pause is necessary for child processes
	  // which return immediatelly
	  //WaitForSingleObject(pi.hProcess, 120);
	  WaitForSingleObject(pi.hProcess, PROC_WAIT_TIMEOUT_MILLIS);

	  CStrT<char>  bufLine;
	  bool         bPrevLineEmpty = false;
	  bool         bDoOutputNext = true;
	  int          nPrevState = 0;
	  DWORD        dwRead = 0;
	  unsigned int nEmptyCount = 0;
	  const DWORD  dwCycleTimeOut = 1000;
	  const DWORD  dwExitTimeOut = 100000;

	  bufLine.Clear(); // just in case :-)

	  //Read child process output till it terminates
	  do 
	  {
		  // inside this cycle: the bOutputAll parameter 
		  // must be controlled within Console_ReadPipesAndOutput
		  dwRead = Console_ReadPipesAndOutput(bufLine, 
			  bPrevLineEmpty, nPrevState, false, bDoOutputNext);

		  // we are not inside a thread
		  if ( dwRead == 0 )
		  {
			  // no output from the child process
			  ++nEmptyCount;
			  if ( nEmptyCount > (dwExitTimeOut/dwCycleTimeOut) )
			  {
				  // no output during more than dwExitTimeOut ms
				  // let's terminate the process...
				  _consoleProcessBreak = true;
			  }
		  }
		  else
		  {
			  nEmptyCount = 0;
		  }
	  }
	  while ((_consoleProcessIsRunning = (WaitForSingleObject(pi.hProcess, 
		  dwCycleTimeOut) == WAIT_TIMEOUT))
		  && output_dlg_docked && !_consoleProcessBreak);
	  // NOTE: time-out inside WaitForSingleObject() prevents from 100% CPU usage!

	  if (output_dlg_docked && !_consoleProcessBreak)
	  {
		  // maybe the child process has exited but not all its data is read
		  Console_ReadPipesAndOutput(bufLine, 
			  bPrevLineEmpty, nPrevState, true, bDoOutputNext);
	  }

	  if ( (!output_dlg_docked) || _consoleProcessBreak ) {
		  if (_consoleProcessIsRunning) {
			  if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pi.dwProcessId)) {
				  if (TerminateProcess(pi.hProcess, 0)){
						  write_to_output( _T("<<< Process has been terminated.\r\n") );
				  } else {
					  output_error( _T("<<< TerminateProcess() returned FALSE.") );
				  }
			  } /*else {
				  write_to_output( _T("<<< Process is finished normally.\r\n") );
			  }*/
		  }
	  }

	  _consoleProcessIsRunning = false;
	  _consoleProcessHandle = NULL;

	  DWORD dwExitCode = (DWORD)(-1);
	  ::GetExitCodeProcess(pi.hProcess, &dwExitCode);
	  m_nChildProcessExitCode = (int) dwExitCode;

	  if(NULL != exit_code)
	  {
		  *exit_code = m_nChildProcessExitCode;
	  }

	  // Process cleanup
	  CloseHandle(pi.hProcess);

	  // closing pipe handles
	  ::CloseHandle(m_hStdOutReadPipe);  m_hStdOutReadPipe = NULL;
	  ::CloseHandle(m_hStdOutWritePipe); m_hStdOutWritePipe = NULL;
	  ::CloseHandle(m_hStdInReadPipe);   m_hStdInReadPipe = NULL;
	  ::CloseHandle(m_hStdInWritePipe);  m_hStdInWritePipe = NULL;

	  /*if (output_dlg_docked && !_consoleProcessBreak)
	  {
			  TCHAR szMessage[96];
			  wsprintf( szMessage, _T("<<< Process finished. (Exit code %d)\r\n"), m_nChildProcessExitCode );
			  write_to_output( szMessage ); 
	  }*/

	  return TRUE;
  }
  else //CreateProcess failed
  {
	  DWORD dwErrorCode = ::GetLastError();

	  // closing pipe handles
	  ::CloseHandle(m_hStdOutReadPipe);  m_hStdOutReadPipe = NULL;
	  ::CloseHandle(m_hStdOutWritePipe); m_hStdOutWritePipe = NULL;
	  ::CloseHandle(m_hStdInReadPipe);   m_hStdInReadPipe = NULL;
	  ::CloseHandle(m_hStdInWritePipe);  m_hStdInWritePipe = NULL;

	  std::wostringstream wss;
	  wss<<_T("CreateProcess() failed with code ")<<dwErrorCode;
	  output_error(wss.str().c_str());

	  if(NULL != exit_code)
	  {
		  *exit_code = dwErrorCode;
	  }

	  return FALSE;
  }
}

//=============================================================================
//From NppExec
//=============================================================================
DWORD iCPlugin::Console_ReadPipesAndOutput(CStrT<char>& bufLine, 
										   bool& bPrevLineEmpty, int& nPrevState, bool bOutputAll,
										   bool& bDoOutputNext)
{
	const unsigned int CONSOLEPIPE_BUFSIZE = 8000;
	DWORD       dwBytesRead = 0;
	char        Buf[CONSOLEPIPE_BUFSIZE];
	CStrT<char> outLine;

	bool   bSomethingHasBeenReadFromThePipe = false; 
	// great name for local variable :-)

	const int nBufLineLength = bufLine.length();

	do
	{ 
		// let other processes use CPU for the whole 10ms - preemptive concurrency kicks ass
		Sleep(10);  // it prevents from 100% CPU usage while reading!

		dwBytesRead = 0;
		if (!PeekNamedPipe(m_hStdOutReadPipe, NULL, 0, NULL, &dwBytesRead, NULL))
		{
			dwBytesRead = 0;
		}
		if (!dwBytesRead)
		{
			// no data in the pipe
			if (!bSomethingHasBeenReadFromThePipe)
			{
				// did we read something from the pipe already?
				// if no, then let's output the data from bufLine (if any)
				bOutputAll = true;
			}
		}
		if ((dwBytesRead > 0) || bOutputAll)
		{
			// some data is in the Pipe or bOutputAll==true

			bool bContainsData = (dwBytesRead > 0) ? true : false;
			// without bContainsData==true the ReadFile operation will never return

			if (bContainsData)
				ZeroMemory(Buf, CONSOLEPIPE_BUFSIZE);
			dwBytesRead = 0;
			if ((bContainsData 
				&& ReadFile(m_hStdOutReadPipe, Buf, (CONSOLEPIPE_BUFSIZE-1)*sizeof(char), &dwBytesRead, NULL)
				&& (dwBytesRead > 0)) || bOutputAll)
			{
				// some data has been read from the Pipe or bOutputAll==true

				//int copy_len;

				Buf[dwBytesRead/sizeof(char)] = 0;
				//bufLine += Buf;
				bufLine.Append( Buf, dwBytesRead/sizeof(char) );

				if (dwBytesRead > 0)
				{
					bSomethingHasBeenReadFromThePipe = true;
				}

				std::string str(Buf);
				std::wstring wstr(str.begin(), str.end());
				write_to_output(wstr.c_str());
			}
		}
	} 
	while ((dwBytesRead > 0) && output_dlg_docked && !_consoleProcessBreak);

	if (bOutputAll && !dwBytesRead)  dwBytesRead = nBufLineLength;
	return dwBytesRead;
}

//=============================================================================
//
//=============================================================================
void iCPlugin::write_to_output( LPCTSTR msg )
{
	HWND output_edit_wnd = GetDlgItem(output_dlg_wnd, IDC_OUTPUT_EDIT);
	if(!output_edit_wnd)
	{
		//return;
		dock_output_dlg();
	}
	HRESULT text_len = ::SendMessage(output_edit_wnd, WM_GETTEXTLENGTH, 0, 0);
	::SendMessage(output_edit_wnd, EM_SETSEL, text_len, text_len); 
	::SendMessage(output_edit_wnd, EM_REPLACESEL, FALSE, (LPARAM)msg);
}

//=============================================================================
//
//=============================================================================
void iCPlugin::clear_output()
{
	HWND output_edit_wnd = GetDlgItem(output_dlg_wnd, IDC_OUTPUT_EDIT);
	if(!output_edit_wnd)
	{
		//return;
		dock_output_dlg();
	}
	::SendMessage(output_edit_wnd, WM_SETTEXT, 0, (LPARAM)_T("")); 
}

//=============================================================================
//
//=============================================================================
void iCPlugin::output_error( LPCTSTR err_msg )
{
	std::wostringstream wss;
	wss<<_T("error: ")<<err_msg<<_T("\r\n");
	write_to_output(wss.str().c_str());
}

//=============================================================================
//Called upon NPPN_READY event in nppBeNotified
//=============================================================================
void iCPlugin::OnNppReady()
{
	
	LoadSettings();

	//Dock the console
	dock_output_dlg();
}

//=============================================================================
//
//=============================================================================
void iCPlugin::SaveSettings()
{

}

//=============================================================================
//
//=============================================================================
void iCPlugin::LoadSettings()
{
	//Get ini file full path
	TCHAR szPath[2*MAX_PATH + 1];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, (WPARAM) 2*MAX_PATH, (LPARAM) szPath);
	lstrcat(szPath, _T("\\"));
	lstrcat(szPath, m_szIniFileName);

	DWORD dw;

	//Read the first setting and check if .ini file exists
	dw = GetPrivateProfileString(_T("Settings"), _T("mcu"), NULL,
		settings_buffer, SETTINGS_BUF_SIZE, szPath);
	if(ERROR_FILE_NOT_FOUND == GetLastError())
		return; //.ini file does not exist
	if(0 != dw)
		select_mcu(get_index(mcu_list, settings_buffer));

	//read next setting
	dw = GetPrivateProfileString(_T("Settings"), _T("programmer"), NULL,
		settings_buffer, SETTINGS_BUF_SIZE, szPath);
	if(0 != dw)
		select_programmer(get_index(programmer_list, settings_buffer));

	//read next setting
	dw = GetPrivateProfileString(_T("Settings"), _T("port"), NULL,
		settings_buffer, SETTINGS_BUF_SIZE, szPath);
	if(0 != dw)
		select_port_index(get_index(com_ports_list, settings_buffer));
}