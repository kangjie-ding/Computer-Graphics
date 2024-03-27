#include "framework.h"

CFreehand::CFreehand(int l, int u, int r, int d) :CFigure(l, u, r, d)
{
	state = 0;
	fcolor = 0xffffff;
}


void CFreehand::startdraw(CPoint point)
{
	origin = point;
	state = TRUE;
}


void CFreehand::ondraw(CPoint cur_point, CDC* pDC)
{

	CBrush b(fcolor);
	pDC->SelectObject(&b);
	CPen p(PS_SOLID, 1, bcolor);
	pDC->SelectObject(&p);
	if (state == TRUE)
	{
		pDC->MoveTo(origin);
		pDC->LineTo(cur_point);
		origin = cur_point;
	}
}


void CFreehand::enddraw()
{
	state = FALSE;
}


// void CFreehand::onDraw(CDC* pDC)



CFreehand::~CFreehand()
{
}