#include "iCNode.h"
#include "ParserContext.h"

bool had_errors = false;

iCNode::iCNode( const ParserContext& context )
 :	line_num(context.line()),
	col_num(context.column()),
	filename(context.file_name())
{
	if(filename.empty())
	{
		//err_msg("dev warning: empty filename in node");
	}

}

void iCNode::err_msg( const char* format, ... ) const
{
	had_errors = true;
	char buffer[ERR_MSG_BUFFER_SIZE];
	va_list args;
	va_start(args,format);
	vsprintf(buffer,format, args);
	va_end(args);
	std::cout<<filename<<":"<<line_num/*<<":"<<col_num*/<<": error: "<<buffer<<std::endl;
}
/*iCMCUIdentifier::iCMCUIdentifier( const std::string& name ): name(name){}*/