#pragma once

#include "Array.h"
#include "Color.h"

void Relevant(double* SrcArr, double* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight);
void Convolution(double* SrcArr, double* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight);

void MeanValueFiltering(Gray* SrcArr, Gray* DstArr, int width, int height, int m_width = 3, int m_height = 3);

void NonSharpenShelter(Gray* SrcArr, Gray* DstArr, int width, int height, int k = 1);

void Relevant(double* SrcArr, double* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight)
{
	int a = (MaskHeight - 1) / 2;
	int b = (MaskWidth - 1) / 2;

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			double data = 0.0;
			for (int n = -a; n <= a; ++n)
			{
				for (int m = -b; m <= b; ++m)
				{
					if (i + m >= 0 && i + m < ImgWidth && j + n >= 0 && j + n < ImgHeight)
					{
						data += SrcArr[(j + n) * ImgWidth + i + m] * Mask[(n + a) * MaskWidth + m + b];
					}
				}
			}
			/*if (data < 0)
			{
			data = 0;
			}
			else if (data > 255)
			{
			data = 255;
			}*/
			DstArr[j * ImgWidth + i] = data;
		}
	}
}

void Convolution(double* SrcArr, double* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight)
{
	int a = (MaskHeight - 1) / 2;
	int b = (MaskWidth - 1) / 2;

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			double data = 0.0;
			for (int n = -a; n <= a; ++n)
			{
				for (int m = -b; m <= b; ++m)
				{
					if (i + m >= 0 && i + m < ImgWidth && j + n >= 0 && j + n < ImgHeight)
					{
						data += SrcArr[(j + n) * ImgWidth + i + m] * Mask[(MaskHeight - n - a) * MaskWidth + (MaskWidth - m - b)];
					}
				}
			}
			/*if (data < 0)
			{
			data = 0;
			}
			else if (data > 255)
			{
			data = 255;
			}*/
			DstArr[j * ImgWidth + i] = data;
		}
	}
}

void MeanValueFiltering(Gray* SrcArr, Gray* DstArr, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight)
{
	double* Mask = new double[MaskWidth * MaskHeight];

	for (int i = 0; i < MaskWidth * MaskHeight; ++i)
	{
		Mask[i] = 1.0 / (MaskWidth * MaskHeight);
	}

	double* SrcTemp = new double[ImgWidth * ImgHeight];
	double* DstTemp = new double[ImgWidth * ImgHeight];

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayGray2T(SrcArr, SrcTemp, ImgWidth, ImgHeight);
	Relevant(SrcTemp, DstTemp, Mask, MaskWidth, ImgHeight, MaskWidth, MaskHeight);
	arrObj->ArrayT2Gray(DstTemp, DstArr, ImgWidth, ImgHeight);
}

void NonSharpenShelter(Gray* SrcArr, Gray* DstArr, int ImgWidth, int ImgHeight, int k)
{
	/*
	k = 1 : Non Sharpen Shelter
	k > 1 : high elevate filtering
	*/
	double Mask[9] = { 1, 2, 1,
		2, 4, 2,
		1, 2, 1 };

	for (int i = 0; i < 9; ++i)
	{
		Mask[i] /= 16;
	}

	double* SrcTemp = new double[ImgWidth * ImgHeight];

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayGray2T(SrcArr, SrcTemp, ImgWidth, ImgHeight);

	double* BlurArr = new double[ImgWidth * ImgHeight];
	double* MaskArr = new double[ImgWidth * ImgHeight];

	Relevant(SrcTemp, BlurArr, Mask, ImgWidth, ImgHeight, 3, 3);

	arrObj->ArraySub(SrcTemp, BlurArr, MaskArr, ImgWidth, ImgHeight);
	arrObj->ArrayMulti(MaskArr, MaskArr, k, ImgWidth, ImgHeight);
	arrObj->ArrayAdd(SrcTemp, MaskArr, SrcTemp, ImgWidth, ImgHeight);
	arrObj->ArrayT2Gray(SrcTemp, DstArr, ImgWidth, ImgHeight);
}

void LaplaceSharpen(Gray* SrcArr, Gray* DstArr, int ImgWidth, int ImgHeight)
{
	double* SrcTemp = new double[ImgWidth * ImgHeight];
	double* DstTemp = new double[ImgWidth * ImgHeight];

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayGray2T(SrcArr, SrcTemp, ImgWidth, ImgHeight);

	double Mask[9] = { 1, 1, 1,
		1, -8, 1,
		1, 1, 1 };

	Relevant(SrcTemp, DstTemp, Mask, ImgWidth, ImgHeight, 3, 3);
	arrObj->ArrayT2Gray(DstTemp, DstArr, ImgWidth * ImgHeight);
}

void TestSpatialFiltering1()
{
	cv::Mat SrcMat = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH03\\Fig0333(a)(test_pattern_blurring_orig).tif");

	int ImgWidth = SrcMat.cols;
	int ImgHeight = SrcMat.rows;

	cv::Mat GrayMat = cv::Mat::zeros(ImgHeight, ImgWidth, CV_8UC1);
	RGBMat2Gray(SrcMat, GrayMat);

	Gray* GrayArr = new Gray[ImgWidth * ImgHeight];
	GrayMat2Arr(GrayMat, GrayArr, ImgWidth, ImgHeight);

	Array<double>* arrObj = Array<double>::GetInstance();

	double* SrcTemp = new double[ImgWidth * ImgHeight];
	arrObj->ArrayGray2T(GrayArr, SrcTemp, ImgWidth, ImgHeight);

	double Mask[9] = {
		1, 2, 1,
		2, 4, 2,
		1, 2, 1 };

	for (int i = 0; i < 9; ++i)
	{
		Mask[i] /= 16;
	}

	double* RelTemp = new double[ImgWidth * ImgHeight];
	Relevant(SrcTemp, RelTemp, Mask, ImgWidth, ImgHeight, 3, 3);

	Gray* RelArr = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrayT2Gray(RelTemp, RelArr, ImgWidth * ImgHeight);
	cv::Mat RelMat = cv::Mat::zeros(ImgHeight, ImgWidth, CV_8UC1);
	GrayArr2Mat(RelArr, RelMat, ImgWidth, ImgHeight);

	double* ConvTemp = new double[ImgWidth * ImgHeight];
	Convolution(SrcTemp, ConvTemp, Mask, ImgWidth, ImgHeight, 3, 3);

	Gray* ConvArr = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrayT2Gray(RelTemp, RelArr, ImgWidth * ImgHeight);
	cv::Mat ConvMat = cv::Mat::zeros(ImgHeight, ImgWidth, CV_8UC1);
	GrayArr2Mat(ConvArr, ConvMat, ImgWidth, ImgHeight);

	Gray* BlurArr = new Gray[ImgWidth * ImgHeight];
	MeanValueFiltering(GrayArr, BlurArr, ImgWidth, ImgHeight, 3, 3);

	cv::Mat BlurMat = cv::Mat::zeros(ImgHeight, ImgWidth, CV_8UC1);
	GrayArr2Mat(BlurArr, BlurMat, ImgWidth, ImgHeight);

	cv::imshow("TestSpatialFiltering1", GrayMat);
	cv::namedWindow("TestSpatialFiltering1");

	cv::imshow("RelMat", RelMat);
	cv::namedWindow("RelMat");

	cv::imshow("ConvMat", ConvMat);
	cv::namedWindow("ConvMat");

	cv::imshow("BlurMat", BlurMat);
	cv::namedWindow("BlurMat");

	cv::waitKey(0);
}

void TestSpatialFiltering2()
{
	cv::Mat SrcMat1 = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH03\\Fig0340(a)(dipxe_text).tif");

	int ImgWidth1 = SrcMat1.cols;
	int ImgHeight1 = SrcMat1.rows;

	cv::Mat GrayMat1 = cv::Mat::zeros(ImgHeight1, ImgWidth1, CV_8UC1);
	RGBMat2Gray(SrcMat1, GrayMat1);

	Gray* GrayArr1 = new Gray[ImgWidth1 * ImgHeight1];
	GrayMat2Arr(GrayMat1, GrayArr1, ImgWidth1, ImgHeight1);

	Gray* NssArr = new Gray[ImgWidth1 * ImgHeight1];
	NonSharpenShelter(GrayArr1, NssArr, ImgWidth1, ImgHeight1);

	cv::Mat NssMat = cv::Mat::zeros(ImgHeight1, ImgWidth1, CV_8UC1);
	GrayArr2Mat(NssArr, NssMat, ImgWidth1, ImgHeight1);

	cv::imshow("TestSpatialFiltering2", SrcMat1);
	cv::namedWindow("TestSpatialFiltering2");

	cv::imshow("NssMat", NssMat);
	cv::namedWindow("NssMat");

	cv::Mat SrcMat2 = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH03\\Fig0338(a)(blurry_moon).tif");

	int ImgWidth2 = SrcMat2.cols;
	int ImgHeight2 = SrcMat2.rows;

	cv::Mat GrayMat2 = cv::Mat::zeros(ImgHeight2, ImgWidth2, CV_8UC1);
	RGBMat2Gray(SrcMat2, GrayMat2);

	Gray* GrayArr2 = new Gray[ImgWidth2 * ImgHeight2];
	GrayMat2Arr(GrayMat2, GrayArr2, ImgWidth2, ImgHeight2);

	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* LapArr = new Gray[ImgWidth2 * ImgHeight2];
	LaplaceSharpen(GrayArr2, LapArr, ImgWidth2, ImgHeight2);
	arrObj->ArraySub(GrayArr2, LapArr, LapArr, ImgWidth2, ImgHeight2);

	cv::Mat LapMat = cv::Mat::zeros(ImgHeight2, ImgWidth2, CV_8UC1);
	GrayArr2Mat(LapArr, LapMat, ImgWidth2, ImgHeight2);

	cv::imshow("LaplaceSharpen", GrayMat2);
	cv::namedWindow("LaplaceSharpen");

	cv::imshow("LapMat", LapMat);
	cv::namedWindow("LapMat");

	cv::waitKey(0);
}