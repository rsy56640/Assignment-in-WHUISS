#include "Elliptical_Curve.h"
using namespace std;
namespace Elliptic_Curve
{
	int tmain();
}




int main()
{
	//Elliptic_Curve::tmain();
	using namespace Elliptic_Curve;

	///*
	//Process::
	E e1{ 1,1,37 };
	int priv1 = 25;
	E e2{ 1,1,97 };
	int priv2 = 7;
	initial_environment();
	auto res1 = generate(e1, priv1);
	auto res2 = generate(e2, priv2);

	auto Alice = Process::Factory(string("Alice"), e1, res1.first, res1.second, priv1);
	auto Bob = Process::Factory(string("Bob"), e2, res2.first, res2.second, priv2);
	Alice->sendPK(Bob, public_key(e1, res1.first, res1.second));
	Bob->sendMessage(Alice, "hello");
	Alice->readAllMessage();
	/**/

	system("pause");
	return 0;
}