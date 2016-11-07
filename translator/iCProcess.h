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
public:
    std::string name; 
	std::string activator;
    StateList states;
	const iCState* start_state; //does not own
	const iCState* stop_state; //does not own
	//iCDeclarationList decls;
	
	iCProcess(const std::string& name, const ParserContext& context);
	virtual ~iCProcess();

	void set_hp(const std::string& hp_name) {activator = hp_name;}
	void add_state(iCState* state) {states.push_back(state);}
	//void add_decl(iCDeclaration* decl) {decls.push_back(decl);}
	void add_states(const StateList& states) {this->states = states;}
	//void add_decls(const iCDeclarationList& decls) {this->decls = decls;}
	bool has_state(const std::string& state_name) const;

	virtual void gen_code(CodeGenContext& context);
};
