#include <iostream> 
#include <iterator>
#include <string>
#include <vector>
using namespace std;

const int layer = 5;
const int MAXN = 1 << layer;
char node[MAXN];
bool pos[MAXN];
int pos_find(const char c) {
	int index = 1;
	for (; index < MAXN; ++index) {
		if (node[index] == c)
			break;
	}
	return index;
}
void Split(const string& s, const string& delimiter, vector<string>* result)
{
	size_t last = 0;
	size_t index = s.find_first_of(delimiter, last);
	while (index != string::npos)
	{
		result->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delimiter, last);
	}
	if (index - last > 0)
	{
		result->push_back(s.substr(last, index - last));
	}
}
int main()
{
	string _cmd;
	getline(cin, _cmd);
	node[1] = _cmd[0];
	pos[1] = true;
	char parent;
	int parent_pos;
	vector<string> result;
	Split(_cmd, " ", &result);
	vector<string>::iterator it = result.begin();
	it++;
	for (; it != result.end(); ++it) {
		string cmd = *it;
		if (cmd.size() == 4) {
			parent = cmd[2];
			parent_pos = pos_find(parent);
			if (pos[2 * parent_pos]) {
				node[2 * parent_pos + 1] = cmd[0];
				pos[2 * parent_pos + 1] = true;
			}
			else {
				node[2 * parent_pos] = cmd[0];
				pos[2 * parent_pos] = true;
			}
		}
		else {
			parent = cmd[2];
			parent_pos = pos_find(parent);
			if (cmd[3] == 'L') {
				node[2 * parent_pos] = cmd[0];
				pos[2 * parent_pos] = true;
			}
			else {
				node[2 * parent_pos + 1] = cmd[0];
				pos[2 * parent_pos + 1] = true;
			}
		}
	}
	//print
	int cur = 1;
	for (int i = 0; i < layer; ++i) {
		for (int j = 0; j < (1 << (layer - i - 1)); ++j)
			cout << " ";
		for (int j = 1; j <= 1 << i; ++j) {
			cout << node[cur++];
			for (int k = 0; k < (1 << (layer - i)) - 1; ++k)
				cout << " ";
		}
		cout << endl;
	}
	system("pause");
	return 0;
}