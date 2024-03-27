#include<iostream>
#include "colortransfer.h"

int main(int argc, char* argv[])
{

	Mat source = imread("C:\\Users\\92595\\Desktop\\�����ͼ��ѧ\\�ڶ��α����ҵ����\\Image-a.bmp");//Դͼ��
	Mat target = imread("C:\\Users\\92595\\Desktop\\�����ͼ��ѧ\\�ڶ��α����ҵ����\\Image-b.bmp");//Ŀ��ͼ��

	if (source.empty() || target.empty())  //���image�������ݣ������� image.empty()���� ��
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