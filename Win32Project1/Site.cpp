#include "stdafx.h"
#include "Site.h"


Site::Site()
	: equilibrium(0)
{
}

void Site::set_pos(int x, int y) {
	pos.x = x;
	pos.y = y;
};
void Site::set_size(int cx, int cy) {
	size.cx = cx;
	size.cy = cy;
};

Site::~Site()
{
}
