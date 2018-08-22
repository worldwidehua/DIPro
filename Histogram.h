#pragma once

#include "Array.h"
#include "Color.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* HistCount(const Gray* SrcArr, int width, int height, int L = 256);
void HistEquil(const Gray* SrcArr, Gray* DstArr, int width, int height, int L = 256);

int* HistCount(const Gray* SrcArr, int width, int height, int L)
{
	int* DstArr = new int[L];
	for (int i = 0; i < L; ++i)
	{
		DstArr[i] = 0;
	}

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Gray data = SrcArr[j * width + i];
			if (data >= 0 && data < L)
			{
				DstArr[data]++;
			}
		}
	}

	return DstArr;
}

void HistEquil(const Gray* SrcArr, Gray* DstArr, int width, int height, int L)
{
	int* rArr = HistCount(SrcArr, width, height);
	int* sArr = new int[L];
	for (int i = 0; i < L; ++i)
	{
		sArr[i] = 0;
	}

	double coef = (L - 1.0) / (width * height);

	for (int i = 0; i < L; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			sArr[i] += rArr[j];
		}
		sArr[i] = Round(coef * sArr[i]);
	}

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			int sp = (int)SrcArr[j * width + i];
			if (sp >= 0 && sp < L)
			{
				DstArr[j * width + i] = sArr[sp];
			}
		}
	}
}

void TestHist()
{
	cv::Mat SrcImg = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH03\\Fig0316(2)(2nd_from_top).tif");

	int width = SrcImg.cols;
	int height = SrcImg.rows;

	RGB* SrcArr = new RGB[width * height];
	Gray* GrayArr = new Gray[width * height];

	RGBMat2Arr(SrcImg, SrcArr, width, height);
	RGBArr2Gray(SrcArr, GrayArr, width, height);

	clock_t start, finish;

	start = clock();

	Gray* HistEquilArr = new Gray[width * height];
	HistEquil(GrayArr, HistEquilArr, width, height);

	cv::Mat HistEquilMat = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(HistEquilArr, HistEquilMat, width, height);

	finish = clock();

	printf("my histequil time : %f seconds.\n", (double)(finish - start) / CLOCKS_PER_SEC);

	cv::Mat GrayImg;

	cv::cvtColor(SrcImg, GrayImg, CV_BGR2GRAY);

	start = clock();

	cv::Mat cvHistEquilImg = cv::Mat::zeros(height, width, CV_8UC1);
	cv::equalizeHist(GrayImg, cvHistEquilImg);

	finish = clock();

	printf("opencv's histequil time : %f seconds.\n", (double)(finish - start) / CLOCKS_PER_SEC);

	cv::imshow("TestHist", SrcImg);
	cv::namedWindow("TestHist");

	cv::imshow("HistEquilMat", HistEquilMat);
	cv::namedWindow("HistEquilMat");

	cv::imshow("OpenCV's HistEquil", cvHistEquilImg);
	cv::namedWindow("OpenCV's HistEquil");

	cv::waitKey(0);
}