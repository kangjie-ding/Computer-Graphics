#pragma once

#include<cmath>
#include<Eigen/Dense>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>

using namespace std;
using namespace cv;
using namespace Eigen;

class ImageWarp
{
private:
	int anchorNumber = 6;
	double sigma = 0.0;
	double lambda = 0.0;
	string radial = "";
	int sourcePoints[6][2];
	int targetPoints[6][2];

public:
	ImageWarp(int points1[6][2], int points2[6][2], string function);
	void setLambda(double Lambda)
	{
		lambda = Lambda;
	}
	void setSigma(double Sigma)
	{
		sigma = Sigma;
	}
	VectorXd parameterSolver1();
	VectorXd parameterSolver2();
	double radialFucntion(double x);
	int transferF(int i, int j, VectorXd x);
	Mat imageWarping(const Mat sourceImage);

	~ImageWarp();

};

