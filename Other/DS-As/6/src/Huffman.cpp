#include "MIN_HEAP.h"

void HuffmanCode(char codes[], double weights[], int size);
int main()
{
	cout << "Test1:" << endl; counter++;
	char codes1[5] = { 'a', 'b', 'c', 'd', 'e' };
	double weights1[5] = { 0.42, 0.19, 0.07, 0.15, 0.17 };
	HuffmanCode(codes1, weights1, 5);
	cout << "\n\n" << endl;
	cout << "Test2:" << endl; counter++;
	char codes2[6] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	double weights2[6] = { 0.07, 0.24, 0.16, 0.30, 0.14, 0.09 };
	HuffmanCode(codes2, weights2, 6);
	cout << "\n\n" << endl;
	cout << "Test3:" << endl; counter++;
	char codes3[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
	double weights3[10] = { 0.07, 0.24, 0.06, 0.12, 0.02, 0.09, 0.10, 0.11, 0.14, 0.05 };
	HuffmanCode(codes3, weights3, 10);

	cout << endl;
	system("pause");
	return 0;
}

void HuffmanCode(char codes[], double weights[], int size) {
	cout << "codes and weights:" << endl;
	for (int i = 0; i < size; ++i)
		cout << codes[i] << ": " << weights[i] << endl;
	cout << endl;
	MinHeap hp;
	hp.construct(codes, weights, size);
	while (hp.getSize() != 1) {
		Node *min1 = hp.remove_top();
		Node *min2 = hp.remove_top();
		Node* add_result = (*min1) + (*min2);
		hp.insert(*add_result);
	}
	Node *root = hp.remove_top();
	char** hcodes = new char*[size];
	root->getCode(hcodes, codes, weights, size);//Huffman Tree has been released
	for (int i = 0; i < size; ++i)
		delete[] hcodes[i];
	delete[] hcodes;
}
