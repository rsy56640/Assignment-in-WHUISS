#pragma once
#ifndef _ELLIPTICAL_CURVE_H
#define _ELLIPTICAL_CURVE_H
#include "engine.h"
#pragma comment(lib,"libeng.lib")
#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmat.lib")
#include <ctime>
#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <functional>
#include <memory>

namespace Elliptic_Curve
{
	class Process;
	static int global_id = 0;
	static std::unordered_map<int, const Process*> table;

	void process_register(const Process* p);

	typedef struct elliptical_curve
	{
		int prime;
		int a, b;
		elliptical_curve() = default;
		elliptical_curve(int _a, int _b, int p)
			:prime(p), a(_a), b(_b) {}
	};

	using E = elliptical_curve;
	using Coordinate = std::pair<double, double>;

	typedef struct public_key
	{
		E curve;
		Coordinate K;
		Coordinate G;
		public_key() = default;
		public_key(const E& e, Coordinate k, Coordinate g)
			:curve(e), K(k), G(g) {}
	};

	/*
	 * translate string into int array
	**/
	std::vector<int> translate(const std::string& msg);


	mxArray *convertVectorToMxArray(double** mat,
		const int nb_rows, const int nb_cols);

	void initial_environment();

	/*
	 * generate K = kG
	 * return [K, G]
	**/
	std::pair<Coordinate, Coordinate> generate(const E& e, const int priv);


	class Process
	{

		using P_ptr = std::shared_ptr<Process>;

	private:

		Process(const std::string& name, const E& E, Coordinate K, Coordinate G, int priv)
			:id(global_id++), _name(name), _message_list(), _pk(), public_k(E, K, G), private_k(priv) {}
	public:

		static P_ptr Factory(const std::string& name, const E& E, Coordinate K, Coordinate G, int priv)
		{
			P_ptr p{ new Process(name, E, K, G, priv) };
			process_register(p.get());
			return p;
		}

		int getID() const noexcept
		{
			return id;
		}

		std::string getName() const
		{
			return _name;
		}

		void sendPK(P_ptr to, const public_key& pk) const
		{
			to->getPK(this, pk);
		}

		void getPK(const Process* from, const public_key& pk)
		{
			_pk[from->id] = pk;
		}

		void sendMessage(P_ptr to, const std::string& msg) const
		{
			auto result = encode(to.get(), msg);
			std::vector<double> message;
			for (auto it : result)
			{
				message.push_back(it.first.first);
				message.push_back(it.first.second);
				message.push_back(it.second.first);
				message.push_back(it.second.second);
			}
			to->getMessage(this, message);
		}

		void getMessage(const Process* from, const std::vector<double>& msg)
		{
			_message_list.push(std::make_pair(from->id, msg));
		}

		void readAllMessage()
		{
			while (!_message_list.empty())
			{
				auto item = _message_list.front(); _message_list.pop();
				const int size = item.second.size();
				auto const& Vec = item.second;
				std::vector<std::pair<Coordinate, Coordinate>> message;
				double x1, y1, x2, y2;
				for (int i = 0; i < size;)
				{
					x1 = Vec[i++];
					y1 = Vec[i++];
					x2 = Vec[i++];
					y2 = Vec[i++];
					message.push_back(std::make_pair(std::make_pair(x1, y1), std::make_pair(x2, y2)));
				}

				decode(table[item.first], message);

			}
		}

	private:

		const int id;
		const std::string _name;
		std::queue<std::pair<int, std::vector<double>>> _message_list;
		std::unordered_map<int, public_key> _pk;
		const public_key public_k;
		const int private_k;

		/*
		 * Encode
		**/
		std::vector<std::pair<Coordinate, Coordinate>> encode(const Process* to, const std::string& msg) const
		{

			public_key pk;
			try {
				pk = _pk.at(to->id);
			}
			catch (std::out_of_range&)
			{
				std::cout << "No public key of the Server: " << to->_name << std::endl;
				return std::vector<std::pair<Coordinate, Coordinate>>{};
			}

			const int a = pk.curve.a;
			const int b = pk.curve.b;
			const int p = pk.curve.prime;
			const Coordinate K = pk.K;
			const Coordinate G = pk.G;

			Engine *engine_ptr;
			// open engine  
			if (!(engine_ptr = engOpen("\0")))
				fprintf(stderr, "\nCan't start MATLAB engine\n");

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

			//execute matlab operations  
			using Deleter = std::function<void(Engine*)>;
			std::shared_ptr<Engine> ep{ engine_ptr, engClose };
			auto evalString = std::bind(engEvalString, ep.get(), std::placeholders::_1);

			std::vector<std::string> str =
			{
				"a = 0",
				"b = 0",
				"p = 0",
				"K = [0,0]",
				"G = [0,0]"
			};
			for (auto const&s : str)
				evalString(s.c_str());
			putDouble("a", a);
			putDouble("b", b);
			putDouble("p", p);
			putCoordinate("K", K);
			putCoordinate("G", G);


			std::vector<int> message = translate(msg);
			std::vector<std::pair<Coordinate, Coordinate>> result;
			srand(time(NULL));

			for (auto x : message)
			{

				//generate the point M(x, y) on the Curve
				evalString("x = 0");
				putDouble("x", x);
				evalString("y = ECCCal(a, b, p, x)");
				double y = getDouble("y");
				evalString("M = [0,0]");
				putCoordinate("M", std::make_pair(x, y));

				//compute C1 and C2
				int r = rand() % 30 + 1;   //a random number used to encode the message
				evalString("r = 0");
				putDouble("r", r);
				evalString("temp = NP(a, b, p, r, K(1), K(2));");
				evalString("[x1, y1] = Add(a, b, p, M(1), M(2), temp(1), temp(2));");
				evalString("[x2, y2] = NP(a, b, p, r, G(1), G(2));");
				evalString("C1 = [x1, y1];");
				evalString("C2 = [x2, y2];");
				auto C1 = getCoordinate("C1");
				auto C2 = getCoordinate("C2");
				result.push_back(std::make_pair(C1, C2));

			}

			return result;

		}

		/*
		 * Decode
		**/
		void decode(const Process* from, const std::vector<std::pair<Coordinate, Coordinate>>& message) const
		{

			/*
			 * Decode the message through pk with Matlab
			**/
			Engine *engine_ptr;
			// open engine  
			if (!(engine_ptr = engOpen("\0")))
				fprintf(stderr, "\nCan't start MATLAB engine\n");

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

			//execute matlab operations  
			using Deleter = std::function<void(Engine*)>;
			std::shared_ptr<Engine> ep{ engine_ptr, engClose };
			auto evalString = std::bind(engEvalString, ep.get(), std::placeholders::_1);

			const int a = public_k.curve.a;
			const int b = public_k.curve.b;
			const int p = public_k.curve.prime;
			const int k = private_k;
			const Coordinate K = public_k.K;
			const Coordinate G = public_k.G;

			std::vector<std::string> str =
			{
				"a = 0",
				"b = 0",
				"p = 0",
				"k = 0",
				"K = [0,0]",
				"G = [0,0]"
			};
			for (auto const&s : str)
				evalString(s.c_str());
			putDouble("a", a);
			putDouble("b", b);
			putDouble("p", p);
			putDouble("k", private_k);
			putCoordinate("K", K);
			putCoordinate("G", G);

			std::string msg;
			for (auto& it : message)
			{
				auto C1 = it.first;
				auto C2 = it.second;
				evalString("C1 = [0,0]");
				evalString("C2 = [0,0]");
				putCoordinate("C1", C1);
				putCoordinate("C2", C2);
				evalString("[x, y] = NP(a, b, p, k, C2(1), C2(2))");
				evalString("[x1, y1] = Add(a, b, p, C1(1), C1(2), x, -y)");
				evalString("M = [x1, y1]");
				auto M = getCoordinate("M");
				msg.push_back(static_cast<char>(M.first));
			}

			std::string name = (from == nullptr) ? "Anonymous" : from->_name;
			std::cout << this->_name << " has read the original Message from the Server \""
				<< name << "\"" << " is: " << msg << std::endl;
			std::cout << std::endl;
		}

	};


	inline void process_register(const Process* p)
	{
		table[p->getID()] = p;
		std::cout << p->getName() << " has been registered." << std::endl;
	}

}//end namespace Elliptic_Curve

#endif // !_ELLIPTICAL_CURVE_H
