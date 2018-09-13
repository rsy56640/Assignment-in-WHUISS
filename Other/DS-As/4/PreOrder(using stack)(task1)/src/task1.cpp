#include <iostream>
using namespace std;

const int MAXN = 100;
struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int _val = 0) : val(_val), left(NULL), right(NULL) {}
};
class mystk {
private:
	TreeNode** MyContainer = new TreeNode*[MAXN];
	int size;
public:
	mystk() :size(0) {}
	void push(TreeNode* v) {
		if (v == NULL)return;
		MyContainer[size++] = v;
	}
	void pop() { cout << (MyContainer[--size])->val << " "; }
	TreeNode* top() { return MyContainer[size - 1]; }
	bool isEmpty() { return size == 0; }
};
mystk stk;
void PreorderTraversal(TreeNode* root) {
	if (root == NULL)return;
	stk.push(root);
	while (!stk.isEmpty()) {
		TreeNode* node = stk.top();
		stk.pop();
		stk.push(node->right);
		stk.push(node->left);
	}
}
int main()
{
	TreeNode* root = new TreeNode(0);
	/*
	*	construct a binary tree
	*
	*/
	PreorderTraversal(root);

	system("pause");
	return 0;
}