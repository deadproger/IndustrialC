#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCState;

//auxiliary type
struct iCProcBody
{
	iCDeclarationList decls; //does not own
	StateList states; //does not own
};

//=================================================================================================
//Process
//=================================================================================================
class iCProcess : public iCNode, public iCProgramItem
{
	bool isr_driven;
	bool _has_timeouts;
public:
    std::string name; 
	std::string activator;
    StateList states;
	const iCState* start_state; //does not own
	const iCState* stop_state; //does not own
	//iCDeclarationList decls;
	
	iCProcess(const std::string& name, const ParserContext& context);
	virtual ~iCProcess();

	void set_hp(const std::string& hp_name) 
	{
		activator = hp_name;
		if(0 != activator.compare("background"))
			isr_driven = true;
	}
	//void add_state(iCState* state);
	//void add_decl(iCDeclaration* decl) {decls.push_back(decl);}
	void add_states(const StateList& states);
	//void add_decls(const iCDeclarationList& decls) {this->decls = decls;}
	bool has_state(const std::string& state_name) const;
	bool is_isr_driven() const {return isr_driven;}
	virtual void gen_code(CodeGenContext& context);
	void gen_timeout_code(CodeGenContext& context);
	bool has_timeouts()const{return _has_timeouts;}
};
