#include "iCProcTypeParamUsage.h"

void iCProcTypeParamUsage::gen_code(CodeGenContext& context)
{
	std::cout << "iCProcTypeParamUsage.gen_code called, name=" << name << ", param_name=" << param->name << std::endl;
	param->gen_code(context);
}
