#include<iostream>
#include "colortransfer.h"

int main(int argc, char* argv[])
{

	Mat source = imread("C:\\Users\\92595\\Desktop\\计算机图形学\\第二次编程作业资料\\Image-a.bmp");//源图像
	Mat target = imread("C:\\Users\\92595\\Desktop\\计算机图形学\\第二次编程作业资料\\Image-b.bmp");//目标图像

	if (source.empty() || target.empty())  //检测image有无数据，无数据 image.empty()返回 真
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	source.convertTo(source, CV_32FC3);
	target.convertTo(target, CV_32FC3);
	ColorTransfer transfer(source, target);
	Mat output = transfer.colorTransfer();
	output.convertTo(output, CV_8UC3);

	imwrite("../result/output.jpg", output);
	namedWindow("output image", WINDOW_AUTOSIZE);
	imshow("output image", output);
	waitKey(0);
	return 0;
}