#pragma once
#include "ObjLoader.h"
#include "ObjStack.h"

void boardBase();
void fieldSquare(int index);
void drawPiece(ObjLoader &shape);

void addWhite(ObjStack& objects, int square, int& objID, int type);
void addBlack(ObjStack& objects, int square, int& objID, int type);

void screenShot();
