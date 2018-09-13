#include "CMD_Parse.h"
namespace YAMLlite
{

	namespace CMD_PARSE
	{

		_STD tuple<YAMLlite::option, const char*, const char*> parse_cmd(int argc, char *argv[])
		{

			//return _STD make_tuple(option::json, "./smpl.yml", "./smpl.yml");
			//return _STD make_tuple(option::find, "./smpl.yml", "array[5].key[2].key1[1]");

			static const std::function<bool(const char*, char)>
				contain_char = [](const char* str, char c)->bool
			{
				const int length = strlen(str);
				for (int i = 0; i < length; i++)
					if (str[i] == c)
						return true;
				return false;
			};//end std::function contain_char(const char*, char);

			static const std::function<bool(const char*)>
				check_validation = [](const char* file_name)->bool
			{
				//check whether the file name contains invalid chars
				const char invalid_char[8] = { '/', ':', '*', '?', '"', '<', '>', '|' };
				for (int i = 0; i < 8; i++)
				{
					if (contain_char(file_name, invalid_char[i]))
						return false;
				}

				//check whether the extension is valid
				const char *ptr = strstr(file_name, ".yml");
				if (ptr && '\0' == *(ptr + 4))
					return true;

				return false;
			};//end std::function check_validation(const char*);

			//yamlite.exe xxx.yml
			if (2 == argc)
			{
				if (check_validation(argv[1]))
					return _STD make_tuple(option::parse, argv[1], nullptr);
				else
				{
					_STD cout << "File name invalid." << _STD endl;
					exit(1);
				}
			}

			//yamlite.exe -parse/-json xxx.yml
			if (3 == argc)
			{
				if (!check_validation(argv[2]))
				{
					_STD cout << "File name invalid." << _STD endl;
					_sleep(2000);
					exit(1);
				}

				if (0 == strcmp(argv[1], "-parse"))
					return _STD make_tuple(option::parse, argv[2], nullptr);
				else if (0 == strcmp(argv[1], "-json"))
					return _STD make_tuple(option::json, argv[2], argv[2]);
				else {
					_STD cout << "Option invalid." << _STD endl;
					exit(1);
				}
			}

			//yamlite.exe -find search_path xxx.yml
			if (4 == argc)
			{
				if (!check_validation(argv[3]))
				{
					_STD cout << "File name invalid." << _STD endl;
					exit(1);
				}
				if (0 == strcmp(argv[1], "-find"))
					return _STD make_tuple(option::find, argv[3], argv[2]);
				else {
					_STD cout << "Option invalid." << _STD endl;
					exit(1);
				}
			}

			std::cout << "Parameter format error." << std::endl;
			exit(1);

		}//end function parse_cmd(int, char*[]);

	}//end namespace CMD_PARSE

}//end namespace YAMLlite
