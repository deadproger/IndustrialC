#include "iCProcType.h"
#include "CodeGenContext.h"
#include "ParserContext.h"
#include "iCNode.h"
#include "iCProcTypeParam.h"
#include "iCState.h"

iCProcType::iCProcType(const std::string& name, const iCProcTypeParamList& param_list,
	const ParserContext& context): iCNode(context)
{
	this->line_num = context.line();
	this->name = name;
	this->param_list = param_list;
}

iCProcType::~iCProcType()
{
	for (iCVariablesList::iterator i = var_list.begin(); i != var_list.end(); i++)
		delete *i;

	for (iCProcTypeParamList::iterator i = param_list.begin(); i != param_list.end(); i++)
		delete *i;

	for (iCStateList::iterator i = states.begin(); i != states.end(); i++)
		delete *i;
}

void iCProcType::add_states(const iCStateList& states)
{
	this->states = states;
	//todo: if (isr_driven) ? (see iCProcess)
}

bool iCProcType::has_state(const std::string& state_name) const
{
	//TODO: optimize
	for (iCStateList::const_iterator i = states.begin(); i != states.end(); i++)
		if (state_name == (*i)->name)
			return true;
	return false;
}

//todo: remove gen_code
void iCProcType::gen_code(CodeGenContext& context)
{
	context.to_code_fmt("\n");
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.to_code_fmt("//Process type: %s\n", name.c_str());
	context.to_code_fmt("%s\n", C_COMMENT_FRAME);
	context.set_location(line_num, filename);
	context.indent();
}
