#pragma once
#include<cmath>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;

class ColorTransfer
{
private:
	Mat sourceImage;
	Mat targetImage;

public:
	ColorTransfer(Mat source, Mat target) :sourceImage{ source }, targetImage{ target }
	{

	}
	Mat transferToLAB(Mat image);
	Mat transferToRGB(Mat image);

	Mat colorTransfer();
	~ColorTransfer();
};