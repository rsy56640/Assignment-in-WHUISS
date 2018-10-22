#include <vector>
#include <functional>
#include <iostream>
#include <numeric>

void test()
{
	const std::vector<std::vector<double>> w1 = { { 0, 1 }, { 0, -1 } };
	const std::vector<std::vector<double>> w2 = { { 1, 0 }, { -1, 0 } };

	const std::function<double(double)> h[3] = {
		[](double) { return 1.0; },
		[](double x) { return 2 * x; },
		[](double x) { return 4 * x * x - 2; },
	};

	std::function<double(const std::vector<double>& X)> phi[3] = {
		[&h](const std::vector<double>& X) { return h[0](X[0]) * h[0](X[1]); },
		[&h](const std::vector<double>& X) { return h[0](X[0]) * h[2](X[1]); },
		[&h](const std::vector<double>& X) { return h[2](X[0]) * h[0](X[1]); },
	};

	const std::function<double(const std::vector<double>& X, const std::vector<double>& Xk)> K =
		[&phi](const std::vector<double>& X, const std::vector<double>& Xk)
	{
		return std::accumulate(std::begin(phi), std::end(phi), 0.0,
			[&X, &Xk](double val, const std::function<double(const std::vector<double>& X)>& phi)
		{ return val + phi(X) * phi(Xk); });
	};

	std::function<double(const std::vector<double>& X)> k = [](const std::vector<double>&) { return 0; };

	for (int times = 0; times < 1; times++)
		for (int i = 0; i < 2; i++)
		{
			if (k(w1[i]) <= 0)
				k = [k, i, &w1, &K](const std::vector<double>& X) { return k(X) + K(X, w1[i]); };
			if (k(w2[i]) >= 0)
				k = [k, i, &w2, &K](const std::vector<double>& X) { return k(X) - K(X, w2[i]); };
		}

	// check
	for (int i = 0; i < 2; i++)
	{
		std::cout << "w1[" << i << "]: " << k(w1[i]) << std::endl;
		std::cout << "w2[" << i << "]: " << k(w2[i]) << std::endl;
	}

}

int main()
{
	test();
	printf("\n%s\n", "done");
	getchar();
	return 0;
}