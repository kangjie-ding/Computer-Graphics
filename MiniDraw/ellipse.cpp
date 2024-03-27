#include "framework.h"

CEllipse::CEllipse(int l, int u, int r, int d) :CFigure(l, u, r, d)
{
	state = 0;
	fcolor = 0xffffff;
}


void CEllipse::onDraw(CDC* pDC)
{
	CPen p(PS_SOLID, 1, bcolor);
	pDC->SelectObject(&p);
	pDC->Ellipse(left, up, right, down);
}


CEllipse::~CEllipse()
{
}
