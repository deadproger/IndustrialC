#pragma once

#include "iCNode.h"
class CodeGenContext;
class ParserContext;
class iCState;

//=================================================================================================
//Process
//=================================================================================================
class iCProcess : public iCNode, public iCProgramItem
{
	bool isr_driven;
	bool _has_timeouts;
	bool isr_referenced;
public:
	void mark_isr_referenced() const {const_cast<iCProcess*>(this)->isr_referenced = true;}//hack
	bool is_isr_referenced()const{return isr_referenced;}

	std::string name;
	std::string activator;
	iCStateList states;
	const iCState* start_state; //does not own
	const iCState* stop_state; //does not own
	
	iCProcess(const std::string& name, const ParserContext& context);
	virtual ~iCProcess();

	void set_hp(const std::string& hp_name) 
	{
		activator = hp_name;
		if(0 != activator.compare("background"))
			isr_driven = true;
	}
	void add_states(const iCStateList& states);
	bool has_state(const std::string& state_name) const;
	bool is_isr_driven() const {return isr_driven;}
	virtual void gen_code(CodeGenContext& context);
	void gen_timeout_code(CodeGenContext& context);
	bool has_timeouts()const{return _has_timeouts;}

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCVariable name: " + name;}
#endif//DEBUG
};
