#include "NppPlugin.h"
#include <string>
#include <sstream>
#include <vector>
#include "CStrT.h"


typedef std::vector<std::wstring> WStringList;
typedef CStrT<TCHAR> tstr;
const std::wstring no_port = TEXT("No Ports");

class iCPlugin : public CNppPlugin
{
public:
	iCPlugin()
		:	selected_port_index(-1),
			selected_mcu(TEXT("")),
			selected_programmer(TEXT("")),
			output_dlg_wnd(NULL),
			//output_edit_box(NULL),
			output_dlg_docked(false)
	{
		build_ports_list();
	}
	virtual const TCHAR* nppGetName() {return NPP_PLUGIN_NAME;}

	void select_port_index(unsigned int index) {selected_port_index = index;}
	void select_mcu(const std::wstring& mcu_name) {selected_mcu = mcu_name;}
	void select_programmer(const std::wstring& prog_name) {selected_programmer = prog_name;}
	int get_port_index() {return selected_port_index;}
	const std::wstring& get_mcu_name() {return selected_mcu;}
	const std::wstring& get_prog_name() {return selected_programmer;}
	const std::wstring& get_selected_port()
	{
		if(com_ports_list.empty())
			return no_port;
		return com_ports_list[selected_port_index];
	}
	void build_ports_list();//rebuilds the list checking which ports are available, takes some time
	const WStringList& get_ports_list() 
	{
		if(com_ports_list.empty())
		{
			build_ports_list();
		}
		return com_ports_list;
	}

	void dock_output_dlg();

	BOOL CreateChildProcess( DWORD *exit_code, LPCTSTR cszCommandLine );
	DWORD Console_ReadPipesAndOutput(CStrT<char>& bufLine, 
		bool& bPrevLineEmpty, int& nPrevState, bool bOutputAll,
		bool& bDoOutputNext);
	void write_to_output(LPCTSTR msg);
	void output_error(LPCTSTR err_msg);
	void clear_output();

	HWND output_dlg_wnd;
	bool output_dlg_docked;
	//HWND output_edit_box;

	
private:
	HANDLE  m_hStdInReadPipe;
	HANDLE  m_hStdInWritePipe; 
	HANDLE  m_hStdOutReadPipe;
	HANDLE  m_hStdOutWritePipe;
	TCHAR   _consoleStrToWrite[0x400];
	HANDLE  _consoleStdInWritePipe;
	bool   	_consoleProcessBreak;
	bool   	_consoleProcessIsRunning;
	//bool   	_consoleIsVisible;
	HWND   	_consoleParentWnd;
	HANDLE 	_consoleProcessHandle;
	int     m_nChildProcessExitCode;

	WStringList com_ports_list;
	int selected_port_index;
	std::wstring selected_mcu;
	std::wstring selected_programmer;
};