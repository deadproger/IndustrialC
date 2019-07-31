#include "iCProcTypeParam.h"

void iCProcTypeParam::gen_code(CodeGenContext& context)
{
	std::cout << "iCProcTypeParam.gen_code proctype=" << context.proctype_instantiation->name << 
		", scoped_value="<< scoped_value << std::endl;
	context.to_code_fmt(scoped_value.c_str());
}
