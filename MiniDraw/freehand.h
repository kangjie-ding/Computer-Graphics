#pragma once
#include "Figure.h"
class CFreehand :
	public CFigure
{
public:
	CFreehand() :CFreehand(50, 50, 100, 100) {}
	CFreehand(int l, int u, int r, int d);
	CPoint origin;
	bool state = FALSE;
	// void onDraw(CDC* pDC);
	int getGraphID() { return 4; }
	void startdraw(CPoint point);
	void ondraw(CPoint cur_point, CDC* pDC);
	void enddraw();
	~CFreehand();
};
