#include "Yamllite.h"
#include "CMD_Parse.h"
using namespace std;
using namespace YAMLlite;

int main(int argc, char *argv[])
{
	constexpr std::size_t MAXSIZE = 128;
	char buffer[MAXSIZE];
	auto cmd_info = CMD_PARSE::parse_cmd(argc, argv);
	option _option = _STD get<0>(cmd_info);
	const char* filename = _STD get<1>(cmd_info);
	const char* search_path = _STD get<2>(cmd_info);
	ifstream inputFile{ filename, std::ios::in };    //if wrong .second.c_str()
	if (!inputFile.is_open()) {
		cout << "failed to open " << filename << endl;
	}
	else {
		try {
			std::size_t line_num = 0;
			while (!inputFile.eof())
			{
				inputFile.getline(buffer, MAXSIZE - 1);
				line_num++;
				auto tp = preprocess(line_num, buffer);
				seperate_one_line(std::get<0>(tp), std::get<1>(tp), std::get<2>(tp));
			}
			inputFile.close();
			parsing(_option, search_path);
		}
		catch (const Yaml_Base_Exception& e)
		{
			e.printException();
		}
	}
	//system("pause");
	return 0;
}
