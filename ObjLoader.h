#pragma once
#include "defs.h"

class ObjLoader
{
public:
	ObjLoader(string filename);
	void BuildMesh();
	int CloseFile();
	int VertCount;
	int QuadCount;
	int TriangleCount;
	float* GetVertices();
	int* GetQuads();
	int* GetTriangles();
private:
	void openFile();
	ifstream infile;
	float* vertices;
	float* normals;
	int* quads;
	int* triangles;
	string filename;
};

