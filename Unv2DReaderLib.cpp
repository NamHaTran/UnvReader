#include "Unv2DReaderLib.h"
#include "DGMessagesLib.h"
#include "DGAuxUltilitiesLib.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int const meshArrSize(1000000);
int const pointsArrSize(300000);
int const elements2DArrSize(800000);

extern std::string caseName, Mesh[meshArrSize], wD, pwd;

void dispLogo()
{
	std::cout << "+------------------------------ CFD LOVES C++ ---------------------------+" << std::endl
		<< "|-------------------UNV MESH FORMAT READER (2D version)------------------|" << std::endl
		<< "|                                 Author(s)                              |" << std::endl
		<< "|   Nam Ha Tran.                                                         |" << std::endl
		<< "|   Ver 1.00                                                             |" << std::endl
		<< "+------------------------------------------------------------------------+" << std::endl
		<< "|   This tool is developed as a part of project 'Discontinous Galerkin   |" << std::endl
		<< "|   Method for 2D problems'.                                             |" << std::endl
		<< "+------------------------------------------------------------------------+" << std::endl;
}

void getCase()
{
	std::cout << "***Getting case's information***\n";
	wD = auxUlti::workingdir();

	//std::cout << wD << std::endl;
	/*Get caseName from SubmitCase*/
	std::string submitLoc(" ");  //submitLoc contents location of submitingCase
	submitLoc = wD; //+"\\CASES\\SubmitCase.txt"
	//submitLoc.erase(submitLoc.end() - 22, submitLoc.end());
	submitLoc += "\\CASES\\SubmitCase.txt";

	std::ifstream submitCaseFlux(submitLoc.c_str());
	if (submitCaseFlux)
	{
		std::string line, keyWord;
		int numLine(0), keyWFlag(0);
		while (std::getline(submitCaseFlux, line))  //Read submitCaseFlux line by line
		{
			std::istringstream line2str(line);
			std::vector<std::string> ptr;
			//Split <line2str> stringstream into array of words
			while ((line2str >> keyWord))
			{
				ptr.push_back(keyWord);
			}

			int numWrd = ptr.size();

			if (numWrd == 2)
			{
				std::string str1(ptr[0]), str2("caseName");
				if (str1.compare(str2) == 0)
				{
					caseName = ptr[1];
					keyWFlag = 1;
					std::cout << "	Case " << caseName << " has been submitted\n";
				}
			}
		}
		if (keyWFlag == 0)
		{
			std::cout << message::undfKeyW("caseName", submitLoc) << std::endl;
		}
	}
	else
	{
		message::writeLog((wD + "\\CASES\\"), "", message::opFError("SubmitCase.txt", submitLoc));
	}
	pwd = wD + "\\CASES\\" + caseName;
}

void GetMesh(std::string meshName)
{
	int iline(0);
	extern std::string Mesh[meshArrSize];  //extern imports variable from <main> to function
	extern int location[3][2], noLines;

	for (int r = 0; r < meshArrSize; r++)
	{
		Mesh[r] = " ";
	}

	std::ifstream meshFlux(meshName.c_str());
	if (meshFlux)
	{
		std::string line;
		int word;
		while (std::getline(meshFlux, line))
		{
			Mesh[iline] = line;
			iline++;
			std::istringstream line2str(line);
			std::vector<int> ptr;

			//Split <line2str> stringstream into array of words
			while ((line2str >> word))
			{
				ptr.push_back(word);
			}

			int sizePtr(static_cast<int>(ptr.size()));
			if (sizePtr == 1)
			{
				int elemType = ptr[0];
				if (elemType == 2411)  //Nodes
				{
					location[0][0] = iline;
					location[0][1] = elemType;
				}
				else if (elemType == 2412)  //Elements
				{
					location[1][0] = iline;
					location[1][1] = elemType;
				}
				else if (elemType == 2467)  //Boundaries
				{
					location[2][0] = iline;
					location[2][1] = elemType;
				}
			}
			noLines += 1;
		}
	}
	else
	{
		std::string logFile(wD + "\\CASES\\" + caseName);
		message::writeLog(pwd, caseName, message::opFError(meshName, logFile));
	}
}

void GetNodes()
{
	extern std::string Mesh[meshArrSize];
	extern int location[3][2], nodeNumber;
	extern double Points[pointsArrSize][4];  //Column 1 for node name, columns 2:4 for x, y, z coordinates
	int firstNodeLine(location[0][0]), lastNodeLine(location[1][0] - 3);
	nodeNumber = ((lastNodeLine - firstNodeLine) / 2);
	std::cout << "**Reading nodes coordinates**" << std::endl
		<< "-----Mesh has " << nodeNumber << " nodes." << std::endl;
	int index(0);
	double xCoor, yCoor, zCoor;
	for (int nline = firstNodeLine + 1; nline < lastNodeLine + 1; nline += 2)
	{
		Points[index][0] = index + 1;
		std::string nodeDataStr(Mesh[nline]);
		std::istringstream nodeData(nodeDataStr);
		while ((nodeData >> xCoor >> yCoor >> zCoor))
		{
			Points[index][1] = xCoor;
			Points[index][2] = yCoor;
			Points[index][3] = zCoor;
		}
		index++;
	}
	std::cout << "DONE!" << std::endl << " " << std::endl;
}

void GetElements()
{
	std::cout << "**Reading information of elements**" << std::endl;
	extern int Elements1D[pointsArrSize][4], Elements2D[elements2DArrSize][5], location[3][2],
		n1D, n2D;
	/*-Elements1D: Column 1 contents element names, column 2:3 contents nodes of each element,
	column 4 contents group which element belong to.
	-Elements2D: Column 1 contents element names, column 2:5 contents nodes of each element.*/
	extern std::string Mesh[meshArrSize];

	int firstElemLine(location[1][0]), lastElemLine(location[2][0] - 3), elemProp1, elemProp2;
	int elemLocation[4]; //From row 1 to 4: starting and ending lines of 1D and 2D elements
	elemLocation[0] = firstElemLine + 1;
	for (int nline = firstElemLine; nline < lastElemLine; nline += 3)
	{
		std::string elemPropertiesStr(Mesh[nline]);
		std::istringstream elemProperties(elemPropertiesStr);
		elemProperties >> elemProp1 >> elemProp2;
		if (elemProp2 != 11)
		{
			elemLocation[1] = nline;
			break;
		}
	}
	elemLocation[2] = elemLocation[1] + 1;
	elemLocation[3] = lastElemLine;
	n1D = (elemLocation[1] - elemLocation[0] + 1) / 3;
	n2D = (elemLocation[3] - elemLocation[2] + 1) / 2;
	std::cout << "-----Mesh has " << n1D << " 1D elements." << std::endl
		<< "-----Mesh has " << n2D << " 2D elements." << std::endl;

	//Set default value for Elements2D array
	for (int nRow = 0; nRow < n2D; nRow++)
	{
		Elements2D[nRow][4] = -22;
	}

	//1D ELEMENTS
	int index(0);
	for (int nline = elemLocation[0] - 1; nline < elemLocation[1] - 1; nline += 3)
	{
		std::string elemPropertiesStr(Mesh[nline]);
		std::istringstream elemProperties(elemPropertiesStr);
		elemProperties >> Elements1D[index][0];

		std::string elemDataStr(Mesh[nline + 2]);
		std::istringstream elemData(elemDataStr);
		elemData >> Elements1D[index][1] >> Elements1D[index][2];
		index++;
	}

	//2D ELEMENTS
	index = 0;
	for (int nline = elemLocation[2] - 1; nline < elemLocation[3] - 1; nline += 2)
	{
		int typeElem(0);
		std::string elemPropertiesStr(Mesh[nline]);
		std::istringstream elemProperties(elemPropertiesStr);
		elemProperties >> Elements2D[index][0] >> typeElem;

		std::string elemDataStr(Mesh[nline + 1]);
		std::istringstream elemData(elemDataStr);
		if (typeElem == 44)  //quad element
		{
			elemData >> Elements2D[index][1] >> Elements2D[index][2] >> Elements2D[index][3] >> Elements2D[index][4];
		}
		else if (typeElem == 41)  //tri element
		{
			elemData >> Elements2D[index][1] >> Elements2D[index][2] >> Elements2D[index][3];
		}
		index++;
	}
	std::cout << "DONE!" << std::endl << " " << std::endl;
}

void GetBoundaries()
{
	std::cout << "**Reading information of boundaries**" << std::endl;
	extern std::string Mesh[meshArrSize];
	extern int location[3][2], nodeNumber, noLines, boundaries[pointsArrSize][2], n1D;
	int firstBoundLine(location[2][0] + 1), lastBoundLine(noLines - 2);
	extern std::vector<int> boundLocation;
	extern std::vector<std::string> boundName;
	extern bool savingFlag;
	extern int numOfBound, numOfBoundEdge;
	for (int nline = firstBoundLine; nline < lastBoundLine; nline++)
	{
		std::string linePropertiesStr(Mesh[nline]);
		std::istringstream lineProperties(linePropertiesStr);
		int checkNumber(0);
		std::string BCname;
		if (!(lineProperties >> checkNumber))  //Check input is string or number
		{
			/*NOTE: when line lineProperties>>checkNumber is excuted, a pointer of stream lineProperties
			is changed (it's not located at the first data of stream), if we use stream lineProperties again,
			(ex: lineProperties>>BCname), lineProperties will transfer data at second location to BCname*/

			std::string linePropertiesStr2(Mesh[nline]);
			std::istringstream lineProperties2(linePropertiesStr2);
			lineProperties2 >> BCname;
			boundName.push_back(BCname);
			boundLocation.push_back(nline);
			numOfBound += 1;
		}
	}
	numOfBound = static_cast<int>(boundLocation.size());
	if (numOfBound != 0)
	{
		savingFlag = true;
		std::cout << "-----Mesh has " << numOfBound << " boundaries:" << std::endl;
		for (int i = 0; i < numOfBound; i++)
		{
			std::cout << "     - " << boundName[i] << std::endl;
		}

		//Set all values of boundaries to -22
		for (int nRow = 0; nRow < n1D; nRow++)
		{
			for (int nColumn = 0; nColumn < 2; nColumn++)
			{
				boundaries[nRow][nColumn] = -22;
			}
		}

		int index(0), group(0);
		for (int nline = firstBoundLine; nline < lastBoundLine + 1; nline++)
		{
			int num;
			std::vector<int> linePropertiesNum;
			std::string linePropertiesStr(Mesh[nline]);
			std::istringstream lineProperties(linePropertiesStr);
			while (lineProperties >> num)
			{
				linePropertiesNum.push_back(num);
			}

			if (nline<lastBoundLine && static_cast<int>(linePropertiesNum.size()) == 0)
			{
				std::string linePropertiesStrNext(Mesh[nline + 1]);
				std::istringstream linePropertiesNext(linePropertiesStrNext);
				std::vector<int> linePropertiesNextNum;
				while (linePropertiesNext >> num)
				{
					linePropertiesNextNum.push_back(num);
				}

				if (static_cast<int>(linePropertiesNextNum.size()) != 0 && linePropertiesNextNum[0] == 8)
				{
					group += 1;
				}
			}

			if (static_cast<int>(linePropertiesNum.size()) == 8 && linePropertiesNum[0] == 8 && linePropertiesNum[4] == 8)
			{
				//Line contents boundary edges
				boundaries[index][0] = linePropertiesNum[1];
				boundaries[index + 1][0] = linePropertiesNum[5];
				boundaries[index][1] = group;
				boundaries[index + 1][1] = group;
				index += 2;
				numOfBoundEdge += 2;
			}
			else if (static_cast<int>(linePropertiesNum.size()) == 4 && linePropertiesNum[0] == 8)
			{
				boundaries[index][0] = linePropertiesNum[1];
				boundaries[index][1] = group;
				index += 1;
				numOfBoundEdge += 1;
			}
		}
		std::cout << "DONE!" << std::endl << " " << std::endl;
	}
	else
	{
		savingFlag = false;
		std::string errorStr(" ");
		errorStr = R"(-----WARNING: Mesh has no boundary!!!
     Boundaries must be created before simulation.
     Reading mesh has finished with no boundary information.
)";
		message::writeLog(pwd, caseName, errorStr);
	}
}

void MarkBoundary()
{
	extern int boundaries[pointsArrSize][2], Elements1D[pointsArrSize][4], n1D, numOfBoundEdge;
	for (int i = 0; i < numOfBoundEdge; i++)
	{
		for (int j = 0; j < n1D; j++)
		{
			if (Elements1D[j][3] == 0)
			{
				if (Elements1D[j][0] == boundaries[i][0])
				{
					Elements1D[j][3] = boundaries[i][1];
				}
			}
		}
	}
}

void SaveMeshData()
{
	std::cout << "**Saving data**\n";
	extern bool savingFlag;

	extern int numOfBound, Elements1D[pointsArrSize][4], Elements2D[elements2DArrSize][5], nodeNumber, n1D, n2D;
	extern double Points[pointsArrSize][4];
	extern std::vector<std::string> boundName;

	//Declare saving locations
	std::string bcPatchPath(wD + "\\CASES\\" + caseName + "\\Constant\\" + "boundaryPatch.txt");
	std::string PointsPatchPath(wD + "\\CASES\\" + caseName + "\\Constant\\Mesh\\" + "Points.txt");
	std::string Elem1DPatchPath(wD + "\\CASES\\" + caseName + "\\Constant\\Mesh\\" + "Elements1D.txt");
	std::string Elem2DPatchPath(wD + "\\CASES\\" + caseName + "\\Constant\\Mesh\\" + "Elements2D.txt");

	std::ofstream FluxBC(bcPatchPath.c_str());
	if (FluxBC)
	{
		std::string header(message::headerFile());
		FluxBC << header
			<< "  " << std::endl
			<< "Boundary condition definitions:\n";

		for (int i = 0; i < numOfBound; i++)
		{
			FluxBC << boundName[i] << std::endl
				<< "{\n"
				<< "	Group				" << i + 1 << std::endl
				<< "	Type				wall\n"
				<< "	Method				weakRiemann\n"
				<< "}\n" << " \n";
		}

	}
	else
	{
		message::writeLog(pwd, caseName, message::opFError("boundaryPatch.txt", bcPatchPath));
	}

	//Save points data
	std::ofstream FluxPt(PointsPatchPath.c_str());
	if (FluxPt)
	{
		for (int i = 0; i < nodeNumber; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				FluxPt << Points[i][j] << " ";
			}
			FluxPt << "\n";
		}

	}
	else
	{
		message::writeLog(pwd, caseName, message::opFError("Points.txt", PointsPatchPath));
	}

	//Save elements 1D data
	std::ofstream FluxElem1D(Elem1DPatchPath.c_str());
	if (FluxElem1D)
	{
		for (int i = 0; i < n1D; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				FluxElem1D << Elements1D[i][j] << " ";
			}
			FluxElem1D << "\n";
		}

	}
	else
	{
		message::writeLog(pwd, caseName, message::opFError("Elements1D.txt", Elem1DPatchPath));
	}

	//Save element 2D data
	std::ofstream FluxElem2D(Elem2DPatchPath.c_str());
	if (FluxElem2D)
	{
		for (int i = 0; i < n2D; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				FluxElem2D << Elements2D[i][j] << " ";
			}
			FluxElem2D << "\n";
		}

	}
	else
	{
		message::writeLog(pwd, caseName, message::opFError("Elements2D.txt", Elem2DPatchPath));
	}

	std::cout << "DONE!" << std::endl << " " << std::endl;
}

void createFolder0()
{
	extern std::vector<std::string> boundName;

	int numOfBC(static_cast<int>(boundName.size()));
	std::string TLoc(wD + "\\CASES\\" + caseName + "\\0\\T.txt");
	std::string ULoc(wD + "\\CASES\\" + caseName + "\\0\\U.txt");
	std::string PLoc(wD + "\\CASES\\" + caseName + "\\0\\p.txt");

	/*create T.txt*/
	std::ofstream TFlux(TLoc.c_str());
	if (TFlux)
	{
		std::string header(message::headerFile());
		TFlux << header << message::headerpTU("T");

		for (int i = 0; i < numOfBC; i++)
		{
			TFlux << boundName[i] << std::endl
				<< "{\n"
				<< "	Group				" << i + 1 << std::endl
				<< "	Type				WallAdiabatic\n" << "}\n" << " \n";
		}

	}
	else
	{
		std::string logFile(wD + "\\CASES\\" + caseName);
		message::writeLog(pwd, caseName, message::opFError("T.txt", logFile));
	}

	/*Create P.txt*/
	std::ofstream PFlux(PLoc.c_str());
	if (PFlux)
	{
		std::string header(message::headerFile());
		PFlux << header << message::headerpTU("p");

		for (int i = 0; i < numOfBC; i++)
		{
			PFlux << boundName[i] << std::endl
				<< "{\n"
				<< "	Group " << i + 1 << std::endl
				<< "	Type				zeroGradient\n" << "}\n" << " \n";
		}

	}
	else
	{
		std::string logFile(wD + "\\CASES\\" + caseName);
		message::writeLog(pwd, caseName, message::opFError("p.txt", logFile));
	}

	/*Create U.txt*/
	std::ofstream UFlux(ULoc.c_str());
	if (UFlux)
	{
		std::string header(message::headerFile());
		UFlux << header << message::headerpTU("U");

		for (int i = 0; i < numOfBC; i++)
		{
			UFlux << boundName[i] << std::endl
				<< "{\n"
				<< "	Group " << i + 1 << std::endl
				<< "	Type				noSlip\n" << "}\n" << " \n";
		}

	}
	else
	{
		std::string logFile(wD + "\\CASES\\" + caseName);
		message::writeLog(pwd, caseName, message::opFError("U.txt", logFile));
	}
}

void createTemplate()
{
	std::cout << "**Creating template files**\n";

	std::string header(message::headerFile());
	std::string DGOpLoc(wD + "\\CASES\\" + caseName + "\\System\\DGOptions.txt");
	std::string MatLoc(wD + "\\CASES\\" + caseName + "\\Constant\\Material.txt");

	/*Create file DGOptions*/
	std::ofstream DGOpFlux(DGOpLoc.c_str());
	if (DGOpFlux)
	{
		DGOpFlux << header
			<< "	Note: all parameter are in SI unit.\n"
			<< "DGoptions\n"
			<< "{\n"
			<< "	numberOfGaussPoints			2\n"
			<< "	orderOfAccuracy				2\n"
			<< "	CourantNumber				0.5\n"
			<< "	totalTime(s)				10\n"
			<< "	writeInterval				100\n"
			<< "	writeLog					true\n"
			<< "	ddtScheme					Euler\n"
			<< "	limiter						PositivityPreserving\n"
			<< "}";
	}
	else
	{
		std::string logFile(wD + "\\CASES\\" + caseName);
		message::writeLog(pwd, caseName, message::opFError("DGOptions.txt", logFile));
	}

	/*Create file Material*/
	std::ofstream MatFlux(MatLoc.c_str());
	if (MatFlux)
	{
		MatFlux << header
			<< "	Note: all parameter are in SI unit.\n"
			<< "Material Properties\n"
			<< "{\n"
			<< "	gammaRatio					1.4\n"
			<< "	gasConstant					287\n"
			<< "	PrandtlNumber				0.72\n"
			<< "	SutherlandAs				1.46e-6\n"
			<< "	SutherlandTs				110.4\n"
			<< "}";
	}
	else
	{
		std::string logFile(wD + "\\CASES\\" + caseName);
		message::writeLog(pwd, caseName, message::opFError("Material.txt", logFile));
	}

	std::cout << "DONE!\n";
}

void clearVar()
{
	extern std::vector<std::string> boundName;
	extern std::vector<int> boundLocation;
	extern int location[3][2], noLines, Elements1D[pointsArrSize][4], Elements2D[elements2DArrSize][5], numOfBound, boundaries[pointsArrSize][2];
	extern double Points[pointsArrSize][4];
	extern bool savingFlag;
	extern int nodeNumber, n1D, n2D, numOfBoundEdge;

	int numOfBC(static_cast<int>(boundName.size()));
	int numOfBCLoc(static_cast<int>(boundLocation.size()));

	/*Use erase function to clear vector*/
	boundName.erase(boundName.begin(), boundName.begin() + numOfBC);
	boundLocation.erase(boundLocation.begin(), boundLocation.begin() + numOfBCLoc);
	/*Shrink to fit
	Requests the container to reduce its capacity to fit its size.*/
	boundName.shrink_to_fit();
	boundLocation.shrink_to_fit();

	/*Reset all variable*/
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			location[i][j] = 0;
		}
	}

	for (size_t i = 0; i < pointsArrSize; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Elements1D[i][j] = 0;
		}
	}

	for (size_t i = 0; i < elements2DArrSize; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Elements2D[i][j] = 0;
		}
	}

	for (size_t i = 0; i < pointsArrSize; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			boundaries[i][j] = 0;
		}
	}

	numOfBound = 0;
	noLines = 0;
	savingFlag = false;  //Flag of saving data
	nodeNumber = 0;
	n1D = 0;
	n2D = 0;
	numOfBoundEdge = 1;
}