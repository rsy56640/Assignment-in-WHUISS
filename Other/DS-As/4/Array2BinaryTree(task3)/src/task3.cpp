#include <iostream> 
using namespace std;

const int layer = 4;
const int MAXN = 1 << layer;
struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int _val = 0) : val(_val), left(nullptr), right(nullptr) {}
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
	void push(TreeNode& val) { q[last++] = &val; last %= MAXN; size++; }
	TreeNode* front() { return q[first]; }
	void pop() { first++; first %= MAXN; size--; }
};
class Solution {
public:
	TreeNode* array2BinaryTree(int* p, int len) {  //BFS utilizing a queue
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
	int p[] = { 1,2,3,4,5,6,7,8,9 };
	TreeNode* root = s.array2BinaryTree(p, 9);

	system("pause");
	return 0;
}