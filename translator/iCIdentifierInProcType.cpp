#include "iCIdentifierInProcType.h"
#include "CodeGenContext.h"

void iCIdentifierInProcType::gen_code(CodeGenContext& context)
{
	std::cout << "iCIdentifierInProcType.gen_code called, name=" << name << std::endl;
	context.set_location(line_num, filename); //todo: remove?

	//context should assuredly have proctype_instantiation here, otherwise there's error in translator
	if (NULL == context.proctype_instantiation)
	{
		err_msg("no proctype_instantiation found in the code gen context");
		return;
	}
	std::cout << "iCIdentifierInProcType.gen_code proctype name=" << context.proctype_instantiation->name << std::endl;
	context.to_code_fmt("%s_%s", context.proctype_instantiation->name.c_str(), name.c_str());
}
