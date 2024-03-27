
// GLFrameView.cpp: CGLFrameView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GLFrame.h"
#endif

#include "GLFrameDoc.h"
#include "GLFrameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int m = 3; // number of recursive steps
GLfloat v[4][3] = { {0.0, 0.0, 1.0},{0.0, 0.942809, -0.33333},
	{-0.816497, -0.471405, -0.333333},{0.816497, -0.471405, -0.333333} };

GLfloat colors[4][3] = { {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},{0.0, 0.0, 0.0} };
// CGLFrameView

IMPLEMENT_DYNCREATE(CGLFrameView, CView)

BEGIN_MESSAGE_MAP(CGLFrameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGLFrameView 构造/析构

CGLFrameView::CGLFrameView() noexcept
{
	// TODO: 在此处添加构造代码

}

CGLFrameView::~CGLFrameView()
{
}

BOOL CGLFrameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// CGLFrameView 绘图

void CGLFrameView::OnDraw(CDC* /*pDC*/)
{
	CGLFrameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (hglrc)
		wglMakeCurrent(pDC->GetSafeHdc(), hglrc);
	else
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	DrawSierpinski();
	SwapBuffers(pDC->GetSafeHdc());
}


// CGLFrameView 打印

BOOL CGLFrameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGLFrameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGLFrameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CGLFrameView 诊断

#ifdef _DEBUG
void CGLFrameView::AssertValid() const
{
	CView::AssertValid();
}

void CGLFrameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLFrameDoc* CGLFrameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLFrameDoc)));
	return (CGLFrameDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLFrameView 消息处理程序


int CGLFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	int n;
	pDC = new CClientDC(this);
	ASSERT(pDC != NULL);
	static PIXELFORMATDESCRIPTOR pfd =

	{

		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd  
		1,                              // version number  

		PFD_DRAW_TO_WINDOW |            // support window  

		PFD_SUPPORT_OPENGL |            // support OpenGL  

		PFD_DOUBLEBUFFER,                // double buffered  

		PFD_TYPE_RGBA,                  // RGBA type  
		24,                             // 24-bit color depth  
		0, 0, 0, 0, 0, 0,               // color bits ignored  
		0,                              // no alpha buffer  
		0,                              // shift bit ignored  
		0,                              // no accumulation buffer  
		0, 0, 0, 0,                     // accum bits ignored  
		16,                             // 16-bit z-buffer  
		0,                              // no stencil buffer  
		0,                              // no auxiliary buffer  
		PFD_MAIN_PLANE,                 // main layer  
		0,                              // reserved  
		0, 0, 0                         // layer masks ignored  

	};
	int m_nPixelFormat = ::ChoosePixelFormat(pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)
	{
		return FALSE;
	}
	if (::SetPixelFormat(pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	n = ::GetPixelFormat(pDC->GetSafeHdc());
	::DescribePixelFormat(pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	//Create Rendering Context  
	hglrc = ::wglCreateContext(pDC->GetSafeHdc());
	//Failure to Create Rendering Context  
	if (hglrc == 0)
	{
		MessageBox(_T("Error Creating RC"));
		return FALSE;
	}
	//Make the RC Current  
	if (::wglMakeCurrent(pDC->GetSafeHdc(), hglrc) == FALSE)
	{
		MessageBox(_T("Error making RC Current"));
		return FALSE;
	}
	//Specify Black as the clear color  
	::glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//Specify the back of the buffer as clear depth  
	::glClearDepth(1.0f);
	//Enable Depth Testing  
	::glEnable(GL_DEPTH_TEST);
	return 0;
}


void CGLFrameView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL, NULL);
	::wglMakeCurrent(NULL, NULL);
	if (hglrc)
		::wglDeleteContext(hglrc);
	if (pDC)
		delete pDC;
	CView::OnDestroy();
}


void CGLFrameView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (hglrc)
		wglMakeCurrent(pDC->GetSafeHdc(), hglrc);
	else
		return;
	// TODO:  在此处添加消息处理程序代码
	if (0 >= cx || 0 >= cy)
	{
		return;
	}
	//select the full client area  
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLsizei w = cx;
	GLsizei h = cy;
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
			2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
			2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//CClientDC clientDC(this);
	//wglMakeCurrent(clientDC.m_hDC, hglrc);
	//glViewport(0, 0, w, h);
	//wglMakeCurrent(NULL, NULL);
	//RedrawWindow();   //显示更新  
}

//绘制三维镂垫的函数

void triangle(GLfloat* va, GLfloat* vb, GLfloat* vc)
{
	glBegin(GL_TRIANGLES);
	glVertex3fv(va);
	glVertex3fv(vb);
	glVertex3fv(vc);
	glEnd();
}

void tetra(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d)
{
	glColor3fv(colors[0]);
	triangle(a, b, c);
	glColor3fv(colors[1]);
	triangle(a, c, d);
	glColor3fv(colors[2]);
	triangle(a, d, b);
	glColor3fv(colors[3]);
	triangle(b, d, c);
}

void divide_tetra(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, int m)
{
	GLfloat mid[6][3];
	int j;
	if (m > 0) {
		/* compute six midpoints */
		for (j = 0; j < 3; j++) mid[0][j] = (a[j] + b[j]) / 2;
		for (j = 0; j < 3; j++) mid[1][j] = (a[j] + c[j]) / 2;
		for (j = 0; j < 3; j++) mid[2][j] = (a[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) mid[3][j] = (b[j] + c[j]) / 2;
		for (j = 0; j < 3; j++) mid[4][j] = (c[j] + d[j]) / 2;
		for (j = 0; j < 3; j++) mid[5][j] = (b[j] + d[j]) / 2;
		/* create 4 tetrahedrons by subdivision */
		divide_tetra(a, mid[0], mid[1], mid[2], m - 1);
		divide_tetra(mid[0], b, mid[3], mid[5], m - 1);
		divide_tetra(mid[1], mid[3], c, mid[4], m - 1);
		divide_tetra(mid[2], mid[4], d, mid[5], m - 1);
	}
	else
		tetra(a, b, c, d);
}

void CGLFrameView::DrawSierpinski(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  //设置背景颜色  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清除深度缓存和颜色缓存  
	//glMatrixMode(GL_MODELVIEW);  //启动模型矩阵  
	//glLoadIdentity();   //初始化为单位矩阵 
	glPushMatrix();
	divide_tetra(v[0], v[1], v[2], v[3], m);
	glPopMatrix();
	glFinish();
}

BOOL CGLFrameView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
