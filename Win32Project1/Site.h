#pragma once
#include "Resource.h"

class Site
{
private:
	//POINT pos;
	//SIZE size;
public:
	Site();
	void set_pos(int x, int y);
	void set_size(int cx, int cy);
	float count_part = 0;
	float equilibrium = 0;
	POINT pos;
	POINT pos_center = { 0 , 0};
	SIZE size;
	short neighbor[VECTORS_COUNT - 1];
	~Site();
};

