#pragma once
#include <cmath>

#define VECTOR POINT
#define UNIT 5

/*struct VECTOR
{
	float x, y;
};*/

const POINT e[9] = { {0, 0}, {UNIT,0}, { UNIT,-UNIT}, {0,-UNIT}, {-UNIT,-UNIT }, {-UNIT,0}, {-UNIT,UNIT }, {0,UNIT }, { UNIT,UNIT} };

const float fdDes_zero = 1;
const int c = 1, density_zero = 1;
int siteCountInRow = WINDOWWIDTHCHILD / UNIT;
const float rlx_param = 1.5;

void CreateLattice(/*site param*/
	Site *site_array,  /*size_t site_count,*/ int site_width,
	/*ClientWindow param*/
	SIZE window_size, int &top)
{
	POINT site_pos = { 0, 0 };
	int sum_width = 0,
		sum_hight = 0;
	int neighbor[VECTORS_COUNT - 1] = { // indexes of the 8th neighbor sites
		+1,
		+siteCountInRow + 1,
		+siteCountInRow,
		+siteCountInRow - 1,
		-1,
		-siteCountInRow - 1,
		-siteCountInRow,
		-siteCountInRow + 1
	};
	for (top = 0; top < WINDOWHIGHTCHILD * WINDOWWIDTHCHILD / pow(UNIT, 2); top++, sum_width += site_width)
	{
		if (sum_width < window_size.cx)
			site_pos.x = sum_width;
		else {
			site_pos.x = sum_width = 0;
			site_pos.y = sum_hight += site_width;
		}
		site_array[top].pos = site_pos;
		site_array[top].size = { site_width, site_width };

		site_array[top].pos_center.x = site_pos.x + site_width / 2;
		site_array[top].pos_center.y = site_pos.y + site_width / 2;
		
		for (size_t i = 0; i < VECTORS_COUNT - 1; i++)
			site_array[top].neighbor[i] = top + neighbor[i];
		top = top;
	}
}

int iOfMyPoint(Site *site, POINT point, int &top_site) {
	int cur = -1; // index of the site to which the point belongs
	for (int i = 0; i < top_site; i++)
	{
		RECT rc = { site[i].pos.x, site[i].pos.y, site[i].pos.x + site[i].size.cx, site[i].pos.y + site[i].size.cy };
		if (PtInRect(&rc, point)) cur = i, i = top_site;
	}
	return cur;
}

void fellOnSite(Site *site, POINT point, int &top_site) { // inizializes part_count on nearests sites
																			// part count initialization
	int cur_site = iOfMyPoint(site, { point.x, point.y }, top_site);

	/*int indexes[9] = {
	 + 1,
	 + siteCountInRow + 1,
	 + siteCountInRow,
	 + siteCountInRow - 1,
	 - 1,
	 - siteCountInRow - 1,
	 - siteCountInRow,
	 - siteCountInRow + 1
	};*/


	for (size_t j = 0; j < VECTORS_COUNT; j++)
	{
		//site[indexes[j]].count_part = COUNT_PART;
	}
}
	
float density(Site *site, int i_ofBaseSite)
{
	float sum = 0;
	int k;
	for (int i = -1; i < VECTORS_COUNT / 3 - 1; i++)
		if ((k = i_ofBaseSite - siteCountInRow + i) >= 0)
			sum += site[k].count_part;
	for (int i = -1; i < VECTORS_COUNT / 3 - 1; i++)
		if ((k = i_ofBaseSite + i) >= 0)
			sum += site[k].count_part;
	for (int i = -1; i < VECTORS_COUNT / 3 - 1; i++)
		if ((i_ofBaseSite + siteCountInRow + i) >= 0)
			sum += site[k].count_part;
	return sum;
}

VECTOR Velocity(Site *site, int i_ofBaseSite) { // base site is site on which a drop of fluid fell
												/* work model

												a  b  c
												d  e  f
												g  h  i

												e - point which get fluid
												a, b, c, d, f, g, h, i - points of which velocities have to multiply
												*/

	VECTOR tmp;
	int k;
	if ((k = i_ofBaseSite - 1) >= 0) {
		tmp.x = e[k].x * site[k].count_part;
		tmp.y = e[k].y * site[k].count_part;
	}
	if ((k = i_ofBaseSite + 1) >= 0) {
		tmp.x = e[k].x * site[k].count_part;
		tmp.y = e[k].y * site[k].count_part;
	}
	for (int i = -1; i < VECTORS_COUNT / 3 - 1; i++)
	{
		if ((k = i_ofBaseSite - siteCountInRow + i) >= 0) {
			tmp.x += e[k].x * site[k].count_part;
			tmp.y += e[k].y * site[k].count_part;
		}
	}

	for (int i = -1; i < VECTORS_COUNT / 3 - 1; i++)
	{
		if ((k = i_ofBaseSite + siteCountInRow + i) >= 0) {
			tmp.x += e[k].x * site[k].count_part;
			tmp.y += e[k].y * site[k].count_part;
		}
	}
	return tmp;
}

float w(int i) {
	return !i ? (float)4.0 / 9 : (i >= 1 && i <= 4) ? (float)1.0 / 9 : (float)1.0 / 36;
}
	
void streaming(Site *site, int &top_site, int &delta_t) {
	int i, j, x, y, k;
	for (i = 0; i < top_site; i++)
	{
		for (j = 0; j < VECTORS_COUNT; j++)
		{
			x = site[i].pos_center.x + e[j].x /** delta_t*/,
			y = site[i].pos_center.y - e[j].y /** delta_t*/;
			k = iOfMyPoint(site, { x, y }, top_site);
			if (k >= 0)
				site[k].count_part += (1 - rlx_param) * site[i].count_part + rlx_param * site[i].equilibrium;
		}
		if (site[i].count_part)
			i = i;
	}
}

void colliding(Site *site, int &top_site) {
	float tmp, tmp2;
	int k;
	VECTOR vel;
	float mpl;
	int z = 0;
	for (int i = 0; i < top_site; i++)
		for (int j = 0; j < VECTORS_COUNT; j++)
		{
			// получаем индекс одного из 9 j-го соседа (вклюяая i-ий, тоесть базовый) и называем его k
			k = iOfMyPoint(site, { site[i].pos_center.x + e[j].x, site[i].pos_center.y - e[j].y}, top_site); 
			// данное вырожение означает: если существует, на заданных в функции выше координатах, елемент, то мы считаем ему equilibrium
			if (k >= 0) {
				vel = Velocity(site, k);
				mpl = e[j].x * vel.x + e[j].y * vel.y;
					
				tmp = (mpl)* 3 / pow(c, 2)
					+ (9 * pow(mpl, 2)) / 2 * pow(c, 4) - 3 * pow(vel.x, 2) + pow(vel.y, 2) / pow(c, 2);

				tmp *= density_zero;
				tmp += density(site, k);
				tmp *= w(k);

				site[k].equilibrium /*+*/= tmp;	
				
				z++;
			}
		}
}

void perform(Site *site, int &top_site, int &delta_t) {
	colliding(site, top_site);
	streaming(site, top_site, delta_t);
}