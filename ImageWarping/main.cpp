#include"imagewarp.h"

int main(int argc, char* argv[])
{
	Mat source = imread("C:\\Users\\92595\\Desktop\\计算机图形学\\第二次编程作业资料\\Mona_Lisa,_by_Leonardo_da_Vinci.jpg");

	if (source.empty())  //检测image有无数据，无数据 image.empty()返回 真
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	//锚点设置及参数初始化
	int sourceAnchor[6][2] = {662, 781, 660, 957, 812, 836, 890, 796, 913, 846, 885, 933};
	int targetAnchor[6][2] = {662, 781, 660, 957, 812, 836, 890, 796, 930, 846, 885, 933};
	ImageWarp transfer(sourceAnchor, targetAnchor, "Plate");
	transfer.setLambda(1000);
	transfer.setSigma(2.5);
	Mat output = transfer.imageWarping(source);
	imwrite("C:\\Users\\92595\\Desktop\\计算机图形学\\第二次编程作业资料\\happy.jpg", output);
	//namedWindow("output image", WINDOW_AUTOSIZE);
	//imshow("output image", output);
	//waitKey(0);
	return 0;
}