#include "iCInitializer.h"
#include "ParserContext.h"
#include "CodeGenContext.h"

//=================================================================================================
//
//=================================================================================================
iCInitializer::~iCInitializer()
{
	for(std::vector<iCExpression*>::iterator i=initializers.begin();i!=initializers.end();i++)
	{
		delete *i;
	}
}

//=================================================================================================
//Code generator
//This method can be recursive in case of multidimensional arrays
//=================================================================================================
void iCInitializer::gen_code( CodeGenContext& context )
{
#ifdef ICDEBUG_TRACE
	std::cout<<"iCInitializer::gen_code " << "...";
	std::cout.flush();
#endif

	context.set_location(line_num, filename);
	
	if(initializers.size() == 1)
	{
		//single assignment expression - tree leaf
		initializers[0]->gen_code(context);
	}
	else
	{
		//array initializer
		context.to_code("{");
		for(std::vector<iCExpression*>::iterator i=initializers.begin();i!=initializers.end();i++)
		{
			(*i)->gen_code(context);//recursion
			context.to_code(", ");
		}
		context.to_code("}");
	}

#ifdef ICDEBUG_TRACE
	std::cout<<"done\n";
	std::cout.flush();
#endif
}