#pragma once
#include "Figure.h"
class CLine :
	public CFigure
{
public:
	CLine() :CLine(50, 50, 100, 100) {}
	CLine(int l, int u, int r, int d);
	void onDraw(CDC* pDC);
	int getGraphID() { return 1; }
	~CLine();
};
