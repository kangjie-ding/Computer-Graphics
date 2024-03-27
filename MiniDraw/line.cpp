#include "framework.h"

CLine::CLine(int l, int u, int r, int d) :CFigure(l, u, r, d)
{
	state = 0;
	fcolor = 0xffffff;
}


void CLine::onDraw(CDC* pDC)
{
	CPoint pts[2];
	pts[0].x = left;
	pts[0].y = (up + down) / 2;
	pts[1].x = right;
	pts[1].y = (up + down) / 2;
	CBrush b(fcolor);
	pDC->SelectObject(&b);
	CPen p(PS_SOLID, 1, bcolor);
	pDC->SelectObject(&p);
	pDC->Polygon(pts, 2);
}


CLine::~CLine()
{
}
