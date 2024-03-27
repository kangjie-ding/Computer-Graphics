#include "imagewarp.h"

ImageWarp::ImageWarp(int points1[6][2], int points2[6][2], string function)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sourcePoints[i][j] = points1[i][j];
			targetPoints[i][j] = points2[i][j];
		}
	}
	radial = function;
}

VectorXd ImageWarp::parameterSolver1()
{
	//求解横坐标径向基函数对应的参数
	MatrixXd A1(9, 9);
	MatrixXd b1(9, 1);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j <= 5)
			{
				if (i == j)
				{
					A1(i, j) = radialFucntion(sqrt(pow(double(sourcePoints[i][0]) - double(sourcePoints[j][0]), 2.0) + pow(double(sourcePoints[i][1]) - double(sourcePoints[j][1]), 2.0))) + lambda;
				}
				else
					A1(i, j) = radialFucntion(sqrt(pow(double(sourcePoints[i][0]) - double(sourcePoints[j][0]), 2.0) + pow(double(sourcePoints[i][1]) - double(sourcePoints[j][1]), 2.0)));
			}
			else if (j == 6)
				A1(i, j) = 1.0;
			else if (j == 7)
				A1(i, j) = double(sourcePoints[i][0]);
			else
				A1(i, j) = double(sourcePoints[i][1]);
		}
	}
	for (int i = 6; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j <= 5)
			{
				if (i == 6)
					A1(i, j) = 1.0;
				else if (i == 7)
					A1(i, j) = double(sourcePoints[j][0]);
				else
					A1(i, j) = double(sourcePoints[j][1]);
			}
			else
				A1(i, j) = 0.0;
		}
	}
	b1 << double(targetPoints[0][0]), double(targetPoints[1][0]), double(targetPoints[2][0]), double(targetPoints[3][0]), double(targetPoints[4][0]), double(targetPoints[5][0]),
		0.0, 0.0, 0.0;
	VectorXd x1 = A1.inverse() * b1;
	return x1;
}

VectorXd ImageWarp::parameterSolver2()
{
	//求解纵坐标径向基函数对应的参数
	MatrixXd A2(9, 9);
	MatrixXd b2(9, 1);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j <= 5)
			{
				if (i == j)
					A2(i, j) = radialFucntion(sqrt(pow(double(sourcePoints[i][0]) - double(sourcePoints[j][0]), 2.0) + pow(double(sourcePoints[i][1]) - double(sourcePoints[j][1]), 2.0))) + lambda;
				else
					A2(i, j) = radialFucntion(sqrt(pow(double(sourcePoints[i][0]) - double(sourcePoints[j][0]), 2.0) + pow(double(sourcePoints[i][1]) - double(sourcePoints[j][1]), 2.0)));
			}
			else if (j == 6)
				A2(i, j) = 1.0;
			else if (j == 7)
				A2(i, j) = double(sourcePoints[i][0]);
			else
				A2(i, j) = double(sourcePoints[i][1]);
		}
	}
	for (int i = 6; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j <= 5)
			{
				if (i == 6)
					A2(i, j) = 1.0;
				else if (i == 7)
					A2(i, j) = double(sourcePoints[j][0]);
				else
					A2(i, j) = double(sourcePoints[j][1]);
			}
			else
				A2(i, j) = 0.0;
		}
	}
	b2 << double(targetPoints[0][1]), double(targetPoints[1][1]), double(targetPoints[2][1]), double(targetPoints[3][1]), double(targetPoints[4][1]), double(targetPoints[5][1]),
		0.0, 0.0, 0.0;
	VectorXd x2 = A2.inverse() * b2;
	return x2;
}

int ImageWarp::transferF(int i, int j, VectorXd x)
{
	//根据得到的径向基函数参数向量x对像素坐标(i,j)进行转换
	double result = x[6] + x[7] * double(i) + x[8] * double(j) + x[0] * radialFucntion(sqrt(pow(double(i) - double(sourcePoints[0][0]), 2.0) + pow(double(j) - double(sourcePoints[0][1]), 2.0))) +
		x[1] * radialFucntion(sqrt(pow(double(i) - double(sourcePoints[1][0]), 2.0) + pow(double(j) - double(sourcePoints[1][1]), 2.0))) +
		x[2] * radialFucntion(sqrt(pow(double(i) - double(sourcePoints[2][0]), 2.0) + pow(double(j) - double(sourcePoints[2][1]), 2.0))) +
		x[3] * radialFucntion(sqrt(pow(double(i) - double(sourcePoints[3][0]), 2.0) + pow(double(j) - double(sourcePoints[3][1]), 2.0))) +
		x[4] * radialFucntion(sqrt(pow(double(i) - double(sourcePoints[4][0]), 2.0) + pow(double(j) - double(sourcePoints[4][1]), 2.0))) +
		x[5] * radialFucntion(sqrt(pow(double(i) - double(sourcePoints[5][0]), 2.0) + pow(double(j) - double(sourcePoints[5][1]), 2.0)));
	return int(result);
}

Mat ImageWarp::imageWarping(const Mat sourceImage)
{
	VectorXd x1 = parameterSolver1();
	VectorXd x2 = parameterSolver2();
	Mat temp = sourceImage;
	//int u_max = 0; 
	//int v_max = 0;
	//设置输出图片像素尺寸
	Mat output(3050, 1950, CV_8UC3);
	int count = 0;
	//进行图片转换
	for (int i = 0; i < temp.rows; i++)
	{
		for (int j = 0; j < temp.cols; j++)
		{
			Vec3b temp_channel_array = temp.at<Vec3b>(i, j);
			int u = transferF(i, j, x1);
			int v = transferF(i, j, x2);
			//if (u > u_max)
			//	u_max = u;
			//if (v > v_max)
			//	v_max = v;
			if (u >= 0 && v >= 0)
				output.at<Vec3b>(u, v) = temp_channel_array;
		}
	}
	//cout << u_max << " " << v_max << endl;
	return output;
}

double ImageWarp::radialFucntion(double x)
{
	if (radial == "Gaussian")
	{
		return exp(-pow(x, 2.0) / pow(sigma, 2.0));
	}
	else if(radial == "Plate")
	{
		if (x == 0)
			return 0;
		return pow(x, 2.0) * log(x);
	}
	else
	{
		cerr << "the radial function has not been set!!" << endl;
	}
	return 0;
}

ImageWarp::~ImageWarp()
{
	cout << "the object has been deleted!" << endl;
}
