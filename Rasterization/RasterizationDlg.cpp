
// RasterizationDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Rasterization.h"
#include "RasterizationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRasterizationDlg 对话框



CRasterizationDlg::CRasterizationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RASTERIZATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	IsCompleted = FALSE;
	points.clear();
	YMAX = 0;
}

void CRasterizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRasterizationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CRasterizationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRasterizationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LINE, &CRasterizationDlg::OnBnClickedLine)
	ON_BN_CLICKED(IDC_CLEAR, &CRasterizationDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_CIRCLE, &CRasterizationDlg::OnBnClickedCircle)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_POLYGON, &PolygonFill)
END_MESSAGE_MAP()


// CRasterizationDlg 消息处理程序

BOOL CRasterizationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRasterizationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRasterizationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRasterizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRasterizationDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CRasterizationDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CRasterizationDlg::DrawLineBresenham(int x0, int y0, int x1, int y1)
{
	// 计算dx和dy
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	// 计算步长方向，根据步长方向来决定递增还是递减
	int stepX = (x0 < x1) ? 1 : -1;
	int stepY = (y0 < y1) ? 1 : -1;

	// 初始化p0和起始点
	int p = 2 * dy - dx;
	int x = x0;
	int y = y0;

	
	// 获取IDC_DRAWAREA的矩形区域
	CStatic* pStaticDrawArea = (CStatic*)GetDlgItem(IDC_DRAWAREA);
	CDC* pDC = pStaticDrawArea->GetDC();

	// 绘制起点
	pDC->SetPixel(x, y, RGB(0, 0, 0)); // 在指定位置绘制像素点

	// 循环绘制直线
	for (int k = 0; k < dx; k++)
	{
		// 根据pk决定下一个点的位置
		if (p < 0)
		{
			x += stepX;
			p += 2 * dy;
		}
		else
		{
			x += stepX;
			y += stepY;
			p += 2 * (dy - dx);
		}

		// 绘制像素点
		pDC->SetPixel(x, y, RGB(0, 0, 0));
	}

	// 释放设备
	pStaticDrawArea->ReleaseDC(pDC);
}


void CRasterizationDlg::OnBnClickedLine()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strX0, strY0, strX1, strY1;
	//得到起点和终点坐标
	GetDlgItemText(IDC_STARTX, strX0);
	GetDlgItemText(IDC_STARTY, strY0);
	GetDlgItemText(IDC_ENDX, strX1);
	GetDlgItemText(IDC_ENDY, strY1);

	int startX = _ttoi(strX0);
	int startY = _ttoi(strY0) + 15;
	int endX = _ttoi(strX1);
	int endY = _ttoi(strY1);

	//获取画框的边界矩形
	CRect rectDrawArea;
	GetDlgItem(IDC_DRAWAREA)->GetClientRect(rectDrawArea);

	//判断坐标是否在画框范围内
	if (startX >= 0 && startX <= rectDrawArea.Width() &&
		startY >= 0 && startY <= rectDrawArea.Height() &&
		endX >= 0 && endY <= rectDrawArea.Width() &&
		endY >= 0 && endY <= rectDrawArea.Height())
	{
		DrawLineBresenham(startX, startY, endX, endY);
	}
	else
	{
		CString message = _T("坐标超出绘图框的边界，请重新输入！");
		CString caption = _T("错误");
		MessageBox(message, caption, MB_ICONERROR);
	}

}


void CRasterizationDlg::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取画框的边界矩形
	CRect rectDrawArea;
	GetDlgItem(IDC_DRAWAREA)->GetClientRect(rectDrawArea);
	RedrawWindow(CRect(0, 0, 2*rectDrawArea.Width(), 2*rectDrawArea.Height()));
	IsCompleted = FALSE;
}


void CRasterizationDlg::DrawCircleBresenham(int centerX, int centerY, int radius)
{
	// 获取IDC_DRAWAREA的矩形区域
	CStatic* pStaticDrawArea = (CStatic*)GetDlgItem(IDC_DRAWAREA);
	CDC* pDC = pStaticDrawArea->GetDC();

	POINT point;
	int xi = 0;
	int yi = radius;
	int delt_i = (xi + 1) * (xi + 1) + (yi - 1) * (yi - 1) - radius * radius;
	int alpha;
	int Limit = 0;

	while (yi >= Limit) {
		point.x = centerX + xi;
		point.y = centerY + yi;
		pDC->SetPixel(point, RGB(0,0,0));
		point.x = centerX + xi;
		point.y = centerY - yi;
		pDC->SetPixelV(point, RGB(0, 0, 0));
		point.x = centerX - xi;
		point.y = centerY + yi;
		pDC->SetPixelV(point, RGB(0, 0, 0));
		point.x = centerX - xi;
		point.y = centerY - yi;
		pDC->SetPixelV(point, RGB(0, 0, 0));
		if (delt_i < 0) {
			alpha = 2 * delt_i + 2 * yi - 1;
			if (alpha <= 0) {
				//mh(xi, yi, delt_i);
				xi = xi + 1;
				delt_i = delt_i + 2 * xi + 1;
			}
			else {
				//md(xi, yi, delt_i);
				xi = xi + 1;
				yi = yi - 1;
				delt_i = delt_i + 2 * xi - 2 * yi + 2;
			}

		}
		else if (delt_i > 0) {
			alpha = 2 * delt_i - 2 * xi - 1;
			if (alpha <= 0) {
				//md(xi, yi, delt_i);
				xi = xi + 1;
				yi = yi - 1;
				delt_i = delt_i + 2 * xi - 2 * yi + 2;
			}
			else {
				//mv(xi, yi, delt_i);
				yi = yi - 1;
				delt_i = delt_i - 2 * yi + 1;
			}

		}
		else {
			//md(xi, yi, delt_i);
			xi = xi + 1;
			yi = yi - 1;
			delt_i = delt_i + 2 * xi - 2 * yi + 2;
		}
	}
	//// 释放设备
	pStaticDrawArea->ReleaseDC(pDC);
}

void CRasterizationDlg::OnBnClickedCircle()
{
	// TODO: 在此添加控件通知处理程序代码
	//得到圆心和半径
	CString strX, strY, strRadius;
	GetDlgItemText(IDC_CENTERX, strX);
	GetDlgItemText(IDC_CENTERY, strY);
	GetDlgItemText(IDC_RADIUS, strRadius);

	int centerX = _ttoi(strX);
	int centerY = _ttoi(strY);
	int radius = _ttoi(strRadius);

	//获取画框的边界矩形
	CRect rectDrawArea;
	GetDlgItem(IDC_DRAWAREA)->GetClientRect(rectDrawArea);

	//判断要绘制的圆的尺寸
	if (radius > 0 && radius <= rectDrawArea.Width() / 2 && radius <= rectDrawArea.Height())
	{
		DrawCircleBresenham(centerX, centerY, radius);
	}
	else
	{
		CString message = _T("半径超出绘图框的边界，请重新输入！");
		CString caption = _T("错误");
		MessageBox(message, caption, MB_ICONERROR);
	}
}


void CRasterizationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 若多边形绘制完毕，忽略该函数，不执行操作
	//获取画框的边界矩形
	CRect rectDrawArea;
	GetDlgItem(IDC_DRAWAREA)->GetClientRect(rectDrawArea);
	// 获取画板
	CStatic* DRAW = (CStatic*)GetDlgItem(IDC_DRAWAREA);
	CDC* pDC = DRAW->GetDC();
	point.x -= 35;
	point.y -= 35;
	if (IsCompleted == TRUE)
	{
		return;
	}
	else
	{
		// 如果是第一个点，存入points数组中
		if (points.empty())
		{
			points.push_back(point);
		}

		// 若不是第一个点，则连接当前点和上一个点
		else
		{
			// 获取points数组的最后一个point（就是上一个点），并将当前绘图位置移动到该点
			pDC->MoveTo(points[points.size() - 1]);
			// 连接上一个点和当前点
			pDC->LineTo(point);

			points.push_back(point); // 将当前点加入数组

			ReleaseDC(pDC);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRasterizationDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsCompleted == TRUE)
	{
		return;
	}

	else
	{
		// 如果前面少于3个点，则无法连接成为多边形，弹出错误信息
		if (points.size() < 3) {
			CString message = _T("顶点数量少于3，无法绘制多边形");
			CString caption = _T("多边形绘制错误");
			MessageBox(message, caption, MB_ICONERROR);
		}

		// 若points已存储有2个以上的顶点，则连接上一个点和第一个点
		else
		{
			// 获取画板
			CStatic* DRAW = (CStatic*)GetDlgItem(IDC_DRAWAREA);
			CDC* pDC = DRAW->GetDC();

			// 将绘图位置移动到上一个点
			pDC->MoveTo(points[points.size() - 1]);
			// 连接第一个点和当前点
			pDC->LineTo(points[0]);

			ReleaseDC(pDC);

			// 成功消息
			CString message = _T("多边形绘制完毕");
			CString caption = _T("多边形绘制完毕");
			MessageBox(message, caption, MB_OK);

			IsCompleted = TRUE;
		}
	}
	CDialogEx::OnRButtonDown(nFlags, point);
}

/**************************扫描线填充****************************/
Edge::Edge(float x_val, float dx_val, float y_val)
{
	x_lower = x_val;
	dx = dx_val;
	y_upper = y_val;
}

bool CRasterizationDlg::compareCPoint(const CPoint& a, const CPoint& b)
{
	return a.x < b.x;
}

void CRasterizationDlg::getET()
{
	for (int i = 1; i <= points.size(); i++)
	{
		float dx;
		float x_lower, y_lower, y_upper;

		// points中最后一个点和第一个点的连线
		if (i == points.size())
		{
			if (points[i - 1].x == points[0].x)
			{
				continue; // 边平行于x轴，不处理
			}
			else if (points[i - 1].y == points[0].y)
			{
				dx = 0.0;
			}
			else
			{
				dx = (points[i - 1].x - points[0].x) * 1.0 / (points[i - 1].y - points[0].y);
			}

			if (points[0].y <= points[i - 1].y)
			{
				y_lower = points[0].y;
				y_upper = points[i - 1].y;
				x_lower = points[0].x;
			}
			else
			{
				y_lower = points[i - 1].y;
				y_upper = points[0].y;
				x_lower = points[i - 1].x;
			}
			// 插入到Edge边信息中
			Edge e(x_lower, dx, y_upper);
			// 插入到ET表中
			ET[y_lower].push_back(e);
		}

		else
		{
			// 斜率
			if (points[i - 1].x == points[i].x)
			{
				continue; // 边平行于x轴，不处理
			}
			else if (points[i - 1].y == points[i].y)
			{
				dx = 0;
			}
			else
			{
				dx = (points[i - 1].x - points[i].x) * 1.0 / (points[i - 1].y - points[i].y);
			}

			// 线段上端点的x，y值
			if (points[i - 1].y <= points[i].y)
			{
				y_lower = points[i - 1].y;
				y_upper = points[i].y;
				x_lower = points[i - 1].x;
			}
			else
			{
				y_lower = points[i].y;
				y_upper = points[i - 1].y;
				x_lower = points[i].x;
			}

			// 计算多边形的最高点y值
			if (y_upper >= YMAX)
			{
				YMAX = y_upper;
			}

			// 插入到Edge边信息中
			Edge e(x_lower, dx, y_upper);
			// 插入到ET表中
			ET[y_lower].push_back(e);
		}
	}
}

void CRasterizationDlg::PolygonFill()
{
	// 获取ET表
	getET();

	// 从ymin到ymax进行扫描，将y初始化为ymin
	float y = ET.begin()->first;

	auto it_ET = ET.begin(); // AET初始化为ET表中ymin映射的边
	AET = (it_ET++)->second;

	while (y < YMAX)
	{
		// 移除已经超出扫描线的边
		std::vector<Edge> newAET;
		for (auto it = AET.begin(); it != AET.end(); ++it) {
			if (it->y_upper > y) {
				newAET.push_back(*it);
			}
		}
		AET = newAET;

		//检查是否有新的边加入
		if (it_ET != ET.end() && y >= it_ET->first) {
			AET.insert(AET.begin(), it_ET->second.begin(), it_ET->second.end());
			it_ET++;
		}

		// 获取活动边与当前扫描线的所有交点
		auto it = AET.begin();
		while (it != AET.end())
		{
			float x1 = it->x_lower;
			it->x_lower += it->dx;
			it++;

			if (it != AET.end())
			{
				float x2 = it->x_lower;
				it->x_lower += it->dx;
				it++;

				// 扫描线遇到两条线相交的顶点时，不记录交点
				if (x1 != x2)
				{
					scanPoints.push_back(CPoint(x1, y));
					scanPoints.push_back(CPoint(x2, y));
				}
			}
		}

		// 对扫描到的所有交点进行升序排列
		std::sort(scanPoints.begin(), scanPoints.end(), compareCPoint);

		// 获取画板
		CStatic* DRAW = (CStatic*)GetDlgItem(IDC_DRAWAREA);
		CDC* pDC = DRAW->GetDC();
		// 成对绘制，i每次+2
		for (int i = 0; i < scanPoints.size(); i += 2)
		{
			pDC->MoveTo(scanPoints[i]);
			pDC->LineTo(scanPoints[i + 1]);
		}
		ReleaseDC(pDC);

		// 当前扫描线已绘制完毕，清空交点，继续扫描
		scanPoints.clear();
		y += 1.0;
	}
}

void CRasterizationDlg::OnBnClickedPolygon()
{
	// TODO: 在此添加控件通知处理程序代码
	PolygonFill();
}
