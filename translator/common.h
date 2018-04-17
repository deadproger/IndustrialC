#pragma once

#include "icmemory.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <typeinfo>
#include <stdarg.h>

const int IC_RETURN_ERROR = 1;
const int ERR_MSG_BUFFER_SIZE = 1024;

//=================================================================================================
//Code generation templates
//=================================================================================================
const char START_STATE_NAME[] = "FS_START";
const char STOP_STATE_NAME[] = "FS_STOP";

//The comma operator and absence of semicolons are for single-statement
//"if/else" and "for" statements
const char C_MACRO_ROUTINES[] = "\
								\n\
#define	FSP_TYPE			unsigned char	\n\
#define set_newfs(p, fs)    psw[p].fsp = fs, psw[p].T = SysTime_cur\n\
#define startp(p)			psw[p].fsp = FS_START, psw[p].T = SysTime_cur\n\
#define stopp(p)			psw[p].fsp = FS_STOP\n\
#define check_active(p)	    ((psw[p].fsp != FS_STOP)/* && (psw[p].fsp != FS_ERROR)*/)\n\
#define check_passive(p)    ((psw[p].fsp == FS_STOP)/* || (psw[p].fsp == FS_ERROR)*/)\n\
#define resetT(p)            psw[p].T = SysTime_cur;\n\
#define timeout(p, N)       if((SysTime_cur - psw[p].T) >= N)\n\
#define SET_BIT(r, b)		(r |= (1<<b))\n\
#define CLR_BIT(r, b)		(r &= ~(1<<b))\n\
";

inline const std::string PROC_SR_NAME(const std::string& proc_name){ return proc_name+"_activate";}

const char C_ATOMIC_BLOCK_START[] = "ATOMIC_BLOCK(ATOMIC_FORCEON){";
const char C_ATOMIC_BLOCK_END[] = "}";

const char C_SYS_TIME_CUR_NAME[] = "SysTime_cur";

const char C_INDENT = '\t';
const char C_PROC_ENUM_NAME[] = "PROCESSES";
const char C_PROC_ENUM_NUM[] = "PROC_NUM";
const char C_PROC_ARRAY_NAME[] = "psw";
const char C_STATE_FUNC_ATTR_NAME[] = "fsp";
const char C_STATE_TIME_ATTR_NAME[] = "T";
const char C_STATE_FUNC_TYPE_NAME[] = "FSP_TYPE";
const char C_COMMON_BKG_PROC_TIME[] = "ic_common_bkg_proc_time";
const char C_STRANS_MACRO[] = "set_newfs";
const char C_PROC_DATA_STRUCT_NAME[] = "S_PROC_DATA_STRUCT";
const char C_SYS_TIME_UPDATE[] = "SysTime_cur = ic_ts_millis();";

const char C_STARTPROC_MACRO[] = "startp";
const char C_STOPPROC_MACRO[] = "stopp";
const char C_CHECK_PROC_STOPPED_MACRO[] = "check_stop";
const char C_RESET_T_MACRO[] = "resetT";
const char C_TIMEOUT_MACRO[] = "timeout";

const char C_COMMENT_FRAME[] = "//=============================================================================";

const char C_STUB_DEFS[] = "";
