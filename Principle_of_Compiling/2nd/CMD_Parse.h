#pragma once
#ifndef _CMD_PARSE_H
#define _CMD_PARSE_H
#include "Yamllite.h"
#ifndef _STD 
#define _STD ::std::
#endif
namespace YAMLlite
{

	namespace CMD_PARSE
	{
		/*
		 * Description:
		 *     parse the command line input
		 * Parameter list:
		 *     from the command line
		 * Return value:
		 *     option      : 0(-parse), 1(-json), 2(-find)
		 *     const char* : filename
		 *     const char* : search_path if option is 2(-find), filename if option is 1(-json)
		 * Exception Handling:
		 *     output the error message or prompt and exit(1)
		**/
		_STD tuple<YAMLlite::option, const char*, const char*> parse_cmd(int argc, char *argv[]);


	}//end namespace CMD_PARSE

}//end namespace YAMLlite

#endif // !_CMD_PARSE_H
