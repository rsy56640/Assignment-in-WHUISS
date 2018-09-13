#pragma once
#ifndef MIN_HEAP
#define MIN_HEAP
#include <iostream> 
using namespace std;

static int counter = 0;		//前两组打印时按5层打印，看起来紧凑一些......
const int layer = 6;		//如果是5，第3组测试会溢出
const int MAXN = 1 << layer;
class Node;
class MyStack {
private:
	Node* stk[MAXN];
	int size;
public:
	MyStack() :size(0) {}
	void push(Node* v) { stk[size++] = v; }
	Node* pop() { return stk[--size]; }
	bool empty() { return size > 0; }
};
class MyQueue {
private:
	pair<Node*, int> q[MAXN];
	//first point to the first pos, while last point to the next pos of the last elemet.
	//that is to say, it only allows the size ranging from 0~(n-1)
	int first, last;
public:
	MyQueue() : first(0), last(0) {}
	int length()const { return (last - first + MAXN) % MAXN; }
	void push(pair<Node*, int> v) { q[last++] = v; last %= MAXN; }
	pair<Node*, int> front() { return q[first]; }
	void pop() { first++; first %= MAXN; }
	bool empty() { return length() == 0; }
};

// template<class T>
// class MinHeap;

//template<class T,
//_Pred op = less<typename T::vaule_type>>
class Node {
	friend class MinHeap;
	//friend void HuffmanCode(char codes[], double weights[], int size);
	friend ostream& operator<<(ostream& out, const Node& node);
private:
	double frequence;
	char code;
	Node* left;
	Node* right;
public:
	Node(double _frequence, char _code, Node* _left = nullptr, Node* _right = nullptr) :
		frequence(_frequence), code(_code), left(_left), right(_right) {}
	bool operator<(const Node& a) { return frequence < a.frequence; }
	bool operator>(const Node& a) { return frequence > a.frequence; }
	Node* operator+(Node& a) {//put left be smaller
		if (frequence < a.frequence)
			return new Node(frequence + a.frequence, 0, this, &a);
		else return new Node(frequence + a.frequence, 0, &a, this);
	}
	void getCode(char** hcode, char codes[], double weights[], int size);
};

ostream& operator<<(ostream& out, const Node& node) {
	out << node.code << ":" << node.frequence << "  ";
	return out;
}

void Node::getCode(char** hcode, char codes[], double weights[], int size) {
	//utilize queue to
	char cd[MAXN];
	for (int i = 0; i < MAXN; ++i)cd[i] = 1;
	MyQueue q;
	q.push(make_pair(this, 0));
	cout << "Huffman code:" << endl;
	while (!q.empty()) {
		pair<Node*, int> node = q.front();
		q.pop();
		//if it's a leaf, store code and num
		//Then create Huffman code
		if (node.first->code != 0) {
			char c = node.first->code;
			int pos = node.second;
			cd[node.second] = c;
			//find the num of hcodes in codes
			int num = 0;
			for (; num < size; ++num)
				if (codes[num] == c)
					break;
			//find the bits of corresponding character
			int _pos = pos + 1;//find the number of bits, cuz same level has same bits
			int bit = -1;
			while (_pos) {
				++bit;
				_pos >>= 1;
			}
			hcode[num] = new char[bit];
			++pos;
			//create Huffman code
			for (int i = bit - 1; i >= 0; --i) {
				hcode[num][i] = (pos & 1) ? '1' : '0';
				pos >>= 1;
			}
			//print Huffman code
			cout << codes[num] << ": ";
			for (int i = 0; i < bit; ++i)
				cout << hcode[num][i];
			cout << endl;
		}
		else cd[node.second] = '0';//output 0 if it's not a leaf
		if (node.first->left != nullptr)
			q.push(make_pair(node.first->left, 2 * node.second + 1));
		if (node.first->right != nullptr)
			q.push(make_pair(node.first->right, 2 * node.second + 2));
	}
	//print Huffman Tree
	cout << endl;
	int cur = 0;
	////////////////////////////////////前两次测试按layer=5
	int _layer = layer;
	if (counter < 3)_layer = 5;
	////////////////////////////////////前两次测试按layer=5
	for (int i = 0; i < _layer; ++i) {
		for (int j = 0; j < (1 << (_layer - i - 1)); ++j)
			cout << " ";
		for (int j = 1; j <= 1 << i; ++j) {
			char val = cd[cur++];
			if (val == 1)
				cout << " ";
			else cout << val;
			for (int k = 0; k < (1 << (_layer - i)) - 1; ++k)
				cout << " ";
		}
		cout << endl;
	}
	//delete Huffman Tree
	MyStack stk;
	stk.push(this);
	while (!stk.empty()) {
		Node* node = stk.pop();
		if (node->right != nullptr)
			stk.push(node->right);
		if (node->left != nullptr)
			stk.push(node->left);
		delete node;
	}
}
//template<class T>
class MinHeap {
private:
	Node *heap[MAXN];
	int size;
public:
	MinHeap() :size(0) {}
	int getSize() { return size; }
	void construct(char codes[], double weights[], int _size);
	void insert(Node& node);
	Node* remove_top();
};
//typedef MIN_HEAP<char> min_hp;

void MinHeap::construct(char codes[], double weights[], int _size) {
	size = _size;
	for (int i = 1; i <= _size; ++i)
		heap[i] = new Node{ weights[i - 1],codes[i - 1] };
	//handle from the last father
	for (int _cur = _size / 2; _cur > 0; --_cur) {
		int cur = _cur;
		int son = 2 * cur;
		while (son <= _size) {
			//able to step down
			if (*heap[cur] > *heap[son] ||
				(son + 1 <= _size && *heap[cur] > *heap[son + 1])) {
				//no right
				if (son + 1 > _size) {
					swap(heap[cur], heap[son]);
					cur = son;
					son *= 2;
					continue;
				}
				//compare the two child
				if (*heap[son] < *heap[son + 1]) {
					swap(heap[cur], heap[son]);
					cur = son;
					son *= 2;
					continue;
				}
				else {
					swap(heap[cur], heap[son + 1]);
					cur = son + 1;
					son = 2 * cur;
				}
			}
			else break;
		}
	}
}

//template<class T>
void MinHeap::insert(Node& node) {
	++size;
	heap[size] = &node;
	int cur = size;
	while (cur > 1 && *heap[cur] < *heap[cur / 2]) {
		swap(heap[cur], heap[cur / 2]);
		cur /= 2;
	}
}

//template<class T>
Node* MinHeap::remove_top() {
	swap(heap[1], heap[size]);
	//pretend to delete
	--size;
	int cur = 1;
	int son = 2 * cur;
	while (son <= size) {
		//able to step down
		if (*heap[cur] > *heap[son] ||
			(son + 1 <= size && *heap[cur] > *heap[son + 1])) {
			//no right
			if (son + 1 > size) {
				swap(heap[cur], heap[son]);
				cur = son;
				son *= 2;
				continue;
			}
			//compare the two child
			if (*heap[son] < *heap[son + 1]) {
				swap(heap[cur], heap[son]);
				cur = son;
				son *= 2;
				continue;
			}
			else {
				swap(heap[cur], heap[son + 1]);
				cur = son + 1;
				son = 2 * cur;
			}
		}
		else break;
	}
	return heap[size + 1];
}
#endif // !1
