#pragma once
#include "Figure.h"
class CEllipse :
	public CFigure
{
public:
	//DECLARE_SERIAL(CFigure)
	//void Serialize(CArchive& ar);
	CEllipse() :CFigure(50, 50, 100, 100) {}
	CEllipse(int l, int u, int r, int d);
	void onDraw(CDC* pDC);
	int getGraphID() { return 3; }
	~CEllipse();
};