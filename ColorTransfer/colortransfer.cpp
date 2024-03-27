#include "colortransfer.h"

Mat ColorTransfer::transferToLAB(const Mat image)
{
	//从RGB颜色空间转换到l-alpha-beta颜色空间
	Mat temp = image.clone();
	Mat transferMatrix1 = (Mat_<double>(3, 3) << 0.3811, 0.5783, 0.0402, 0.1967, 0.7244, 0.0782, 0.0241, 0.1288, 0.8444);
	Mat transferMatrix2 = (Mat_<double>(3, 3) << 1.0 / sqrt(3), 1.0 / sqrt(3), 1.0 / sqrt(3), 1.0 / sqrt(6), 1.0 / sqrt(6), -2.0 / sqrt(6), 1.0 / sqrt(2), -1.0 / sqrt(2), 0);
	for (int i = 0; i < temp.rows; i++) 
	{
		for (int j = 0; j < temp.cols; j++) 
		{
			Vec3f temp_channel_array = temp.at<Vec3f>(i, j);//temp_channel_array临时的通道数组
			Mat channelMat = (Mat_<double>(3, 1) << temp_channel_array[0], temp_channel_array[1], temp_channel_array[2]);
			Mat tempResult = transferMatrix1 * channelMat;
			tempResult = (Mat_<double>(3,1) << log10(tempResult.at<double>(0)), log10(tempResult.at<double>(1)), log10(tempResult.at<double>(2)));
			tempResult = transferMatrix2 * tempResult;
			temp.at<Vec3f>(i, j) = Vec3f(tempResult.at<double>(0), tempResult.at<double>(1), tempResult.at<double>(2));
		}
	}
	return temp;
}


Mat ColorTransfer::transferToRGB(Mat image)
{
	//从l-alpha-beta颜色空间转换到RGB颜色空间
	Mat temp = image.clone();
	Mat transferMatrix1 = (Mat_<double>(3, 3) << sqrt(3) / 3, sqrt(6) / 6, sqrt(2) / 2, sqrt(3) / 3, sqrt(6) / 6, -sqrt(2) / 2, sqrt(3) / 3, -sqrt(6) / 3, 0);
	Mat transferMatrix2 = (Mat_<double>(3, 3) << 4.4679, -3.5873, 0.1193, -1.2186, 2.3809, -0.1624, 0.0497, -0.2439, 1.2045);
	for (int i = 0; i < temp.rows; i++)
	{
		for (int j = 0; j < temp.cols; j++)
		{
			Vec3f tempChannelArray = temp.at<Vec3f>(i, j);//temp_channel_array临时的通道数组
			Mat channelMat = (Mat_<double>(3, 1) << tempChannelArray[0], tempChannelArray[1], tempChannelArray[2]);
			Mat tempResult = transferMatrix1 * channelMat;
			tempResult = (Mat_<double>(3, 1) << pow(10, tempResult.at<double>(0)), pow(10, tempResult.at<double>(1)), pow(10, tempResult.at<double>(2)));
			tempResult = transferMatrix2 * tempResult;
			temp.at<Vec3f>(i, j) = Vec3f(tempResult.at<double>(0), tempResult.at<double>(1), tempResult.at<double>(2));
		}
	}
	return temp;
}


Mat ColorTransfer::colorTransfer()
{
	Mat sourceLAB = transferToLAB(sourceImage);
	Mat targetLAB = transferToLAB(targetImage);
	Mat sourceMean, sourceStd, targetMean, targetStd;
	//求解源图像和目标图像在l-alpha-beta空间下的标准差和均值
	meanStdDev(sourceLAB, sourceMean, sourceStd);
	meanStdDev(targetLAB, targetMean, targetStd);
	//利用论文所给公式进行转换
	Mat sourceMeanMat(sourceImage.rows, sourceImage.cols, CV_32FC3, Scalar(sourceMean.at<double>(0), sourceMean.at<double>(1), sourceMean.at<double>(2)));
	Mat targetMeanMat(sourceImage.rows, sourceImage.cols, CV_32FC3, Scalar(targetMean.at<double>(0), targetMean.at<double>(1), targetMean.at<double>(2)));
	Mat processed = sourceLAB - sourceMeanMat;
	for (int i = 0; i < sourceImage.rows; i++)
	{
		for (int j = 0; j < sourceImage.cols; j++)
		{
			Vec3f tempArray = processed.at<Vec3f>(i, j);
			processed.at<Vec3f>(i, j) = Vec3f(targetStd.at<double>(0) / sourceStd.at<double>(0) * tempArray[0], targetStd.at<double>(1) / sourceStd.at<double>(1) * tempArray[1], targetStd.at<double>(2) / sourceStd.at<double>(2) * tempArray[2]);
		}
	}
	processed = processed + targetMeanMat;
	Mat output = transferToRGB(processed);
	return output;
}


ColorTransfer::~ColorTransfer()
{
	cout << "the object has been deleted!" << endl;
}