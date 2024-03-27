#include "framework.h"

IMPLEMENT_SERIAL(CFigure, CObject, 1)
CFigure::CFigure(int l, int u, int r, int d)
{
	left = l;
	up = u;
	right = r;
	down = d;
	state = 0;
	fcolor = 0xffffff;
}


void CFigure::Offset(int cx, int cy)
{
	left += cx;
	right += cx;
	up += cy;
	down += cy;
}


void CFigure::onPress(int x, int y)
{
	sx = x; sy = y;
	state = 0;
	//选中图形
	if (left < x && x < right && up < y && y < down) 
	{
		state = 1;
		return;
	}

	if (left - f_width / 2 < x && x < left + f_width / 2)	state |= 2;	//	选中左边
	if (up - f_width / 2 < y && y < up + f_width / 2)	state |= 4;//选中上边
	if (right - f_width / 2 < x && x < right + f_width / 2)	state |= 8;//选中右边
	if (down - f_width / 2 < y && y < down + f_width / 2)	state |= 16;	//	选中下边

}


void CFigure::onRelease(int x, int y)
{
	state = 0;
}


void CFigure::SetBorderColor(int color)
{
	fcolor = color;
}


void CFigure::SetFillColor(int color)
{
	bcolor = color;
}


int CFigure::onMove(int x, int y)
{
	int cx, cy;
	cx = x - sx; cy = y - sy;
	sx = x; sy = y;

	if (state == 1) {
		Offset(cx, cy);		//  位移量cx,cy
	}

	if (2 == (state & 2)) {
		left = x;

	}

	if (4 == (state & 4)) {
		up = y;

	}

	if (8 == (state & 8)) {
		right = x;

	}

	if (16 == (state & 16)) {
		down = y;

	}
	return state == 0 ? 0 : 1;
}


void CFigure::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsLoading()) {
		ar >> left >> right >> up >> down >> f_width >> fcolor >> bcolor;
	}
	else
	{
		ar << left << right << up << down << f_width << fcolor << bcolor;
	}
}


CFigure::~CFigure()
{
}


void CFigure::onDraw(CDC* pDC) {
	CBrush b(fcolor);
	pDC->SelectObject(&b);
	CRect r(left, up, right, down);
	pDC->FillRect(&r, &b);
	CPen p(PS_SOLID, 1, bcolor);
	pDC->SelectObject(&p);
	pDC->Rectangle(left, up, right, down);
	pDC->MoveTo(left, up);
	pDC->DrawText(_T("空图形"), -1, new CRect(left, up, right, down), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
