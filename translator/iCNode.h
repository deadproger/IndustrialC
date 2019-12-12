#pragma once

#include "common.h"

class iCVariable;
class CodeGenContext;
class ParserContext;
class iCIdentifierInProcType;
class iCProcTypeParam;
class iCProcTypeInstantiation;

enum ICNODE_TYPE
{
	CCODE 						,
	CCODELINE 					,

	ICASSIGNMENTEXPRESSION 		,
	ICBINARYEXPRESSION     		,
	ICCASTEXPRESSION 			,
	ICDOUBLE         			,
	ICEXPRESSION 				,
	ICPOSTFIXEXPRESSION         ,
	ICPRIMARYEXPRESSION         ,
	ICIDENTIFIER                ,
	ICSTRING                    ,
	ICSUBSCRIPTEXPRESSION       ,
	ICLOGICCONST                ,
	ICINTEGER 					,
	ICUNARYEXPRESSION       	,
	ICPROCSTATUSCHECK       	,
                            	
	ICATOMICBLOCK           	,
	CCODESTATEMENT          	,
	ICCASESTATEMENT         	,
	ICCOMPOUNDSTATEMENT 		,
	ICDEFAULTSTATEMENT  		,
	ICEXPRESSIONSTATEMENT 		,
	ICFUNCTIONCALL      		,
	ICIFELSESTATEMENT   		,
	ICITERATIONSTATEMENT		, 
	ICJUMPSTATEMENT     		,
	ICRESETTIMEOUTSTATEMENT 	,
	ICRESTARTSTATEMENT      	,
	ICSELECTIONSTATEMENT    	,
	ICSTARTHPSTATEMENT      	,
	ICSTARTPROCSTATEMENT    	,
	ICSTATEMENT             	,
	ICSTATETRANSITION       	,
	ICSTOPHPSTATEMENT 			,
	ICSTOPPROCSTATEMENT     	,
	ICSWITCHSTATEMENT       	,
	ICTIMEOUT               	,
                            	
	ICDECLARATION           	,
	ICFUNCTION              	,
	ICHYPERPROCESS 				,
	ICINITIALIZER           	,
	ICMCUIDENTIFIER         	,
	ICNODE                  	,
	ICPROCESS               	,
	ICPROGRAM               	,
	ICPROGRAMITEM           	,
	ICPROGRAMITEMSLIST 			,
	ICSTATE                 	,
	ICVARIABLE              	,
	ICVARIABLEDECLARATION   	,
                            	
	ICIDENTIFIERINPROCTYPE  	,
	ICPROCTYPE              	,
	ICPROCTYPEINSTANTIATION 	,
	ICPROCTYPEPARAM 			,
	ICPROCTYPEPARAMUSAGE 		,
};

//=================================================================================================
//Base class for all AST nodes
//=================================================================================================
class iCNode 
{
public:

	//Nodes location in the source code
	unsigned long line_num;
	unsigned long col_num;
	std::string filename;
	
	std::string pre_comment;
	std::string post_comment;

	iCNode(){}
	iCNode(const ParserContext& context);
	virtual ~iCNode() {}

	virtual void gen_code(CodeGenContext& context) = 0;
	virtual void second_pass() {}
	
	virtual std::vector<iCNode*> get_issues() {std::vector<iCNode*> issues; return issues;};
	
	virtual int wcet() { return 0; }
	
	virtual ICNODE_TYPE node_type() { return ICNODE; }

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
class iCExpression : public virtual iCNode {};
class iCDeclaration : public iCBlockItem, public iCProgramItem, public iCStatement {};
class iCHyperprocess;
class iCProcType;
class iCProcess;
class iCState;
class iCIdentifier;
typedef std::vector<iCNode*> NodesList;
typedef std::vector<iCBlockItem*> iCBlockItemsList;
typedef std::vector<iCDeclaration*> iCDeclarationList;
typedef std::ostringstream iCStream;
typedef std::list<std::string> iCStringList;
typedef std::map<std::string, iCHyperprocess*> iCHyperprocessMap;
typedef std::map<std::string, iCProcType*> iCProctypeMap;
typedef std::map<std::string, iCProcTypeInstantiation*> iCProctypeInstantiationMap;
typedef std::map<std::string, iCProcess*> iCProcessMap;
typedef std::vector<iCState*> iCStateList;
typedef std::list<iCVariable*> iCVariablesList;
typedef std::list<iCIdentifier*> iCIdentifierList;
typedef std::list<iCIdentifierInProcType*> iCIdentifierInProcTypeList;
typedef std::list<iCProcTypeParam*> iCProcTypeParamList;
