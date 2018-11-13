#ifndef UNV2DREADERLIB_H_INCLUDED
#define UNV2DREADERLIB_H_INCLUDED
#include <string>

void dispLogo();

//Function gets case name from submit file
void getCase();

//Function gets mesh informations and save them in <Mesh> array, it's also get locations of points, elements and boundaries
void GetMesh(std::string meshName);

//Function gets information of nodes (node's name and coordinates)
void GetNodes();

//Function gets information of elements (1D and 2D elements)
void GetElements();

//Function gets information of boundaries (how many boundaries, name of each boundary .etc..)
void GetBoundaries();

//Function marks edge to respective boundary group
void MarkBoundary();

//Function saves mesh data
void SaveMeshData();

//Function create folder 0
void createFolder0();

//Function create template of DGOptions and Material
void createTemplate();

//Function clear all variables
void clearVar();

#endif // UNV2DREADERLIB_H_INCLUDED
