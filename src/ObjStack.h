#pragma once
#include "defs.h"

class ObjStack 
{
public:
	ObjStack();
	void push(shape_ptr);
	shape_ptr erase(int);
	shape_ptr find(int);
	shape_ptr pop();
	shape_ptr getTop();
	int length();
private:
	shape_ptr top;
	int size;
};
