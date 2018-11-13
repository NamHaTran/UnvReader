#include "DGMessagesLib.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

//include chrono and ctime to get time data
#include <chrono>
#include <ctime>

namespace message
{
	std::string headerFile()
	{
		std::string headerStr(" ");
		headerStr = R"(+------------------------------ CFD LOVES C++ ---------------------------+
|--------------------DISCONTINUOS GALERKIN METHOD SOLVER-----------------|
|                                  Author                                |
|   Nam Ha Tran.                                                         |
|   Ver 1.00                                                             |
+------------------------------------------------------------------------+
|   This program uses Discontinous Galerkin method to solve 2D problems  |
|   on structural and unstructural mesh.                                 |
+------------------------------------------------------------------------+
)";
		std::string Timestr(getTime());
		headerStr += "	Program ran at (d-m-y_h-m-s) " + Timestr + "\n";
		return headerStr;
	}

	std::string undfKeyW(std::string keyW, std::string location)
	{
		std::string str("Cannot find key word <" + keyW + "> at " + location);
		return str;
	}

	std::string opFError(std::string fileName, std::string location)
	{
		std::string str("Cannot open file <" + fileName + "> located at " + location);
		return str;
	}

	void writeLog(std::string location, std::string caseName, std::string str)
	{
		std::string strTime(getTime());
		std::string logFile(location + "\\log_" + caseName + ".txt");
		std::ofstream logFlux(logFile.c_str(), std::ios_base::app | std::ios_base::out);

		//Get time
		std::string crash_time(message::getTime());
		std::cout << "ERROR: " << str << std::endl;
		logFlux << "log was created at " << crash_time << std::endl << str << std::endl;
		std::cout << "DGSolver will exit after you hit return.\n";
		system("pause");
		exit(EXIT_FAILURE);
	}

	std::string getTime() //This function is referenced from internet
	{
		/*
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
		auto str = oss.str();
		*/
		std::string str("time");
		return str;
	}

	std::string headerpTU(std::string file)
	{
		std::string headerStr(" ");
		if (file.compare("p") == 0)
		{
			headerStr = R"(Pressure conditions (Pa)
initialValue				0
)";
		}
		else if (file.compare("T") == 0)
		{
			headerStr = R"(Temperature conditions (K)
initialValue				0
)";
		}
		else if (file.compare("U") == 0)
		{
			headerStr = R"(Velocity conditions (m/s)
initialValue			0 0 0
)";
		}
		return headerStr;
	}
}
