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

const char C_MACRO_ROUTINES[] = "\
#include <avr/io.h> \n\
#include <avr/interrupt.h>\n\
volatile unsigned long ovf_cnt = 0;\n\
unsigned long millis()\n\
{\n\
	unsigned char sreg = SREG;\n\
	cli();\n\
	unsigned long ovf = ovf_cnt;\n\
	unsigned long tcnt = TCNT0;\n\
	SREG = sreg;\n\
\n\
	//Timer has already overflown, but interrupt has yet to execute\n\
	if ((TIFR0 & _BV(TOV0)) && (tcnt < 255))\n\
		ovf++;\n\
	return (((ovf<<8) + tcnt)*1024)/(F_CPU/1000);\n\
}\n\
\n\
ISR(TIMER0_OVF_vect)\n\
{\n\
	ovf_cnt++;\n\
}\n\
								\n\
#define set_newfs(p, fs)    psw[p].fsp = fs; psw[p].T = SysTime_cur;\n\
#define startp(p)	    psw[p].fsp = FS_START; psw[p].T = SysTime_cur;\n\
#define stopp(p)	    psw[p].fsp = FS_STOP\n\
#define check_active(p)	    ((psw[p].fsp != FS_STOP)/* && (psw[p].fsp != FS_ERROR)*/)\n\
#define check_passive(p)    ((psw[p].fsp == FS_STOP)/* || (psw[p].fsp == FS_ERROR)*/)\n\
#define resetT(p)            psw[p].T = SysTime_cur;\n\
#define timeout(p, N)       if((SysTime_cur - psw[p].T) >= N)\n\
#define SET_BIT(r, b)		(r |= (1<<b))\n\
#define CLR_BIT(r, b)		(r &= ~(1<<b))\n\
";

inline const std::string PROC_SR_NAME(const std::string& proc_name){ return proc_name+"_activate";}

const char C_INDENT = '\t';
const char C_PROC_ENUM_NAME[] = "PROCESSES";
const char C_PROC_ENUM_NUM[] = "PROC_NUM";
const char C_PROC_ARRAY_NAME[] = "psw";
const char C_STATE_FUNC_ATTR_NAME[] = "fsp";
const char C_STRANS_MACRO[] = "set_newfs";
const char C_PROC_DATA_STRUCT_NAME[] = "S_PROC_DATA_STRUCT";
const char C_SYS_TIME_UPDATE[] = "SysTime_cur = millis();";

const char C_STARTPROC_MACRO[] = "startp";
const char C_STOPPROC_MACRO[] = "stopp";
const char C_CHECK_PROC_STOPPED_MACRO[] = "check_stop";
const char C_RESET_T_MACRO[] = "resetT";
const char C_TIMEOUT_MACRO[] = "timeout";

const char C_STUB_DEFS[] = "";/*"#include <ctime>\n\
unsigned long millis()\n\
{\n\
	const long double sysTime = time(0);\n\
	const long double sysTimeMS = sysTime*1000.0;\n\
	return (unsigned long) sysTimeMS;\n\
}";*/
