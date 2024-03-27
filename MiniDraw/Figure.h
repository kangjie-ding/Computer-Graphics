#pragma once
class CFigure :
	public CObject
{
protected:
	//�߿�
	DECLARE_SERIAL(CFigure)
	int left, up, right, down;
	//ѡ��״̬
	unsigned int state;
	int sx, sy;
	int  f_width = 5;
	int fcolor = 0xffffff, bcolor = 0;

public:
	CFigure() :CFigure(50, 50, 100, 100) {

	}
	CFigure(int l, int u, int r, int d);
	void Offset(int cx, int cy);
	void  onPress(int x, int y);	//  ��갴��
	int  onMove(int cx, int cy);	//  ����ƶ�
	void  onRelease(int x, int y);	//  ����ͷ�
	virtual void onDraw(CDC* pDC);
	virtual int getGraphID() { return 0; }
	virtual void Serialize(CArchive& ar);
	virtual void startdraw(CPoint point){}
	virtual void ondraw(CPoint cur_point, CDC* pDC){}
	virtual void enddraw(){}
	void SetFillColor(int color);
	void SetBorderColor(int color);
	~CFigure();
};

