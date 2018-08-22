#pragma once

#include "Array.h"
#include "Color.h"
#include "Math.h"

void DFT1D(const Complex* SrcArr, Complex* DstArr, int Len);
void IDFT1D(const Complex* SrcArr, Gray* DstArr, int Len);
void DFT2DNormal(const Complex* SrcArr, Complex* DstArr1, Gray* DstArr2, Gray* DstArr3, int width, int height);
void DFT2DSplite(const Complex* SrcArr, Complex* DstArr1, Gray* DstArr2, Gray* DstArr3, int width, int height);
void IDFT2D(const Complex* SrcArr, Complex* DstArr, int width, int height);
void FFT1D(const Complex* SrcArr, Complex* DstArr, int Len);
void FFT2D(const Complex* SrcArr, Complex* CompArr, Gray* AmpArr, Gray* AngleArr, int width, int height);

void DFT1D(const Complex* SrcArr, Complex* CompArr, int Len)
{
	Complex* Temp = new Complex[Len];
	for (int i = 0; i < Len; ++i)
	{
		Temp[i].Real = std::pow(-1, i) * SrcArr[i].Real;
		Temp[i].Imagin = std::pow(-1, i) * SrcArr[i].Imagin;
	}

	for (int i = 0; i < Len; ++i)
	{
		double Real = .0;
		double Imagin = .0;
		for (int j = 0; j < Len; ++j)
		{
			double sita = 2 * PI * (i * j / (double)Len);
			Real += std::cos(sita) * Temp[j].Real + std::sin(sita) * Temp[j].Imagin;
			Imagin += std::cos(sita) * Temp[j].Imagin - std::sin(sita) * Temp[j].Real;
		}
		CompArr[i].Real = Real;
		CompArr[i].Imagin = Imagin;
	}
}

void IDFT1D(const Complex* SrcArr, Complex* DstArr, int Len)
{
	for (int j = 0; j < Len; ++j)
	{
		double Real = .0;
		double Imagin = .0;
		for (int i = 0; i < Len; ++i)
		{
			double sita = 2 * PI * i * j / (double)Len;
			Real += std::cos(sita) * SrcArr[i].Real - std::sin(sita) * SrcArr[i].Imagin;
			Imagin += std::sin(sita) * SrcArr[i].Real + std::cos(sita) * SrcArr[i].Imagin;
		}
		Real /= Len;
		Imagin /= Len;
		DstArr[j].Real = Real;
		DstArr[j].Imagin = Imagin;
	}
}

void DFT2DNormal(const Complex* SrcArr, Complex* CompArr, Gray* AmpArr, Gray* AngleArr, int width, int height)
{
	Complex* TempArr1 = new Complex[width * height];
	double* TempArr2 = new double[width * height];
	double* TempArr3 = new double[width * height];

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			TempArr1[j * width + i].Real = std::pow(-1, i + j) * SrcArr[j * width + i].Real;
			TempArr1[j * width + i].Imagin = std::pow(-1, i + j) * SrcArr[j * width + i].Imagin;
		}
	}

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			double Real = .0;
			double Imagin = .0;
			for (int m = 0; m < height; ++m)
			{
				for (int n = 0; n < width; ++n)
				{
					double sita = 2 * PI * (i * n / (double)width + j * m / (double)height);
					Real += std::cos(sita) * TempArr1[m * width + n].Real + std::sin(sita) * TempArr1[m * width + n].Imagin;
					Imagin += std::cos(sita) * TempArr1[m * width + n].Imagin - std::sin(sita) * TempArr1[m * width + n].Real;
				}
			}
			CompArr[j * width + i].Real = Real;
			CompArr[j * width + i].Imagin = Imagin;
			TempArr2[j * width + i] = std::pow(std::pow(Real, 2) + std::pow(Imagin, 2), 0.5);
			TempArr2[j * width + i] = std::log(1 + TempArr2[j * width + i]);
			TempArr3[j * width + i] = std::atan2(Imagin, Real + EPSION);
		}
	}

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayT2Gray(TempArr2, AmpArr, width * height);
	arrObj->ArrayT2Gray(TempArr3, AngleArr, width * height);
}

void ComputePerDFT1D(const Complex* SrcArr, Complex& CompVal, int Len, int x)
{
	Complex* TempArr = new Complex[Len];
	for (int i = 0; i < Len; ++i)
	{
		TempArr[i].Real = std::pow(-1, i) * SrcArr[i].Real;
		TempArr[i].Imagin = std::pow(-1, i) * SrcArr[i].Imagin;
	}

	double Real = .0;
	double Imagin = .0;
	for (int i = 0; i < Len; ++i)
	{
		double sita = 2 * PI * (i * x / (double)Len);
		Real += std::cos(sita) * TempArr[i].Real + std::sin(sita) * TempArr[i].Imagin;
		Imagin += std::cos(sita) * TempArr[i].Imagin - std::sin(sita) * TempArr[i].Real;
	}
	CompVal.Real = Real;
	CompVal.Imagin = Imagin;
}

void DFT2DSplite(const Complex* SrcArr, Complex* CompArr, Gray* AmpArr, Gray* AngleArr, int width, int height)
{
	Complex* TempArr1 = new Complex[width];
	Complex* TempArr2 = new Complex[height];

	double* TempArr3 = new double[width * height];
	double* TempArr4 = new double[width * height];

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Complex Comp1;
			for (int m = 0; m < height; ++m)
			{
				for (int n = 0; n < width; ++n)
				{
					TempArr1[n].Real = SrcArr[m * width + n].Real;
					TempArr1[n].Imagin = SrcArr[m * width + n].Imagin;
				}
				ComputePerDFT1D(TempArr1, Comp1, width, i);
				TempArr2[m].Real = Comp1.Real;
				TempArr2[m].Imagin = Comp1.Imagin;
			}
			Complex Comp2;
			ComputePerDFT1D(TempArr2, Comp2, height, j);
			CompArr[j * width + i].Real = Comp2.Real;
			CompArr[j * width + i].Imagin = Comp2.Imagin;
			TempArr3[j * width + i] = std::pow(std::pow(Comp2.Real, 2) + std::pow(Comp2.Imagin, 2), 0.5);
			TempArr3[j * width + i] = std::log(1 + TempArr3[j * width + i]);
			TempArr4[j * width + i] = std::atan2(Comp2.Imagin, Comp2.Real + EPSION);
		}
	}

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayT2Gray(TempArr3, AmpArr, width * height);
	arrObj->ArrayT2Gray(TempArr4, AngleArr, width * height);
}

void IDFT2D(const Complex* SrcArr, Complex* DstArr, int width, int height)
{
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			double Real = .0;
			double Imagin = .0;
			for (int m = 0; m < height; ++m)
			{
				for (int n = 0; n < width; ++n)
				{
					double sita = 2 * PI * (i * n / (double)width + j * m / (double)height);
					Real += std::cos(sita) * SrcArr[m * width + n].Real + std::sin(sita) * SrcArr[m * width + n].Imagin;
					Imagin += std::cos(sita) * SrcArr[m * width + n].Imagin - std::sin(sita) * SrcArr[m * width + n].Real;
				}
			}
			DstArr[j * width + i].Real = Real;
			DstArr[j * width + i].Imagin = Imagin;
		}
	}
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			DstArr[j * width + i].Real = std::pow(-1, i + j) * DstArr[j * width + i].Real;
			DstArr[j * width + i].Imagin = std::pow(-1, i + j) * DstArr[j * width + i].Imagin;
		}
	}
}

void FFTRealRemap(double* SrcArr, int Len)
{
	if (Len == 1)
	{
		return;
	}

	double* Temp = new double[Len];
	for (int i = 0; i < Len; ++i)
	{
		if (i % 2 == 0)
		{
			Temp[i / 2] = SrcArr[i];
		}
		else
		{
			Temp[(Len + i) / 2] = SrcArr[i];
		}
	}

	for (int i = 0; i < Len; ++i)
	{
		SrcArr[i] = Temp[i];
	}

	delete Temp;

	FFTRealRemap(SrcArr, Len / 2);
	FFTRealRemap(SrcArr + Len / 2, Len / 2);

	return;
}

void FFTComplexRemap(Complex* SrcArr, int Len)
{
	if (Len == 1)
	{
		return;
	}

	Complex* Temp = new Complex[Len];
	for (int i = 0; i < Len; ++i)
	{
		if (i % 2 == 0)
		{
			ComplexCopy(&SrcArr[i], &Temp[i / 2]);
		}
		else
		{
			ComplexCopy(&SrcArr[i], &Temp[(Len + i) / 2]);
		}
	}

	for (int i = 0; i < Len; ++i)
	{
		ComplexCopy(&Temp[i], &SrcArr[i]);
	}

	delete Temp;
	FFTComplexRemap(SrcArr, Len / 2);
	FFTComplexRemap(SrcArr + Len / 2, Len / 2);

	return;
}

int IsBase2(int Len)
{
	int k = Len;
	int z = 0;
	while (k /= 2)
	{
		z++;
	}
	k = z;

	if (Len != (1 << k))
	{
		return -1;
	}

	return k;
}

void GetWN(int n, int Len, Complex* dst)
{
	double sita = 2 * PI * n / Len;
	dst->Real = std::cos(sita);
	dst->Imagin = std::sin(-sita);
}

void FFT1D(const Complex* SrcArr, Complex* DstArr, int Len)
{
	int k = IsBase2(Len);
	if (k == -1)
	{
		return;
	}

	Complex* CompArr = new Complex[Len];
	for (int i = 0; i < Len; ++i)
	{
		CompArr[i].Real = std::pow(-1, i) * SrcArr[i].Real;
		CompArr[i].Imagin = std::pow(-1, i) * SrcArr[i].Imagin;
	}
	FFTComplexRemap(CompArr, Len);

	for (int i = 0; i < k; ++i)
	{
		int z = 0;
		for (int j = 0; j < Len; ++j)
		{
			if ((j / (1 << i) % 2) == 1)
			{
				Complex wn;
				GetWN(z, Len, &wn);
				ComplexMulty(&CompArr[j], &wn, &CompArr[j]);
				z += 1 << (k - i - 1);
				int Neighboor = j - (1 << i);
				Complex Temp;
				ComplexCopy(&CompArr[Neighboor], &Temp);
				ComplexAdd(&Temp, &CompArr[j], &CompArr[Neighboor]);
				ComplexSub(&Temp, &CompArr[j], &CompArr[j]);
			}
			else
			{
				z = 0;
			}
		}
	}

	for (int i = 0; i < Len; ++i)
	{
		ComplexCopy(&CompArr[i], &DstArr[i]);
	}
}

void ColumnVector(Complex* src, Complex* dst, int width, int height)
{
	for (int i = 0; i < height; ++i)
	{
		ComplexCopy(&src[width * i], &dst[i]);
	}
}

void IColumnVector(Complex* src, Complex* dst, int width, int height)
{
	for (int i = 0; i < height; ++i)
	{
		ComplexCopy(&src[i], &dst[width * i]);
	}
}

void FFT2D(const Complex* SrcArr, Complex* CompArr, Gray* AmpArr, Gray* AngleArr, int width, int height)
{
	if (IsBase2(width) == -1 || IsBase2(height) == -1)
	{
		return;
	}

	Complex* TempArr1 = new Complex[width * height];
	double* TempArr2 = new double[width * height];
	double* TempArr3 = new double[width * height];

	for (int i = 0; i < height; ++i)
	{
		FFT1D(&SrcArr[width * i], &TempArr1[width * i], width);
	}

	Complex* Column = new Complex[height];
	for (int i = 0; i < width; ++i)
	{
		ColumnVector(&TempArr1[i], Column, width, height);
		FFT1D(Column, Column, height);
		IColumnVector(Column, &TempArr1[i], width, height);
	}

	for (int i = 0; i < width * height; ++i)
	{
		ComplexCopy(&TempArr1[i], &CompArr[i]);
		TempArr2[i] = std::pow(std::pow(TempArr1[i].Real, 2) + std::pow(TempArr1[i].Imagin, 2), 0.5);
		TempArr2[i] = std::log(1 + TempArr2[i]);
		TempArr3[i] = std::atan2(TempArr1[i].Imagin, TempArr1[i].Real + EPSION);
	}

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayT2Gray(TempArr2, AmpArr, width * height);
	arrObj->ArrayT2Gray(TempArr3, AngleArr, width * height);

	delete TempArr1;
	delete TempArr2;
	delete TempArr3;
	delete Column;
}

//void FFT2D(const Gray* SrcArr, Gray* DstArr, int width, int height)
//{
//	double* Temp1 = new double[width * height];
//	double* Temp2 = new double[width * height];
//
//	for (int j = 0; j < height; ++j)
//	{
//		for (int i = 0; i < width; ++i)
//		{
//			Temp1[j * width + i] = std::pow(-1, i + j) * SrcArr[j * width + i];
//		}
//	}
//	
//	int K1 = width / 2;
//	int K2 = height / 2;
//
//	for (int j = 0; j < height; ++j)
//	{
//		for (int i = 0; i < width; ++i)
//		{
//			double Real = .0;
//			double Imagin = .0;
//			for (int m = 0; m < K2; ++m)
//			{
//				for (int n = 0; n < K1; ++n)
//				{
//					double sita1 = 2 * PI * (i * n / (double)K1 + j * m / (double)K2);
//					double sita2 = 2 * PI * (i * n / (double)K1 + j * m / (double)K2 + i / (2.0 * K1) + j / (2.0 * K2));
//					double sita3 = 2 * PI * (i * n / (double)K1 + j * m / (double)K2 + i / (2.0 * K1));
//					double sita4 = 2 * PI * (i * n / (double)K1 + j * m / (double)K2 + j / (2.0 * K2));
//
//					double cos1 = std::cos(sita1);
//					double cos2 = std::cos(sita2);
//					double cos3 = std::cos(sita3);
//					double cos4 = std::cos(sita4);
//
//					double sin1 = std::sin(-sita1);
//					double sin2 = std::sin(-sita2);
//					double sin3 = std::sin(-sita3);
//					double sin4 = std::sin(-sita4);
//					
//					Real += cos1 * Temp1[2 * m * width + 2 * n] + cos2 * Temp1[(2 * m + 1) * width + 2 * n + 1] +
//						cos3 * Temp1[(2 * m + 1) * width + 2 * n] + cos4 * Temp1[2 * m * width + 2 * n + 1];
//					Imagin += sin1 * Temp1[2 * m * width + 2 * n] + sin2 * Temp1[(2 * m + 1) * width + 2 * n + 1] + 
//						sin3 * Temp1[(2 * m + 1) * width + 2 * n] + sin4 * Temp1[2 * m * width + 2 * n + 1];
//
//					Real += cos1 * Temp1[2 * m * width + 2 * n] + cos2 * Temp1[(2 * m + 1) * width + 2 * n + 1] -
//						cos3 * Temp1[(2 * m + 1) * width + 2 * n] - cos4 * Temp1[2 * m * width + 2 * n + 1];
//					Imagin += sin1 * Temp1[2 * m * width + 2 * n] + sin2 * Temp1[(2 * m + 1) * width + 2 * n + 1] - 
//						sin3 * Temp1[(2 * m + 1) * width + 2 * n] - sin4 * Temp1[2 * m * width + 2 * n + 1];
//					
//					Real += cos1 * Temp1[2 * m * width + 2 * n] - cos2 * Temp1[(2 * m + 1) * width + 2 * n + 1] -
//						cos3 * Temp1[(2 * m + 1) * width + 2 * n] + cos4 * Temp1[2 * m * width + 2 * n + 1];
//					Imagin += sin1 * Temp1[2 * m * width + 2 * n] - sin2 * Temp1[(2 * m + 1) * width + 2 * n + 1] - 
//						sin3 * Temp1[(2 * m + 1) * width + 2 * n] + sin4 * Temp1[2 * m * width + 2 * n + 1];
//
//					Real += cos1 * Temp1[2 * m * width + 2 * n] - cos2 * Temp1[(2 * m + 1) * width + 2 * n + 1] +
//						cos3 * Temp1[(2 * m + 1) * width + 2 * n] - cos4 * Temp1[2 * m * width + 2 * n + 1];
//					Imagin += sin1 * Temp1[2 * m * width + 2 * n] - sin2 * Temp1[(2 * m + 1) * width + 2 * n + 1] + 
//						sin3 * Temp1[(2 * m + 1) * width + 2 * n] - sin4 * Temp1[2 * m * width + 2 * n + 1];
//				}
//			}
//			Temp2[j * width + i] = std::pow(std::pow(Real, 2) + std::pow(Imagin, 2), 0.5);
//			Temp2[j * width + i] = std::log(1 + Temp2[j * width + i]);
//		}
//	}
//	ArrayCalibration(Temp2, DstArr, width * height);
//}

void TestDFTFFT()
{
	double* src = new double[16];

	for (int i = 0; i < 16; ++i)
	{
		src[i] = i + 1;
	}

	Complex* CompArr = new Complex[16];
	for (int i = 0; i < 16; ++i)
	{
		CompArr[i].Real = src[i];
		CompArr[i].Imagin = 0;
	}

	Complex* CompArr2 = new Complex[16];
	Complex* CompArr3 = new Complex[16];

	/*DFT1D(CompArr, CompArr2, 4);
	FFT1D(CompArr, CompArr3, 4);*/

	Gray* temp = new Gray[4];
	DFT2DSplite(CompArr, CompArr2, temp, temp, 4, 4);
	FFT2D(CompArr, CompArr3, temp, temp, 4, 4);

	cv::Mat M = (cv::Mat_<float>(4, 4) << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	cv::Mat planes[] = { cv::Mat_<float>(M), cv::Mat::zeros(M.size(), CV_32F) };
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	cv::dft(complexI, complexI);            // this way the result may fit in the source matrix

	std::cout << "DFT: " << std::endl;
	for (int i = 0; i < 16; ++i)
	{
		std::cout << CompArr2[i].Real << "+" << CompArr2[i].Imagin << "j" << std::endl;
	}

	std::cout << "FFT: " << std::endl;
	for (int i = 0; i < 16; ++i)
	{
		std::cout << CompArr3[i].Real << "+" << CompArr3[i].Imagin << "j" << std::endl;
	}

	std::cout << "Opencv DFT: " << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4 * 2; j += 2)
		{
			std::cout << complexI.at<float>(i, j) << "+" << complexI.at<float>(i, j + 1) << "j" << std::endl;
		}
	}

	getchar();
}

void TestFrequencyFiltering()
{
	cv::Mat SrcMat = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH04\\Fig0424(a)(rectangle).tif");

	int width = SrcMat.cols;
	int height = SrcMat.rows;

	cv::Mat GrayMat = cv::Mat::zeros(height, width, CV_8UC1);
	RGBMat2Gray(SrcMat, GrayMat);

	Gray* GrayArr = new Gray[width * height];
	GrayMat2Arr(GrayMat, GrayArr, width, height);

	Complex* CompArr1 = new Complex[width * height];
	Complex* CompArr2 = new Complex[width * height];
	Complex* CompArr3 = new Complex[width * height];
	Gray* AmpArr1 = new Gray[width * height];
	Gray* AngleArr1 = new Gray[width * height];
	Gray* IDFTArr = new Gray[width * height];

	for (int i = 0; i < width * height; ++i)
	{
		CompArr1[i].Real = GrayArr[i];
		CompArr1[i].Imagin = 0;
	}

	DFT2DSplite(CompArr1, CompArr2, AmpArr1, AngleArr1, width, height);
	IDFT2D(CompArr2, CompArr3, width, height);

	double* Temp = new double[width * height];
	for (int i = 0; i < width * height; ++i)
	{
		Temp[i] = CompArr3[i].Real;
	}

	Array<double>* arrObj = Array<double>::GetInstance();
	arrObj->ArrayT2Gray(Temp, IDFTArr, width * height);

	cv::Mat DFTMat1 = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(AmpArr1, DFTMat1, width, height);
	cv::Mat DFTMat2 = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(AngleArr1, DFTMat2, width, height);
	cv::Mat DFTMat3 = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(IDFTArr, DFTMat3, width, height);

	Complex* CompArr4 = new Complex[width * height];
	Gray* AmpArr2 = new Gray[width * height];
	Gray* AngleArr2 = new Gray[width * height];
	FFT2D(CompArr1, CompArr4, AmpArr2, AngleArr2, width, height);

	cv::Mat FFTMat1 = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(AmpArr2, FFTMat1, width, height);
	cv::Mat FFTMat2 = cv::Mat::zeros(height, width, CV_8UC1);
	GrayArr2Mat(AngleArr2, FFTMat2, width, height);

	cv::imshow("TestFrequencyFiltering", GrayMat);
	cv::namedWindow("TestFrequencyFiltering");

	cv::imshow("DFTMat1", DFTMat1);
	cv::namedWindow("DFTMat1");

	cv::imshow("DFTMat2", DFTMat2);
	cv::namedWindow("DFTMat2");

	cv::imshow("DFTMat3", DFTMat3);
	cv::namedWindow("DFTMat3");

	cv::imshow("FFTMat1", FFTMat1);
	cv::namedWindow("FFTMat1");

	cv::imshow("FFTMat2", FFTMat2);
	cv::namedWindow("FFTMat2");

	cv::waitKey(0);
}

void TestOpencvDFT()
{
	cv::Mat I = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH04\\Fig0427(a)(woman).tif", 0);

	cv::Mat padded;         
	int m = cv::getOptimalDFTSize(I.rows);
	int n = cv::getOptimalDFTSize(I.cols);

	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexI;
	merge(planes, 2, complexI);   

	cv::dft(complexI, complexI);  

	split(complexI, planes);    
	magnitude(planes[0], planes[1], planes[0]);  
	cv::Mat magI = planes[0];

	magI += cv::Scalar::all(1);
	log(magI, magI);               

	magI = magI(cv::Rect(0, 0, magI.cols&-2, magI.rows&-2));

	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   
	cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy)); 
	cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  
	cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); 

	cv::Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX);

	cv::imshow("I", I);
	cv::imshow("magI", magI);
	cv::waitKey(0);
}