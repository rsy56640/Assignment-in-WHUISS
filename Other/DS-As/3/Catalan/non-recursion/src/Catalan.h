#pragma once
#include<iostream>
using namespace std;

static const int Catalan[10] = { 1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862 };
const int num = 9;
static int _offset = 0;


class MyString {
private:
	char* str;
public:
	MyString() { str = nullptr; };
	char* operator=(const char* s);
	MyString operator=(const MyString &s);
	MyString(const MyString &s);
	void print();
	~MyString();
	friend MyString operator+ (const MyString &s1, const MyString &s2);
	friend ostream& operator<<(ostream& out, const MyString &s);
};
char* MyString::operator=(const char* s) {
	if (str) delete[]str;
	if (s) {
		str = new char[strlen(s) + 1];
		strcpy(str, s);
	}
	else str = NULL;
	return str;
}
MyString MyString::operator=(const MyString &s) {
	if (str == s.str)return *this;
	if (str)delete[]str;
	if (s.str) {
		str = new char[strlen(s.str) + 1];
		strcpy(str, s.str);
	}
	else str = NULL;
	return *this;
}
MyString::MyString(const MyString &s) {
	if (s.str) {
		str = new char[strlen(s.str) + 1];
		strcpy(str, s.str);
	}
	else str = nullptr;
}
MyString::~MyString() {
	if (str)
		delete[]str;
}
ostream& operator<< (ostream& out, const MyString &s) {
	for (int i = 0; i < strlen(s.str); ++i)
		out << s.str[i];
	return out;
}
MyString operator+ (const MyString &s1, const MyString &s2) {
	MyString temp;
	int _size1 = strlen(s1.str);
	int _size2 = strlen(s2.str);
	temp.str = new char[_size1 + _size2 + 3];
	for (int i = 0; i < _size1; ++i)
		temp.str[i] = (i % 2 == 1) ? (' ') : (s1.str[i] + 1);
	temp.str[_size1] = '1';
	temp.str[_size1 + 1] = ' ';
	for (int i = 0; i < _size2; ++i)
		temp.str[i + _size1 + 2] = (i % 2 == 1) ? (' ') : (s2.str[i] + _offset);
	temp.str[_size1 + _size2 + 2] = '\0';
	return temp;
}

void MyString::print() {
	for (int i = 0; i < strlen(str); ++i)
		printf("%c", str[i]);
	printf("\n");
}