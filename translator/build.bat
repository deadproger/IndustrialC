@echo --------------------------------Build------------------------------------
del industrialc.exe
win_bison --report=all --report-file=bisonreport -d -o parser.cpp parser.h 
win_flex -otokens.cpp tokens.l 
g++ -static-libgcc -static-libstdc++ -oindustrialc parser.cpp tokens.cpp main.cpp ^
CCode.cpp ^
CCodeStatement.cpp ^
CodeGenContext.cpp ^
iCAssignmentExpression.cpp ^
iCBinaryExpression.cpp ^
iCDouble.cpp ^
iCExpressionStatement.cpp ^
iCHyperprocess.cpp ^
iCIdentifier.cpp ^
iCInteger.cpp ^
icmemory.cpp ^
iCPostfixExpression.cpp ^
iCPrimaryExpression.cpp ^
iCProcess.cpp ^
iCProcStatusCheck.cpp ^
iCProgram.cpp ^
iCScope.cpp ^
iCStartHPStatement.cpp ^
iCStartProcStatement.cpp ^
iCState.cpp ^
iCStateTransition.cpp ^
iCStopHPStatement.cpp ^
iCStopProcStatement.cpp ^
iCTimeout.cpp ^
iCUnaryExpression.cpp ^
iCVariableDeclaration.cpp ^
ParserContext.cpp ^
iCNode.cpp ^
iCCompoundStatement.cpp ^
iCMCUIdentifier.cpp ^
iCSelectionStatement.cpp ^
iCVariable.cpp ^
iCFunction.cpp ^
iCFunctionCall.cpp ^
iCLogicConst.cpp ^
iCSubscriptExpression.cpp ^
iCInitializer.cpp ^
iCString.cpp ^
iCIterationStatement.cpp ^
iCAtomicBlock.cpp ^
iCReturnStatement.cpp
@echo --------------------------------Done-------------------------------------

 
