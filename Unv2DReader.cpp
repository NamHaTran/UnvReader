#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include "Unv2DReaderLib.h"
using namespace std;

/*VARIABLE DECLARATION-------------------------------------------------------------------*/
int const meshArrSize(1500000);
int const pointsArrSize(500000);
int const elements2DArrSize(1000000);

string caseName(" "), Mesh[meshArrSize], wD(" "), pwd(" "), meshFileName(" ");
int location[3][2], noLines(0), Elements1D[pointsArrSize][4], Elements2D[elements2DArrSize][5], numOfBound(0), boundaries[pointsArrSize][2];
double Points[pointsArrSize][4];  //variable for GetPoints
std::vector<int> boundLocation;  //variable for GetBoundaries
std::vector<std::string> boundName;  //variable for GetBoundaries
bool savingFlag(false);  //Flag of saving data
/*Declare size of Points, Elements1D, Elements2D*/
int nodeNumber(0), n1D(0), n2D(0), numOfBoundEdge(1);

int main()
{
	//DISPLAY LOGO--------------------------------------------------------------------------
	dispLogo();
	
	//INPUT FILE NAME-----------------------------------------------------------------------
	getCase();

	cout << "Enter file name: ";
	getline(cin, meshFileName);

	string fullName = pwd + "\\" + meshFileName;
	cout << " " << endl
		<< "**Unv2DReader is running**" << endl;
	
	//GET MESH INFORMATION------------------------------------------------------------------
	GetMesh(fullName);
	
	//GET NODES INFORMATION-----------------------------------------------------------------
	GetNodes();

	//GET ELEMENTS INFORMATION--------------------------------------------------------------
	GetElements();

	//GET BOUNDARIES INFORMATION------------------------------------------------------------
	GetBoundaries();

	//MARK BOUNDARY EDGE--------------------------------------------------------------------
	MarkBoundary();

	//SAVE DATA-----------------------------------------------------------------------------
	SaveMeshData();

	//CREATE FOLDER 0-----------------------------------------------------------------------
	createFolder0();

	//CREATE TEMPLATE FILES-----------------------------------------------------------------
	createTemplate();

	system("pause");
	return 0;
}
