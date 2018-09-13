#include "engine.h"
#pragma comment(lib,"libeng.lib")
#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmat.lib")
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
using namespace std;


namespace Elliptic_Curve
{

	mxArray *convertVectorToMxArray(double** mat,
		const int nb_rows, const int nb_cols);

	int tmain()
	{

		Engine *engine_ptr;

		// open engine  
		if (!(engine_ptr = engOpen("\0")))
		{
			fprintf(stderr, "\nCan't start MATLAB engine\n");
			return EXIT_FAILURE;
		}
		cout << "MATLAB open success." << endl;

		//execute matlab operations  
		using Deleter = std::function<void(Engine*)>;
		std::shared_ptr<Engine> ep{ engine_ptr, engClose };
		auto evalString = std::bind(engEvalString, ep.get(), std::placeholders::_1);

		std::vector<std::string> str =
		{
			"str = ['A', 'B', 'C'];",
			"syms x y",
			"eq1 = y^2 - x^3 - 4.5 * x^2 + 4;",
			"eq2 = y - x - 2;",
			"s = solve(eq1, eq2, x, y)",
			"ezplot(eq1, [-6, 6])\n axis([-8 8 -8 8])\n",
			"hold on",
			"ezplot(eq2, [-6, 6])",
			"X = double(s.x)",
			"Y = double(s.y)",
			"for i = 1:(size(X))\n text(X(i), Y(i), str(i), 'FontSize', 9)\n end\n"
			//"for i = 1:(size(X))\n text(X(i), Y(i), ['(', num2str(X(i),'%.3f'), ', ', num2str(Y(i),'%.3f'), ')'], 'FontSize', 9)\n end\n"
			"hold on",
			"plot([X(3), X(3)],[-Y(3), Y(3)], 'r')",
			"plot([0, 0],[-8, 8], 'k')",
			"plot([-8, 8],[0, 0], 'k')",
			"eq3 = -Y(3) - 0.5 * (x - X(3));",
			"ezplot(eq3)",
			"text(X(3), -Y(3), 'D', 'Fontsize', 9)",
			"a=7",
			"b=[1,2]"
		};
		for (auto const&s : str)
			evalString(s.c_str());


		using Coordinate = std::pair<double, double>;
		auto getDouble = [engine_ptr](const char* c)->decltype(auto)
		{
			//std::shared_ptr<mxArray> ret(engGetVariable(engine_ptr, c), mxDestroyArray);
			//return std::shared_ptr<double>(static_cast<double*>(mxGetData(ret.get())));

			auto ret = engGetVariable(engine_ptr, c);
			double tmp = *(static_cast<double*>(mxGetData(ret)));
			mxDestroyArray(ret);
			return tmp;
		};
		auto getCoordinate = [engine_ptr](const char* c)->decltype(auto)
		{
			auto ret = engGetVariable(engine_ptr, c);
			double *d = static_cast<double*>(mxGetData(ret));
			auto tmp = std::make_pair(*d, *(d + 1));
			mxDestroyArray(ret);
			return tmp;
		};
		auto putDouble = [engine_ptr](const char* c, const int x)->void
		{
			double **mat = new double*[1];
			mat[0] = new double[1];
			mat[0][0] = x;
			auto data = convertVectorToMxArray(mat, 1, 1);
			if (engPutVariable(engine_ptr, c, data) != 0)
				std::cout << "fail to put Double" << std::endl;
			delete[] mat[0]; delete[] mat;
		};
		auto putCoordinate = [engine_ptr](const char* c, const Coordinate& C)->void
		{
			double **mat = new double*[1];
			mat[0] = new double[2];
			mat[0][0] = C.first; mat[0][1] = C.second;
			auto data = convertVectorToMxArray(mat, 1, 2);
			if (engPutVariable(engine_ptr, c, data) != 0)
				std::cout << "fail to put Coordinate" << std::endl;
			delete[] mat[0]; delete[] mat;
		};
		auto d = getDouble("a");
		cout << "the value is: " << d << endl;
		auto d2 = getCoordinate("b");
		cout << d2.first << " " << d2.second << endl;


		//modify a
		putDouble("a", 777);
		cout << "the value is: " << getDouble("a") << endl;

		//modify b
		putCoordinate("b", std::make_pair(23, 456));
		auto d3 = getCoordinate("b");
		cout << d3.first << " " << d3.second << endl;


		cout << "Hit return to close." << endl;
		cin.get();

		evalString("close;");

		return EXIT_SUCCESS;
	}
}
