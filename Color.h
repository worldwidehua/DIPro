#pragma once

#include "Array.h"

typedef uchar RGB[3];
typedef uchar Gray;

void RGBMat2Arr(const cv::Mat& SrcMat, RGB* DstArr, int width, int height);
void RGBArr2Mat(const RGB* SrcArr, cv::Mat& DstMat, int width, int height);

void GrayMat2Arr(const cv::Mat& SrcMat, Gray* DstArr, int width, int height);
void GrayArr2Mat(const Gray* SrcArr, cv::Mat& DstMat, int width, int height);

void RGBArr2Gray(const RGB* SrcArr, Gray* DstArr, int width, int height);
void RGBMat2Gray(const cv::Mat& SrcMat, cv::Mat& DstMat);

void RGBMat2Arr(const cv::Mat& SrcMat, RGB* DstArr, int width, int height)
{
	assert(SrcMat.channels() == 3);
	assert(SrcMat.cols == width && SrcMat.rows == height);

	for (int i = 0; i < height; ++i)
	{
		const Gray* data = SrcMat.ptr<Gray>(i);
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < SrcMat.channels(); ++k)
			{
				DstArr[i*width + j][k] = *data++;
			}
		}
	}
}

void GrayMat2Arr(const cv::Mat& SrcMat, Gray* DstArr, int width, int height)
{
	assert(SrcMat.channels() == 1);
	assert(SrcMat.cols == width && SrcMat.rows == height);

	for (int i = 0; i < height; ++i)
	{
		const Gray* data = SrcMat.ptr<Gray>(i);
		for (int j = 0; j < width; ++j)
		{
			DstArr[i*width + j] = *data++;
		}
	}
}

void RGBArr2Mat(const RGB* SrcArr, cv::Mat& DstMat, int width, int height)
{
	assert(DstMat.channels() == 3);
	assert(DstMat.cols == width && DstMat.rows == height);

	for (int i = 0; i < height; ++i)
	{
		Gray* data = DstMat.ptr<Gray>(i);
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < DstMat.channels(); ++k)
			{
				*data++ = SrcArr[i*width + j][k];
			}
		}
	}
}

void GrayArr2Mat(const Gray* SrcArr, cv::Mat& DstMat, int width, int height)
{
	assert(DstMat.channels() == 1);
	assert(DstMat.cols == width && DstMat.rows == height);

	for (int j = 0; j < height; ++j)
	{
		Gray* data = DstMat.ptr<Gray>(j);
		for (int i = 0; i < width; ++i)
		{
			*data++ = SrcArr[j * width + i];
		}
	}
}

void RGBArr2Gray(const RGB* SrcArr, Gray* DstArr, int width, int height)
{
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			DstArr[j * width + i] = (SrcArr[j * width + i][0] * 299 + SrcArr[j * width + i][1] * 587 + SrcArr[j * width + i][2] * 114) / 1000;
		}
	}
}

void RGBMat2Gray(const cv::Mat& SrcMat, cv::Mat& DstMat)
{
	assert(SrcMat.channels() == 3 && DstMat.channels() == 1);
	assert(SrcMat.rows == DstMat.rows && SrcMat.cols == DstMat.cols);

	int width = SrcMat.cols;
	int height = SrcMat.rows;

	RGB* SrcArr = new RGB[width * height];
	RGBMat2Arr(SrcMat, SrcArr, width, height);

	Gray* DstArr = new Gray[width * height];
	RGBArr2Gray(SrcArr, DstArr, width, height);

	GrayArr2Mat(DstArr, DstMat, width, height);
}