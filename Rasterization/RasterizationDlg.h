
// RasterizationDlg.h: 头文件
//
#include <map>
#include <vector>
#include <algorithm>
#pragma once

// 边信息
class Edge
{
public:
	float x_lower; // 扫描线与边的交点x值（初始值为线段下端点x值）
	float dx; // 斜率倒数
	float y_upper; // 线段上端点y值

	Edge() : x_lower(0.0), dx(0.0), y_upper(0.0) {}
	Edge(float x_val, float dx_val, float y_val);
};


// CRasterizationDlg 对话框
class CRasterizationDlg : public CDialogEx
{
// 构造
public:
	std::vector<CPoint> points; // 存储多边形的顶点
	std::vector<CPoint> scanPoints; // 存储扫描线与活动边的交点
	BOOL IsCompleted; // 标志多边形是否绘制完毕
	std::map<int, std::vector<Edge>> ET; // ET表：从线段下端点y值到边信息链表的映射
	std::vector<Edge> AET; // 活动边表

	float YMAX; // 存储多边形最高点

	void getET();
	void PolygonFill();
	static bool compareCPoint(const CPoint& a, const CPoint& b);
	CRasterizationDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RASTERIZATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLine();
	void DrawLineBresenham(int x0, int y0, int x1, int y1);
	void DrawCircleBresenham(int centerX, int centerY, int radius);
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedCircle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedPolygon();
};
