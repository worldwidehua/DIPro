#pragma once

#include "Array.h"
#include "Color.h"

void LogTranform(const Gray* SrcArr, Gray* DstArr, int width, int height, int c = 1);

void LogTranform(const Gray* SrcArr, Gray* DstArr, int width, int height, int c)
{
	double* Temp = new double[width * height];

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Temp[j * width + i] = c * std::log(1 + SrcArr[j * width + i]);
		}
	}

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayT2Gray(Temp, DstArr, width * height);
}

void TestGrayTransform()
{
	cv::Mat SrcMat = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH03\\Fig0305(a)(DFT_no_log).tif");

	int width = SrcMat.cols;
	int height = SrcMat.rows;

	cv::Mat GrayMat = cv::Mat::zeros(height, width, CV_8UC1);
	RGBMat2Gray(SrcMat, GrayMat);

	Gray* GrayArr = new Gray[width * height];
	GrayMat2Arr(GrayMat, GrayArr, width, height);

	Gray* LogArr = new Gray[width * height];
	LogTranform(GrayArr, LogArr, width, height);

	cv::Mat LogMat = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(LogArr, LogMat, width, height);

	cv::imshow("TestGrayTransform", GrayMat);
	cv::namedWindow("TestGrayTransform");

	cv::imshow("LogMat", LogMat);
	cv::namedWindow("LogMat");

	cv::waitKey(0);
}