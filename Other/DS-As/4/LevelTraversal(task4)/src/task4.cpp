#include <iostream> 
#include <vector>
#include <iterator>
using namespace std;

const int layer = 5;
const int MAXN = 1 << layer;
struct TreeNode {
	char word;
	TreeNode *left;
	TreeNode *right;
	TreeNode(char _word = NULL) : word(_word), left(nullptr), right(nullptr) {}
};
class myQueue {
private:
	int size;
	TreeNode* q[MAXN];
	//first point to the first pos, while last point to the next pos of the last elemet.
	//that is to say, it only allows the size ranging from 0~(n-1)
	int first, last;
public:
	myQueue() :size(0), first(0), last(0) {}
	int length()const { return (last - first + MAXN) % MAXN; }
	bool isEmpty() { return size == 0; }
	void push(TreeNode& val) { q[last++] = &val; last %= MAXN; size++; }
	TreeNode* front() { return q[first]; }
	void pop() { first++; first %= MAXN; size--; }
};
class Solution {
public:
	vector<char> levelTraversal(TreeNode* root, int scale, int level) {
		vector<char> result;
		myQueue mq;
		mq.push(*root);
		if (level == 1) {
			result.push_back(root->word);
			return result;
		}
		//reach the level
		for (int i = 2; i < 1 << level; i <<= 1) {
			for (int j = i; j < 2 * i;) {
				TreeNode* father = mq.front();
				mq.pop();
				if (j++ <= scale)
					mq.push(*(father->left));
				if (j++ <= scale)
					mq.push(*(father->right));
			}
		}
		//store into the result
		while (!mq.isEmpty()) {
			TreeNode* node = mq.front();
			result.push_back(node->word);
			mq.pop();
		}
		//reverse result
		int n = result.size();
		for (int i = 0; i < n / 2; ++i)
			swap(result[i], result[n - 1 - i]);
		return result;
	}
};
class Solution_task3 {
public:
	TreeNode* array2BinaryTree(char* p, int len) {  //BFS utilizing a queue
		myQueue mq;
		TreeNode* root = new TreeNode(p[0]);
		mq.push(*root);
		for (int i = 2; i < len; i <<= 1) {
			for (int j = i - 1; j < (i << 1) - 1 && j < len;) {
				TreeNode* father = mq.front();
				mq.pop();
				TreeNode* node = new TreeNode(p[j++]);
				father->left = node;
				mq.push(*node);
				if (j < len) {
					node = new TreeNode(p[j++]);
					father->right = node;
					mq.push(*node);
				}
				node = nullptr;
			}
		}
		return root;
	}
};
int main()
{
	Solution s;
	Solution_task3 s3;
	char p[] = { 'A','B','C','D','E','F','G','H','I','J','K','L',
		'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	cout << " The binary tree is below." << endl;
	//print
	int cur = 0;
	bool stop = false;
	for (int i = 0; i < layer; ++i) {
		for (int j = 0; j < (1 << (layer - i - 1)); ++j)
			cout << " ";
		if (stop)break;
		for (int j = 1; j <= 1 << i; ++j) {
			if (cur >= 26) { stop = true; break; }
			cout << p[cur++];
			for (int k = 0; k < (1 << (layer - i)) - 1; ++k)
				cout << " ";
		}
		cout << endl;
	}
	int level;
	cout << "Please input the level." << endl;
	cin >> level;
	TreeNode* root = s3.array2BinaryTree(p, 26);
	vector<char> result = s.levelTraversal(root, 26, level);
	ostream_iterator<char>_oit(cout, " ");
	copy(result.begin(), result.end(), _oit);

	system("pause");
	return 0;
}