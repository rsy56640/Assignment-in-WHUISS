#include "Elliptical_Curve.h"

namespace Elliptic_Curve
{


	std::vector<int> translate(const std::string& msg)
	{
		std::vector<int> result;
		for (auto c : msg)
			result.push_back(static_cast<int>(c));
		return result;
	}

	mxArray *convertVectorToMxArray(double** mat,
		const int nb_rows, const int nb_cols)
	{
		mxArray *outputMxArray = mxCreateDoubleMatrix(
			(int)nb_rows,
			(int)nb_cols,
			mxREAL);

		double *data = (double*)mxGetData(outputMxArray);
		for (int r = 0; r < nb_rows; r++)
			for (int c = 0; c < nb_cols; c++)
				data[r + c*nb_rows] = (double)mat[r][c];

		return outputMxArray;
	};

	void initial_environment()
	{
		Engine *engine_ptr;
		// open engine  
		if (!(engine_ptr = engOpen("\0")))
			fprintf(stderr, "\nCan't start MATLAB engine\n");
		using Deleter = std::function<void(Engine*)>;
		std::shared_ptr<Engine> ep{ engine_ptr, engClose };
		auto evalString = std::bind(engEvalString, ep.get(), std::placeholders::_1);
		evalString("cd bin");
		std::cout << "environment has been set appropriately." << std::endl;
	}

	std::pair<Coordinate, Coordinate> generate(const E& e, const int priv)
	{

		Engine *engine_ptr;
		// open engine  
		if (!(engine_ptr = engOpen("\0")))
			fprintf(stderr, "\nCan't start MATLAB engine\n");

		auto getDouble = [engine_ptr](const char* c)->decltype(auto)
		{
			//std::shared_ptr<mxArray> ret(engGetVariable(engine_ptr, c), mxDestroyArray);
			//return std::shared_ptr<double>(static_cast<double*>(mxGetData(ret.get())));

			auto ret = engGetVariable(engine_ptr, c);
			double *d = static_cast<double*>(mxGetData(ret));
			auto tmp = d[0];
			mxDestroyArray(ret);
			return tmp;
		};
		auto getCoordinate = [engine_ptr](const char* c)->decltype(auto)
		{
			auto ret = engGetVariable(engine_ptr, c);
			double *d = static_cast<double*>(mxGetData(ret));
			auto tmp = std::make_pair(d[0], d[1]);
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

		//execute matlab operations  
		using Deleter = std::function<void(Engine*)>;
		std::shared_ptr<Engine> ep{ engine_ptr, engClose };
		auto evalString = std::bind(engEvalString, ep.get(), std::placeholders::_1);

		const int a = e.a;
		const int b = e.b;
		const int p = e.prime;

		std::vector<std::string> str =
		{
			"a = 0",
			"b = 0",
			"p = 0",
			"priv = 0"
		};
		for (auto const&s : str)
			evalString(s.c_str());
		putDouble("a", a);
		putDouble("b", b);
		putDouble("p", p);
		putDouble("priv", priv);

		evalString("[x, y] = ECCgenerate(a, b, p)");
		std::cout << getDouble("x") << " " << getDouble("y") << " ";
		evalString("G = [x, y]");
		auto G = getCoordinate("G");

		evalString("[x, y] = NP(a, b, p, priv, G(1), G(2))");
		std::cout << getDouble("x") << " " << getDouble("y") << std::endl;
		evalString("K = [x, y]");
		auto K = getCoordinate("K");

		return std::make_pair(K, G);
	}

}