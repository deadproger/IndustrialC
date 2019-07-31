@echo --------------------------------Build------------------------------------
del industrialc.exe
win_bison --report=all --report-file=bisonreport -d -o parser.cpp parser.y 
win_flex -otokens.cpp tokens.l 
win_bison --report=all --report-file=bisonprepreport -d -oprep_parser.cpp prep_parser.y 
win_flex -oprep_tokens.cpp prep_tokens.l 
g++ -static-libgcc -static-libstdc++ -oindustrialc parser.cpp tokens.cpp main.cpp ^
prep_parser.cpp prep_tokens.cpp ^
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
iCProcType.cpp ^
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
iCJumpStatement.cpp ^
iCResetTimeoutStatement.cpp ^
iCCastExpression.cpp ^
iCRestartStatement.cpp ^
iCProcTypeInstantiation.cpp ^
iCIdentifierInProcType.cpp ^
iCProcTypeParam.cpp ^
iCProcTypeParamUsage.cpp
@echo --------------------------------Done-------------------------------------

 
