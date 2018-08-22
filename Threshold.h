#pragma once

#include "Array.h"
#include "Color.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void TestThreshold()
{
	cv::Mat SrcImg = cv::imread("..\\..\\Data\\OtherImages\\kobe.jpg");

	cv::Mat GrayImg;

	clock_t start, finish;

	start = clock();

	cv::cvtColor(SrcImg, GrayImg, CV_BGR2GRAY);

	cv::Mat cvOTSUImg;
	cv::threshold(GrayImg, cvOTSUImg, 0, 255, CV_THRESH_OTSU);

	finish = clock();

	double duration = (double)(finish - start) / CLOCKS_PER_SEC;

	printf("%f seconds\n", duration);

	cv::imshow("SrcImg", SrcImg);
	cv::imshow("GrayImg", GrayImg);
	cv::imshow("OpenCV's OTSU", cvOTSUImg);
	cv::waitKey(0);
}