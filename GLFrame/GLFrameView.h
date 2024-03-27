
// GLFrameView.h: CGLFrameView 类的接口
//

#pragma once
#include <Gl/gl.h>
#include <GL/glu.h>
#include <Gl/glut.h>

class CGLFrameView : public CView
{
protected: // 仅从序列化创建
	CGLFrameView() noexcept;
	DECLARE_DYNCREATE(CGLFrameView)

// 特性
public:
	CGLFrameDoc* GetDocument() const;
	CClientDC *pDC;
	HGLRC hglrc;
	void DrawSierpinski(void);

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CGLFrameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // GLFrameView.cpp 中的调试版本
inline CGLFrameDoc* CGLFrameView::GetDocument() const
   { return reinterpret_cast<CGLFrameDoc*>(m_pDocument); }
#endif

