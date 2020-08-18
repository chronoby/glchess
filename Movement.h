#pragma once
#include "defs.h"

class Movement
{
public:
	Movement();
	shape_ptr piece;
	int moveX;
	int moveY;
	void setObj(shape_ptr p);
	void setStart();
	void setFinish(int pickedObject);
	void on(bool value);
	bool on();
	int *getDifference();
private:
	int s,t,i,j;
	bool state;
	int getJ(int position);

};
