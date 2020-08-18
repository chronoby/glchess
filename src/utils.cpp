#include "TextureLoader.h"
#include "defs.h"
#include "ObjStack.h"
#include "utils.h"

#pragma warning (disable: 4996)

GLfloat textures[][2] = {
	{0.0,0.0},
	{1.0,0.0},
	{1.0,1.0},
	{0.0,1.0}
};

void boardBase()
{
	glPushMatrix();
	for (int j = 0; j < 4; j++)
	{
		glTranslatef(-4.5, 0, -4.5);
		glRotatef(90, 0, 1, 0);
		glBegin(GL_TRIANGLES);
		glNormal3f(-.8, 0, 0);
		glVertex3f(-.5, .1, 4);
		glVertex3f(-.5, .1, -4); //
		glVertex3f(-.5, -.9, 4);
		glNormal3f(-.8, 0, 0);
		glVertex3f(-.5, .1, -4);
		glVertex3f(-.5, -.9, -4); //
		glVertex3f(-.5, -.9, 4);
		glNormal3f(0, .8, -.8);
		glVertex3f(-.5, .1, -4);
		glVertex3f(.5, -.9, -5); //
		glVertex3f(-.5, -.9, -4);
		glNormal3f(.8, .8, 0);
		glVertex3f(.5, -.9, -5);
		glVertex3f(-.5, .1, -4); //
		glVertex3f(-.5, .1, 4);
		glNormal3f(.8, .8, 0);
		glVertex3f(.5, -.9, 5);
		glVertex3f(.5, -.9, -5); //
		glVertex3f(-.5, .1, 4);
		glNormal3f(-.8, 0, .8);
		glVertex3f(.5, -.9, 5);
		glVertex3f(-.5, .1, 4); //
		glVertex3f(-.5, -.9, 4);
		glNormal3f(0, -.8, 0);
		glVertex3f(-.5, -.9, 4);
		glVertex3f(.5, -.9, -5); //
		glVertex3f(.5, -.9, 5);
		glNormal3f(0, -.8, 0);
		glVertex3f(-.5, -.9, -4);
		glVertex3f(.5, -.9, -5); //
		glVertex3f(-.5, -.9, 4);
		glEnd();
	}
	glPopMatrix();
}

void fieldSquare(int index)
{
	glBegin(GL_QUADS);
	glVertex3f(.5, -.5, .5);
	glVertex3f(.5, .5, .5);
	glVertex3f(-.5, .5, .5);
	glVertex3f(-.5, -.5, .5); // front
	glVertex3f(.5, -.5, .5);
	glVertex3f(.5, -.5, -.5);
	glVertex3f(.5, .5, -.5);
	glVertex3f(.5, .5, .5); // right
	glNormal3f(0, -.16, 0);
	glTexCoord2fv(textures[index % 4]);
	glVertex3f(.5, -.5, .5);
	glTexCoord2fv(textures[(index + 1) % 4]);
	glVertex3f(-.5, -.5, .5);
	glTexCoord2fv(textures[(index + 2) % 4]);
	glVertex3f(-.5, -.5, -.5);
	glTexCoord2fv(textures[(index + 3) % 4]);
	glVertex3f(.5, -.5, -.5); // bottom
	glNormal3f(0, .16, 0);
	glTexCoord2fv(textures[index % 4]);
	glVertex3f(-.5, .5, -.5);
	glTexCoord2fv(textures[(index + 1) % 4]);
	glVertex3f(-.5, .5, .5);
	glTexCoord2fv(textures[(index + 2) % 4]);
	glVertex3f(.5, .5, .5);
	glTexCoord2fv(textures[(index + 3) % 4]);
	glVertex3f(.5, .5, -.5); // up
	glNormal3f(0, 0, 0);
	glVertex3f(-.5, .5, -.5);
	glVertex3f(.5, .5, -.5);
	glVertex3f(.5, -.5, -.5);
	glVertex3f(-.5, -.5, -.5); //
	glVertex3f(-.5, .5, -.5);
	glVertex3f(-.5, -.5, -.5);
	glVertex3f(-.5, -.5, .5);
	glVertex3f(-.5, .5, .5); //
	glEnd();
}

void drawPiece(ObjLoader &shape)
{
	float edgeOne[3];
	float edgeTwo[3];
	float firstVert[3];
	float secondVert[3];
	float thirdVert[3];
	float fourthVert[3];
	float normal[3];
	float* vertices = shape.GetVertices();
	int* quads = shape.GetQuads();
	int* triangles = shape.GetTriangles();

	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (int i = shape.TriangleCount; i > 0; i--)
	{
		firstVert[0] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 1))) * 3)));
		firstVert[1] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 1))) * 3) + 1));
		firstVert[2] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 1))) * 3) + 2));
		secondVert[0] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 2))) * 3)));
		secondVert[1] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 2))) * 3) + 1));
		secondVert[2] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 2))) * 3) + 2));
		thirdVert[0] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 3))) * 3)));
		thirdVert[1] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 3))) * 3) + 1));
		thirdVert[2] = *((float *)((vertices + (*((int *)(triangles + i * 3 - 3))) * 3) + 2));
		edgeTwo[0] = firstVert[0] - secondVert[0];
		edgeTwo[1] = firstVert[1] - secondVert[1];
		edgeTwo[2] = firstVert[2] - secondVert[2];
		edgeOne[0] = thirdVert[0] - secondVert[0];
		edgeOne[1] = thirdVert[1] - secondVert[1];
		edgeOne[2] = thirdVert[2] - secondVert[2];
		normal[0] = .3*(edgeOne[1] * edgeTwo[2] - edgeOne[2] * edgeTwo[1]);
		normal[1] = .3*(edgeOne[2] * edgeTwo[0] - edgeOne[0] * edgeTwo[2]);
		normal[2] = .3*(edgeOne[0] * edgeTwo[1] - edgeOne[1] * edgeTwo[0]);
		normal[0] = .3*(normal[0] / sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
		normal[1] = .3*(normal[1] / sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
		normal[2] = .3*(normal[2] / sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
		glNormal3fv(normal);
		glVertex3fv(firstVert);
		glVertex3fv(secondVert);
		glVertex3fv(thirdVert);
		normal[0] = -normal[0]; normal[1] = -normal[1]; normal[2] = -normal[2];
		glNormal3fv(normal);
		glVertex3fv(thirdVert);
		glVertex3fv(secondVert);
		glVertex3fv(firstVert);
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glBegin(GL_QUADS);
	for (int i = shape.QuadCount; i > 0; i--)
	{
		firstVert[0] = *((float *)((vertices + (*((int *)(quads + i * 4 - 1))) * 3)));
		firstVert[1] = *((float *)((vertices + (*((int *)(quads + i * 4 - 1))) * 3) + 1));
		firstVert[2] = *((float *)((vertices + (*((int *)(quads + i * 4 - 1))) * 3) + 2));
		secondVert[0] = *((float *)((vertices + (*((int *)(quads + i * 4 - 2))) * 3)));
		secondVert[1] = *((float *)((vertices + (*((int *)(quads + i * 4 - 2))) * 3) + 1));
		secondVert[2] = *((float *)((vertices + (*((int *)(quads + i * 4 - 2))) * 3) + 2));
		thirdVert[0] = *((float *)((vertices + (*((int *)(quads + i * 4 - 3))) * 3)));
		thirdVert[1] = *((float *)((vertices + (*((int *)(quads + i * 4 - 3))) * 3) + 1));
		thirdVert[2] = *((float *)((vertices + (*((int *)(quads + i * 4 - 3))) * 3) + 2));
		fourthVert[0] = *((float *)((vertices + (*((int *)(quads + i * 4 - 4))) * 3)));
		fourthVert[1] = *((float *)((vertices + (*((int *)(quads + i * 4 - 4))) * 3) + 1));
		fourthVert[2] = *((float *)((vertices + (*((int *)(quads + i * 4 - 4))) * 3) + 2));
		edgeTwo[0] = firstVert[0] - secondVert[0];
		edgeTwo[1] = firstVert[1] - secondVert[1];
		edgeTwo[2] = firstVert[2] - secondVert[2];
		edgeOne[0] = thirdVert[0] - secondVert[0];
		edgeOne[1] = thirdVert[1] - secondVert[1];
		edgeOne[2] = thirdVert[2] - secondVert[2];
		normal[0] = .3*(edgeOne[1] * edgeTwo[2] - edgeOne[2] * edgeTwo[1]);
		normal[1] = .3*(edgeOne[2] * edgeTwo[0] - edgeOne[0] * edgeTwo[2]);
		normal[2] = .3*(edgeOne[0] * edgeTwo[1] - edgeOne[1] * edgeTwo[0]);
		normal[0] = .3*(normal[0] / sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
		normal[1] = .3*(normal[1] / sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
		normal[2] = .3*(normal[2] / sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]));
		glNormal3fv(normal);
		glVertex3fv(firstVert);
		glVertex3fv(secondVert);
		glVertex3fv(thirdVert);
		glVertex3fv(fourthVert);
		normal[0] = -normal[0]; normal[1] = -normal[1]; normal[2] = -normal[2];
		glNormal3fv(normal);
		glVertex3fv(fourthVert);
		glVertex3fv(thirdVert);
		glVertex3fv(secondVert);
		glVertex3fv(firstVert);
	}   /// ADD A NORMALIZATION FUNCTION IN THE VERTSORT CLASS
	glEnd();
	glPopMatrix();
}

void addBlack(ObjStack& objects, int square, int& objID, int type)
{
	cout << "TRY ADD A BLACK" << endl;
	if (objects.find(square)->type > 20 && objects.find(square)->type < 27)
	{
		//		cout << "Tried to erase someone." << endl;
		objects.erase(square);
		//		cout << "I erased " << square << " at " << square << endl;
	}
	else
	{
		//		cout << "Tried to add someone." << endl;
		shape_ptr p = new shape;
		p->type = type;
		p->location = square;
		p->ambimat[0] = .1;
		p->ambimat[1] = .1;
		p->ambimat[2] = .1;
		p->specmat[0] = .5;
		p->specmat[1] = .5;
		p->specmat[2] = .5;
		p->diffmat[0] = .1;
		p->diffmat[1] = .1;
		p->diffmat[2] = .1;
		p->shininess = 8;
		p->position[0] = -3.5 + (square / 8);
		p->position[1] = 0.2;
		p->position[2] = -3.5 + (square % 8);
		p->rotation[0] = 0;
		p->rotation[1] = 0;
		p->rotation[2] = 0;
		p->scale[0] = .3;
		p->scale[1] = .3;
		p->scale[2] = .3;

		p->ID = objID;
		objID++;
		objects.push(p);
	}
}

void addWhite(ObjStack& objects, int square, int& objID, int type)
{
	cout << "TRY ADD A WHITE" << endl;
	if (objects.find(square)->type > 20 && objects.find(square)->type < 27) objects.erase(square);
	else
	{
		shape_ptr p = new shape;
		p->type = type;
		p->location = square;
		p->ambimat[0] = .7;
		p->ambimat[1] = .7;
		p->ambimat[2] = .7;
		p->specmat[0] = 1;
		p->specmat[1] = 1;
		p->specmat[2] = 1;
		p->diffmat[0] = .8;
		p->diffmat[1] = .8;
		p->diffmat[2] = .8;
		p->shininess = 8;
		p->position[0] = -3.5 + (square / 8);
		p->position[1] = 0.2;
		p->position[2] = -3.5 + (square % 8);
		p->rotation[0] = 0;
		p->rotation[1] = 0;
		p->rotation[2] = 0;
		p->scale[0] = .3;
		p->scale[1] = .3;
		p->scale[2] = .3;

		p->ID = objID;
		objID++;
		objects.push(p);
	}
}

void screenShot()
{
	FILE*    prototypeFile;
	FILE*    Output;
	GLubyte* pPixelData;
	GLubyte  BMP_Header[54];
	GLint    i, j;
	GLint    PixelDataLength;   

	i = WindowWidth * 3;
	while (i % 4 != 0) i++;
	PixelDataLength = i * WindowHeight;
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	prototypeFile = fopen("../resource/bitmap.bmp", "rb");
	if (prototypeFile == 0)
		cout << "Proto Failed" << endl;

	Output = fopen("result/grab.bmp", "wb");
	if (Output == 0)
		cout << "Output Failed" << endl;

	
	fread(BMP_Header, sizeof(BMP_Header), 1, prototypeFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, Output);
	fseek(Output, 0x0012, SEEK_SET);
	i = WindowWidth;
	j = WindowHeight;
	fwrite(&i, sizeof(i), 1, Output);
	fwrite(&j, sizeof(j), 1, Output);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WindowWidth, WindowHeight,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);


	fseek(Output, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, Output);

	fclose(prototypeFile);
	fclose(Output);
	free(pPixelData);
}

