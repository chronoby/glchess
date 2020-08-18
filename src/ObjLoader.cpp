
#include "ObjLoader.h"


ObjLoader::ObjLoader(string file)
{
	filename = file;
	ifstream infile;
	VertCount = 0;
	QuadCount = 0;
	TriangleCount = 0;
	vertices = NULL;
	quads = NULL;
	triangles = NULL;

	BuildMesh();
}

void ObjLoader::openFile()
{
	cout << filename;
	infile.open(filename.c_str());
	if(infile.fail())
	{
		cout << "Counting" << endl;
		cout << "File open failure![01]" << endl;
		exit(1);
	}
}

int ObjLoader::CloseFile()
{
	infile.close();
	return 1;
}

void ObjLoader::BuildMesh()
{
	QuadCount = 0;
	TriangleCount = 0;
	
	openFile();
	
	string name;
	string buffer = "";
	string tmpStore;
	infile >> tmpStore;
	infile >> name;
	infile >> VertCount;
	
	vertices = new float[VertCount*3];
	int* tempquads = new int[VertCount*5];
	int* temptriangles = new int[VertCount*5];
	int tmp[4];
	int count = 0;

	for(int i=0;i<VertCount;i++)
	{
		if(infile.eof())
		{
			cout << name << " has incorrect file format!" << endl;
			break;
		}
		infile >> tmpStore;
		infile >> vertices[i * 3 + 0] >> vertices[i * 3 + 1] >> vertices[i * 3 + 2];
	}
	while(true)
	{
		if(infile.eof())
		{
			cout << name << " processed." << endl;
			break;
		}
		while (true)
		{
			infile >> tmpStore;
			if (infile.eof())
				break;
			if (tmpStore == "f")
				break;
			else
			{
				tmp[count] = atoi(tmpStore.c_str()) - 1;
				count++;
			}
		}
		if( count == 4 )
		{	
			tempquads[QuadCount * 4 + 0] = tmp[0];
			tempquads[QuadCount * 4 + 1] = tmp[1];
			tempquads[QuadCount * 4 + 2] = tmp[2];
			tempquads[QuadCount * 4 + 3] = tmp[3];
			QuadCount++;
		}
		else if( count == 3 )
		{
			temptriangles[TriangleCount * 3 + 0] = tmp[0];
			temptriangles[TriangleCount * 3 + 1] = tmp[1];
			temptriangles[TriangleCount * 3 + 2] = tmp[2];
			TriangleCount++;
		}
		count = 0;
	}
	quads = new int[QuadCount*4];
	for(int i=0;i<QuadCount*4;i++) 
		quads[i]=tempquads[i];  
	triangles = new int[TriangleCount*3];
	for(int i=0;i<TriangleCount*3;i++) 
		triangles[i]=temptriangles[i]; 
	CloseFile();
}

float* ObjLoader::GetVertices()
{
	return vertices;
}

int* ObjLoader::GetQuads()
{
	return quads;
}

int* ObjLoader::GetTriangles()
{
	return triangles;
}
