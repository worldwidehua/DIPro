#pragma once

#include "Array.h"
#include "Color.h"
#include "Math.h"

cv::Mat ShiftImage(const cv::Mat& SrcMat, int sx, int sy);
cv::Mat RotateImage(const cv::Mat& SrcMat, double ra);
cv::Mat ScalingImage(const cv::Mat& SrcMat, double sx, double sy);

cv::Mat ShiftImage(const cv::Mat& SrcMat, int sx, int sy)
{
	assert(SrcMat.channels() == 1);

	int width = SrcMat.cols;
	int height = SrcMat.rows;

	Gray* SrcArr = new Gray[width * height];
	GrayMat2Arr(SrcMat, SrcArr, width, height);

	int Nwidth = width + sx;
	int Nheight = height + sy;

	Gray* DstArr = new Gray[Nwidth * Nheight];

	for (int i = 0; i < Nheight; ++i)
	{
		for (int j = 0; j < Nwidth; ++j)
		{
			Gray g = 0;
			if ((i - sy >= 0) && (i - sy < height) && (j - sx >= 0) && (j - sx < width))
			{
				g = SrcArr[(i - sy) * width + j - sx];
			}
			DstArr[i * Nwidth + j] = g;
		}
	}

	cv::Mat DstMat = cv::Mat::zeros(Nheight, Nwidth, CV_8UC1);
	GrayArr2Mat(DstArr, DstMat, Nwidth, Nheight);

	return DstMat;
}

cv::Mat RotateImage(const cv::Mat& SrcMat, double ra)
{
	assert(SrcMat.channels() == 1);

	int width = SrcMat.cols;
	int height = SrcMat.rows;

	Gray* SrcArr = new Gray[width * height];
	GrayMat2Arr(SrcMat, SrcArr, width, height);

	double sita = ra * PI / 180;

	double a = (width - 1) / 2.0;
	double b = (height - 1) / 2.0;

	double x1 = -a * std::cos(sita) - b * std::sin(sita);
	double y1 = -a * std::cos(sita) + b * std::sin(sita);

	double x2 = a * std::cos(sita) - b * std::sin(sita);
	double y2 = a * std::cos(sita) + b * std::sin(sita);

	double x3 = a * std::cos(sita) + b * std::sin(sita);
	double y3 = a * std::cos(sita) - b * std::sin(sita);

	double x4 = -a * std::cos(sita) + b * std::sin(sita);
	double y4 = -a * std::cos(sita) - b * std::sin(sita);

	int Nwidth = Round(std::max(std::abs(x1 - x3), std::abs(x2 - x4)));
	int Nheight = Round(std::max(std::abs(y1 - y3), std::abs(y2 - y4)));

	double c = (Nwidth - 1) / 2.0;
	double d = (Nheight - 1) / 2.0;

	double f1 = -c * cos(sita) + d * sin(sita) + a;
	double f2 = -c * cos(sita) - d * sin(sita) + b;

	Gray* DstArr = new Gray[Nwidth * Nheight];

	for (int j = 0; j < Nheight; ++j)
	{
		for (int i = 0; i < Nwidth; ++i)
		{
			int SrcX = Round(j * cos(sita) - i * sin(sita) + f1);
			int SrcY = Round(j * cos(sita) + i * sin(sita) + f2);

			if (SrcX >= 0 && SrcX < width && SrcY >= 0 && SrcY < height)
			{
				DstArr[j * Nwidth + i] = SrcArr[SrcY * width + SrcX];
			}
		}
	}

	cv::Mat DstMat = cv::Mat::zeros(Nheight, Nwidth, CV_8UC1);
	GrayArr2Mat(DstArr, DstMat, Nwidth, Nheight);

	return DstMat;
}

cv::Mat ScalingImage(const cv::Mat& SrcMat, double sx, double sy)
{
	assert(SrcMat.channels() == 1);

	int width = SrcMat.cols;
	int height = SrcMat.rows;

	Gray* SrcArr = new Gray[width * height];
	GrayMat2Arr(SrcMat, SrcArr, width, height);

	int Nwidth = (int)(width * sx);
	int Nheight = (int)(height * sy);

	Gray* DstArr = new Gray[Nwidth * Nheight];

	for (int j = 0; j < Nheight; ++j)
	{
		double dy = (double)((j + 0.5) / sy - 0.5);
		int iy = (int)std::floor(dy);
		dy -= iy;

		dy = std::max(.0, dy);
		iy = std::min(iy, height - 2);
		iy = std::max(0, iy);

		int cbufy[2];
		cbufy[0] = (int)((1.0 - dy) * 2048);
		cbufy[1] = 2048 - cbufy[0];

		for (int i = 0; i < Nwidth; ++i)
		{
			double dx = (double)((i + 0.5) / sx - 0.5);
			int ix = (int)std::floor(dx);
			dx -= ix;

			dx = std::max(.0, dx);
			ix = std::min(ix, width - 2);
			ix = std::max(0, ix);

			int cbufx[2];
			cbufx[0] = (int)((1.0 - dx) * 2048);
			cbufx[1] = 2048 - cbufx[0];

			DstArr[j*Nwidth + i] = (SrcArr[iy * width + ix] * cbufx[0] * cbufy[0] +
				SrcArr[(iy + 1) * width + ix] * cbufx[0] * cbufy[1] +
				SrcArr[iy * width + ix + 1] * cbufx[1] * cbufy[0] +
				SrcArr[(iy + 1) * width + ix + 1] * cbufx[1] * cbufy[1]) >> 22;
		}
	}

	cv::Mat DstMat = cv::Mat::zeros(Nheight, Nwidth, CV_8UC1);
	GrayArr2Mat(DstArr, DstMat, Nwidth, Nheight);

	return DstMat;
}

void TestAffineWrap()
{
	cv::Mat SrcImg = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH02\\Fig0236(a)(letter_T).tif");

	int width = SrcImg.cols;
	int height = SrcImg.rows;

	cv::Mat GrayImg = cv::Mat::zeros(height, width, CV_8UC1);
	RGBMat2Gray(SrcImg, GrayImg);

	int tx = 50, ty = 50;
	cv::Mat SftImg = ShiftImage(GrayImg, tx, ty);
	cv::Mat cvSftImg = cv::Mat::zeros(height, width, CV_8UC1);

	double ra = 45;
	cv::Mat RotImg = RotateImage(GrayImg, ra);

	cv::Point center = cv::Point(width / 2, height / 2);
	cv::Mat cvRotMat = cv::getRotationMatrix2D(center, ra, 1);
	cv::Mat ImgMat22 = cv::Mat::zeros(height, width, CV_8UC1);
	cv::warpAffine(GrayImg, ImgMat22, cvRotMat, GrayImg.size());

	double sx = 1.5, sy = 1.5;
	cv::Mat ScImg = ScalingImage(GrayImg, sx, sy);

	cv::imshow("TestAffineWrap", GrayImg);
	cv::namedWindow("TestAffineWrap");

	cv::imshow("ShiftImage", SftImg);
	cv::namedWindow("ShiftImage");

	cv::imshow("RotateImage", RotImg);
	cv::namedWindow("RotateImage");
	cv::imshow("OpenCV's RotateImage", cvRotMat);
	cv::namedWindow("OpenCV's RotateImage");

	cv::imshow("ScalingImage", ScImg);
	cv::namedWindow("ScalingImage");

	cv::waitKey(0);
}