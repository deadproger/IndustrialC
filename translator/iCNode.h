#pragma once

#include "common.h"
class CodeGenContext;
class ParserContext;

//=================================================================================================
//Abstract node
//=================================================================================================
class iCNode 
{
public:
	unsigned long line_num;
	unsigned long col_num;
	std::string filename;
	iCNode(){}
	iCNode(const ParserContext& context);
    virtual ~iCNode() {}
	virtual void gen_code(CodeGenContext& context) = 0;
	virtual void second_pass() {}

#ifdef DEBUG
	virtual const std::string& identify() const { return "iCNode";}
#endif//DEBUG

	void err_msg(const char* format, ...) const;
	void warning_msg( const char* format, ... ) const;
};

//=================================================================================================
//Auxiliary nodes and node lists
//=================================================================================================
class iCStatement : public virtual iCNode 
{
public:
	virtual ~iCStatement(){}
	virtual bool is_compound() {return false;}
};
class iCBlockItem : public virtual iCNode {};
class iCProgramItem {};
class iCProgramItemsList {};
class iCExpression : public virtual iCNode  {};
class iCDeclaration : public iCBlockItem, public iCProgramItem, public iCStatement {};
class iCHyperprocess;
class iCProcess;
class iCState;
class iCIdentifier;
typedef std::vector<iCNode*> NodesList;
typedef std::vector<iCBlockItem*> iCBlockItemsList;
typedef std::vector<iCDeclaration*> iCDeclarationList;
typedef std::ostringstream iCStream;
typedef std::list<std::string> iCStringList;
typedef std::map<std::string, iCHyperprocess*> iCHyperprocessMap;
typedef std::map<std::string, iCProcess*> iCProcessList;
typedef std::vector<iCState*> StateList;
typedef std::vector<iCIdentifier*> iCIdentifierList;