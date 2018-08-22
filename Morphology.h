#include "Array.h"
#include "Color.h"

#define BLACK_0_WHITE_1 1
#define BLACK_0_WHITE_255 2

void BinaryErode(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType = BLACK_0_WHITE_255);
void BinaryDilate(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType = BLACK_0_WHITE_255);
void BinaryOpen(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType = BLACK_0_WHITE_255);
void BinaryClose(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType = BLACK_0_WHITE_255);
void BinaryEdgeExtract(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType = BLACK_0_WHITE_255);

void BinaryArrINV(Gray* SrcArr, Gray* DstArr, int width, int height, int ImgType = BLACK_0_WHITE_255);
void BinaryArrAND(Gray* SrcArr1, Gray* SrcArr2, Gray* DstArr, int width, int height);
void BinaryArrOR(Gray* SrcArr1, Gray* SrcArr2, Gray* DstArr, int width, int height);
bool CompareTwoBinaryArr(Gray* Arr1, Gray* Arr2, int width, int height);

void BinaryErode(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType)
{
	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* SrcTemp = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrayCopy(SrcArr, SrcTemp, ImgWidth, ImgHeight);

	Gray ImgMax;
	if (ImgType == BLACK_0_WHITE_1)
	{
		ImgMax = 1;
	}
	else if (ImgType == BLACK_0_WHITE_255)
	{
		ImgMax = 255;
	}

	int a = (MaskHeight - 1) / 2;
	int b = (MaskWidth - 1) / 2;

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			bool flag = true;
			for (int n = -a; n <= a; ++n)
			{
				if (!flag)
				{
					break;
				}
				for (int m = -b; m <= b; ++m)
				{
					if (i + m >= 0 && i + m < ImgWidth && j + n >= 0 && j + n < ImgHeight)
					{
						if (Mask[(n + a) * MaskWidth + m + b] >= 0 && SrcTemp[(j + n) * ImgWidth + i + m] != Mask[(n + a) * MaskWidth + m + b])
						{
							DstArr[j * ImgWidth + i] = 0;
							flag = false;
							break;
						}
					}
				}
			}
			if (flag)
			{
				DstArr[j * ImgWidth + i] = ImgMax;
			}
		}
	}
}

void BinaryDilate(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType)
{
	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* SrcTemp = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrayCopy(SrcArr, SrcTemp, ImgWidth, ImgHeight);

	double* MaskTemp = new double[MaskWidth * MaskHeight];
	for (int j = 0; j < MaskHeight; ++j)
	{
		for (int i = 0; i < MaskWidth; ++i)
		{
			MaskTemp[j * MaskWidth + i] = Mask[(MaskHeight - 1 - j) * MaskWidth + MaskWidth - 1 - i];
		}
	}

	Gray ImgMax;
	if (ImgType == BLACK_0_WHITE_1)
	{
		ImgMax = 1;
	}
	else if (ImgType == BLACK_0_WHITE_255)
	{
		ImgMax = 255;
	}

	int a = (MaskHeight - 1) / 2;
	int b = (MaskWidth - 1) / 2;

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			bool flag = false;
			for (int n = -a; n <= a; ++n)
			{
				if (flag)
				{
					break;
				}
				for (int m = -b; m <= b; ++m)
				{
					if (i + m >= 0 && i + m < ImgWidth && j + n >= 0 && j + n < ImgHeight)
					{
						if (MaskTemp[(n + a) * MaskWidth + m + b] >= 0)
						{
							if (MaskTemp[(n + a) * MaskWidth + m + b] == SrcTemp[(j + n) * ImgWidth + i + m] && SrcTemp[(j + n) * ImgWidth + i + m] > 0)
							{
								DstArr[j * ImgWidth + i] = ImgMax;
								flag = true;
								break;
							}
						}
					}
				}
			}
			if (!flag)
			{
				DstArr[j * ImgWidth + i] = 0;
			}
		}
	}
}

void BinaryOpen(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType)
{
	BinaryErode(SrcArr, DstArr, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, ImgType);
	BinaryDilate(DstArr, DstArr, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, ImgType);
}

void BinaryClose(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType)
{
	BinaryDilate(SrcArr, DstArr, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, ImgType);
	BinaryErode(DstArr, DstArr, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, ImgType);
}

void BinaryEdgeExtract(Gray* SrcArr, Gray* DstArr, double* Mask, int ImgWidth, int ImgHeight, int MaskWidth, int MaskHeight, int ImgType)
{
	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	BinaryErode(SrcArr, DstArr, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, ImgType);
	arrObj->ArraySub(SrcArr, DstArr, DstArr, ImgWidth, ImgHeight);
}

void BinaryArrAND(Gray* SrcArr1, Gray* SrcArr2, Gray* DstArr, int width, int height)
{
	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* Temp1 = new Gray[width * height];
	Gray* Temp2 = new Gray[width * height];
	arrObj->ArrayCopy(SrcArr1, Temp1, width, height);
	arrObj->ArrayCopy(SrcArr2, Temp2, width, height);

	for (int i = 0; i < width * height; ++i)
	{
		if (Temp1[i] > 0 && Temp2[i] > 0)
		{
			DstArr[i] = Temp1[i];
		}
		else
		{
			DstArr[i] = 0;
		}
	}
}

void BinaryArrOR(Gray* SrcArr1, Gray* SrcArr2, Gray* DstArr, int width, int height)
{
	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* Temp1 = new Gray[width * height];
	Gray* Temp2 = new Gray[width * height];
	arrObj->ArrayCopy(SrcArr1, Temp1, width, height);
	arrObj->ArrayCopy(SrcArr2, Temp2, width, height);

	for (int i = 0; i < width * height; ++i)
	{
		if (Temp1[i] > 0 || Temp2[i] > 0)
		{
			if (Temp1[i] > 0)
			{
				DstArr[i] = Temp1[i];
			}
			else if (Temp2[i] > 0)
			{
				DstArr[i] = Temp2[i];
			}
		}
		else
		{
			DstArr[i] = 0;
		}
	}
}

void BinaryArrINV(Gray* SrcArr, Gray* DstArr, int width, int height, int ImgType)
{
	Gray ImgMax;
	if (ImgType == BLACK_0_WHITE_1)
	{
		ImgMax = 1;
	}
	else if (ImgType == BLACK_0_WHITE_255)
	{
		ImgMax = 255;
	}

	for (int i = 0; i < width * height; ++i)
	{
		DstArr[i] = ImgMax - SrcArr[i];
	}
}

bool CompareTwoBinaryArr(Gray* Arr1, Gray* Arr2, int width, int height)
{
	for (int i = 0; i < width * height; ++i)
	{
		if (Arr1[i] != Arr2[i])
		{
			return false;
		}
	}
	return true;
}

void ParallelThin(Gray* SrcArr, Gray* DstArr, int ImgWidth, int ImgHeight)
{
	for (int i = 0; i < ImgWidth * ImgHeight; ++i)
	{
		DstArr[i] = SrcArr[i] > 0 ? 255 : 0;
	}

	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* M = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrInit(M, ImgWidth, ImgHeight);

	int C = 0;

	while (true)
	{
		// first subiteration
		for (int j = 0; j < ImgHeight; ++j)
		{
			for (int i = 0; i < ImgWidth; ++i)
			{
				if (DstArr[j * ImgWidth + i] > 0)
				{
					int SatCon = 0;

					int P2T9[10];
					P2T9[2] = j - 1 >= 0 ? DstArr[(j - 1) * ImgWidth + i] : 0;
					P2T9[3] = j - 1 >= 0 && i + 1 < ImgWidth ? DstArr[(j - 1) * ImgWidth + i + 1] : 0;
					P2T9[4] = i + 1 < ImgWidth ? DstArr[j * ImgWidth + i + 1] : 0;
					P2T9[5] = j + 1 < ImgHeight && i + 1 < ImgWidth ? DstArr[(j + 1) * ImgWidth + i + 1] : 0;
					P2T9[6] = j + 1 < ImgHeight ? DstArr[(j + 1) * ImgWidth + i] : 0;
					P2T9[7] = j + 1 < ImgHeight && i - 1 >= 0 ? DstArr[(j + 1) * ImgWidth + i - 1] : 0;
					P2T9[8] = i - 1 >= 0 ? DstArr[j * ImgWidth + i - 1] : 0;
					P2T9[9] = j - 1 >= 0 && i - 1 >= 0 ? DstArr[(j - 1) * ImgWidth + i - 1] : 0;

					int BP = 0;
					for (int k = 2; k <= 9; ++k)
					{
						BP += P2T9[k];
					}
					if (BP >= 2 * 255 && BP <= 6 * 255)
					{
						SatCon += 1;
					}

					int AP = 0;
					for (int k = 2; k < 9; ++k)
					{
						if (P2T9[k] == 0 && P2T9[k + 1] > 0)
						{
							AP += 1;
						}
					}
					if (P2T9[9] == 0 && P2T9[2] > 0)
					{
						AP += 1;
					}
					if (AP == 1)
					{
						SatCon += 1;
					}

					if (P2T9[4] == 0 || P2T9[6] == 0 || (P2T9[2] == 0 && P2T9[8] == 0))
					{
						SatCon += 1;
					}

					if (SatCon == 3 && M[j * ImgWidth + i] == 0)
					{
						M[j * ImgWidth + i] = 255;
						C += 1;
					}
				}
			}
		}

		arrObj->ArraySub(DstArr, M, DstArr, ImgWidth, ImgHeight);

		if (C == 0)
		{
			break;
		}
		else
		{
			C = 0;
			arrObj->ArrInit(M, ImgWidth, ImgHeight);
			// second subiteration
			for (int j = 0; j < ImgHeight; ++j)
			{
				for (int i = 0; i < ImgWidth; ++i)
				{
					if (DstArr[j * ImgWidth + i] > 0)
					{
						int SatCon = 0;

						int P2T9[10];
						P2T9[2] = j - 1 >= 0 ? DstArr[(j - 1) * ImgWidth + i] : 0;
						P2T9[3] = j - 1 >= 0 && i + 1 < ImgWidth ? DstArr[(j - 1) * ImgWidth + i + 1] : 0;
						P2T9[4] = i + 1 < ImgWidth ? DstArr[j * ImgWidth + i + 1] : 0;
						P2T9[5] = j + 1 < ImgHeight && i + 1 < ImgWidth ? DstArr[(j + 1) * ImgWidth + i + 1] : 0;
						P2T9[6] = j + 1 < ImgHeight ? DstArr[(j + 1) * ImgWidth + i] : 0;
						P2T9[7] = j + 1 < ImgHeight && i - 1 >= 0 ? DstArr[(j + 1) * ImgWidth + i - 1] : 0;
						P2T9[8] = i - 1 >= 0 ? DstArr[j * ImgWidth + i - 1] : 0;
						P2T9[9] = j - 1 >= 0 && i - 1 >= 0 ? DstArr[(j - 1) * ImgWidth + i - 1] : 0;

						int BP = 0;
						for (int k = 2; k <= 9; ++k)
						{
							BP += P2T9[k];
						}
						if (BP >= 2 * 255 && BP <= 6 * 255)
						{
							SatCon += 1;
						}

						int AP = 0;
						for (int k = 2; k < 9; ++k)
						{
							if (P2T9[k] == 0 && P2T9[k + 1] > 0)
							{
								AP += 1;
							}
						}
						if (P2T9[9] == 0 && P2T9[2] > 0)
						{
							AP += 1;
						}
						if (AP == 1)
						{
							SatCon += 1;
						}

						if (P2T9[2] == 0 || P2T9[8] == 0 || (P2T9[4] == 0 && P2T9[6] == 0))
						{
							SatCon += 1;
						}

						if (SatCon == 3 && M[j * ImgWidth + i] == 0)
						{
							M[j * ImgWidth + i] = 255;
							C += 1;
						}
					}
				}
			}

			arrObj->ArraySub(DstArr, M, DstArr, ImgWidth, ImgHeight);

			if (C == 0)
			{
				break;
			}
		}
	}
}


void TestMorphology()
{
	cv::Mat SrcMat1 = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH09\\Fig0905(a)(wirebond-mask).tif");

	int ImgWidth1 = SrcMat1.cols;
	int ImgHeight1 = SrcMat1.rows;

	cv::Mat GrayMat1 = cv::Mat::zeros(ImgHeight1, ImgWidth1, CV_8UC1);
	RGBMat2Gray(SrcMat1, GrayMat1);

	Gray* GrayArr1 = new Gray[ImgWidth1 * ImgHeight1];
	GrayMat2Arr(GrayMat1, GrayArr1, ImgWidth1, ImgHeight1);

	int MaskWidth1 = 45;
	int MaskHeight1 = 45;
	double* Mask1 = new double[MaskWidth1 * MaskHeight1];
	for (int i = 0; i < MaskWidth1 * MaskHeight1; ++i)
	{
		Mask1[i] = 255;
	}

	Gray* ErodeArr = new Gray[ImgWidth1 * ImgHeight1];
	BinaryErode(GrayArr1, ErodeArr, Mask1, ImgWidth1, ImgHeight1, MaskWidth1, MaskHeight1);

	cv::Mat ErodeMat = cv::Mat::zeros(ImgHeight1, ImgWidth1, CV_8UC1);
	GrayArr2Mat(ErodeArr, ErodeMat, ImgWidth1, ImgHeight1);

	cv::imshow("GrayMat1", GrayMat1);
	cv::namedWindow("GrayMat1");

	cv::imshow("ErodeMat", ErodeMat);
	cv::namedWindow("ErodeMat");

	cv::waitKey(0);

	cv::Mat SrcMat2 = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH09\\Fig0907(a)(text_gaps_1_and_2_pixels).tif");

	int ImgWidth2 = SrcMat2.cols;
	int ImgHeight2 = SrcMat2.rows;

	cv::Mat GrayMat2 = cv::Mat::zeros(ImgHeight2, ImgWidth2, CV_8UC1);
	RGBMat2Gray(SrcMat2, GrayMat2);

	Gray* GrayArr2 = new Gray[ImgWidth2 * ImgHeight2];
	GrayMat2Arr(GrayMat2, GrayArr2, ImgWidth2, ImgHeight2);

	int MaskWidth2 = 3;
	int MaskHeight2 = 3;
	double Mask2[9] = {
		0, 255, 0,
		255, 255, 255,
		0, 255, 0 };

	Gray* DilateArr = new Gray[ImgWidth2 * ImgHeight2];
	BinaryDilate(GrayArr2, DilateArr, Mask2, ImgWidth2, ImgHeight2, MaskWidth2, MaskHeight2);

	cv::Mat DilateMat = cv::Mat::zeros(ImgHeight2, ImgWidth2, CV_8UC1);
	GrayArr2Mat(DilateArr, DilateMat, ImgWidth2, ImgHeight2);

	cv::imshow("GrayMat2", GrayMat2);
	cv::namedWindow("GrayMat2");

	cv::imshow("DilateMat", DilateMat);
	cv::namedWindow("DilateMat");

	cv::waitKey(0);

	cv::Mat SrcMat3 = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH09\\Fig0911(a)(noisy_fingerprint).tif");

	int ImgWidth3 = SrcMat3.cols;
	int ImgHeight3 = SrcMat3.rows;

	cv::Mat GrayMat3 = cv::Mat::zeros(ImgHeight3, ImgWidth3, CV_8UC1);
	RGBMat2Gray(SrcMat3, GrayMat3);

	Gray* GrayArr3 = new Gray[ImgWidth3 * ImgHeight3];
	GrayMat2Arr(GrayMat3, GrayArr3, ImgWidth3, ImgHeight3);

	int MaskWidth3 = 3;
	int MaskHeight3 = 3;
	double* Mask3 = new double[MaskWidth3 * MaskHeight3];
	for (int i = 0; i < MaskWidth3 * MaskHeight3; ++i)
	{
		Mask3[i] = 255;
	}

	Gray* OpenCloseArr = new Gray[ImgWidth3 * ImgHeight3];
	BinaryOpen(GrayArr3, OpenCloseArr, Mask3, ImgWidth3, ImgHeight3, MaskWidth3, MaskHeight3);
	BinaryClose(OpenCloseArr, OpenCloseArr, Mask3, ImgWidth3, ImgHeight3, MaskWidth3, MaskHeight3);

	cv::Mat OpenCloseMat = cv::Mat::zeros(ImgHeight3, ImgWidth3, CV_8UC1);
	GrayArr2Mat(OpenCloseArr, OpenCloseMat, ImgWidth3, ImgHeight3);

	cv::imshow("GrayMat3", GrayMat3);
	cv::namedWindow("GrayMat3");

	cv::imshow("OpenCloseMat", OpenCloseMat);
	cv::namedWindow("OpenCloseMat");

	cv::waitKey(0);

	cv::Mat SrcMat4 = cv::imread("..\\..\\Data\\DIP3E_Original_Images_CH09\\Fig0914(a)(licoln from penny).tif");

	int ImgWidth4 = SrcMat4.cols;
	int ImgHeight4 = SrcMat4.rows;

	cv::Mat GrayMat4 = cv::Mat::zeros(ImgHeight4, ImgWidth4, CV_8UC1);
	RGBMat2Gray(SrcMat4, GrayMat4);

	Gray* GrayArr4 = new Gray[ImgWidth4 * ImgHeight4];
	GrayMat2Arr(GrayMat4, GrayArr4, ImgWidth4, ImgHeight4);

	Gray* EdgeArr = new Gray[ImgWidth4 * ImgHeight4];
	BinaryEdgeExtract(GrayArr4, EdgeArr, Mask3, ImgWidth4, ImgHeight4, MaskWidth3, MaskHeight3);

	cv::Mat EdgeMat = cv::Mat::zeros(ImgHeight4, ImgWidth4, CV_8UC1);
	GrayArr2Mat(EdgeArr, EdgeMat, ImgWidth4, ImgHeight4);

	cv::imshow("GrayMat4", GrayMat4);
	cv::namedWindow("GrayMat4");

	cv::imshow("EdgeMat", EdgeMat);
	cv::namedWindow("EdgeMat");

	cv::waitKey(0);
}

void TestBinaryErode()
{
	Gray SrcArr[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	double Mask[] = {
		0, 1, 0,
		1, 1, 1,
		0, 1, 0,
	};

	int ImgWidth = 15;
	int ImgHeight = 7;

	int MaskWidth = 3;
	int MaskHeight = 3;

	Gray* DstArr = new Gray[ImgWidth * ImgHeight];

	BinaryErode(SrcArr, DstArr, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			std::cout << DstArr[j * ImgWidth + i] << " ";
		}
		std::cout << std::endl;
	}

	getchar();
}

void TestBinaryHitOrNotHit()
{
	Gray SrcArr[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	int ImgWidth = 16;
	int ImgHeight = 10;

	Gray* SrcInvArr = new Gray[ImgWidth * ImgHeight];
	BinaryArrINV(SrcArr, SrcInvArr, ImgWidth, ImgHeight, BLACK_0_WHITE_1);

	double MaskW[] = {
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
	};

	double MaskD[] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0,
	};

	int MaskWidth = 5;
	int MaskHeight = 5;

	Array<double>* arrObj = Array<double>::GetInstance();

	double* MaskW_D = new double[MaskWidth * MaskHeight];
	arrObj->ArraySub(MaskW, MaskD, MaskW_D, MaskWidth, MaskHeight);

	Gray* ErodeAD = new Gray[ImgWidth * ImgHeight];
	BinaryErode(SrcArr, ErodeAD, MaskD, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);

	Gray* ErodeAWD = new Gray[ImgWidth * ImgHeight];
	BinaryErode(SrcInvArr, ErodeAWD, MaskW_D, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);

	Gray* AXD = new Gray[ImgWidth * ImgHeight];
	BinaryArrAND(ErodeAD, ErodeAWD, AXD, ImgWidth, ImgHeight);

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			if (AXD[j * ImgWidth + i] == 0)
				std::cout << "0 ";
			else
				std::cout << "1 ";
		}
		std::cout << std::endl;
	}

	getchar();
}

void TestBinaryHoleFill()
{
	Gray SrcArr[] = {
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 0, 0, 0,
		0, 1, 0, 0, 1, 0, 0,
		0, 1, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 1, 0, 0,
		0, 0, 1, 0, 1, 0, 0,
		0, 1, 0, 0, 0, 1, 0,
		0, 1, 0, 0, 0, 1, 0,
		0, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
	};

	double Mask[] = {
		0, 1, 0,
		1, 1, 1,
		0, 1, 0,
	};

	int ImgWidth = 7;
	int ImgHeight = 10;

	int MaskWidth = 3;
	int MaskHeight = 3;

	Gray* SrcInvArr = new Gray[ImgWidth * ImgHeight];
	BinaryArrINV(SrcArr, SrcInvArr, ImgWidth, ImgHeight, BLACK_0_WHITE_1);

	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* X0 = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrInit(X0, ImgWidth, ImgHeight);
	X0[2 * ImgWidth + 2] = 1;

	Gray* XN = new Gray[ImgWidth * ImgHeight];
	while (true)
	{
		BinaryDilate(X0, XN, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		BinaryArrAND(XN, SrcInvArr, XN, ImgWidth, ImgHeight);

		if (CompareTwoBinaryArr(XN, X0, ImgWidth, ImgHeight))
		{
			break;
		}

		arrObj->ArrayCopy(XN, X0, ImgWidth, ImgHeight);

		std::cout << "------------------------" << std::endl;
		for (int j = 0; j < ImgHeight; ++j)
		{
			for (int i = 0; i < ImgWidth; ++i)
			{
				std::cout << XN[j * ImgWidth + i] << " ";
			}
			std::cout << std::endl;
		}
	}

	getchar();
}

void TestBinaryConnectExtract()
{
	Gray SrcArr[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	double Mask[] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
	};

	int ImgWidth = 10;
	int ImgHeight = 10;

	int MaskWidth = 3;
	int MaskHeight = 3;

	Array<Gray>* arrObj = Array<Gray>::GetInstance();

	Gray* X0 = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrInit(X0, ImgWidth, ImgHeight);
	X0[6 * ImgWidth + 3] = 1;

	Gray* X1 = new Gray[ImgWidth * ImgHeight];
	arrObj->ArrInit(X1, ImgWidth, ImgHeight);
	X1[1 * ImgWidth + 8] = 1;

	Gray* XN = new Gray[ImgWidth * ImgHeight];
	for (int i = 0; i < 6; ++i)
	{
		BinaryDilate(X0, XN, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		BinaryArrAND(XN, SrcArr, XN, ImgWidth, ImgHeight);

		arrObj->ArrayCopy(XN, X0, ImgWidth, ImgHeight);

		std::cout << "------------------------" << std::endl;
		for (int j = 0; j < ImgHeight; ++j)
		{
			for (int i = 0; i < ImgWidth; ++i)
			{
				std::cout << XN[j * ImgWidth + i] << " ";
			}
			std::cout << std::endl;
		}
	}

	Gray* XM = new Gray[ImgWidth * ImgHeight];
	for (int i = 0; i < 4; ++i)
	{
		BinaryDilate(X1, XM, Mask, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		BinaryArrAND(XM, SrcArr, XM, ImgWidth, ImgHeight);

		arrObj->ArrayCopy(XM, X1, ImgWidth, ImgHeight);

		std::cout << "------------------------" << std::endl;
		for (int j = 0; j < ImgHeight; ++j)
		{
			for (int i = 0; i < ImgWidth; ++i)
			{
				std::cout << XM[j * ImgWidth + i] << " ";
			}
			std::cout << std::endl;
		}
	}

	std::cout << "------------------------" << std::endl;

	Gray* X = new Gray[ImgWidth * ImgHeight];
	BinaryArrOR(XM, XN, X, ImgWidth, ImgHeight);

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			std::cout << X[j * ImgWidth + i] << " ";
		}
		std::cout << std::endl;
	}

	getchar();
}

void TestConvexHull()
{
	Gray SrcArr[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	double Mask1[] = {
		1, -1, -1,
		1, 0, -1,
		1, -1, -1,
	};

	int ImgWidth = 11;
	int ImgHeight = 12;

	int MaskWidth = 3;
	int MaskHeight = 3;

	Array<double>* arrObj = Array<double>::GetInstance();

	double* Mask2 = new double[MaskWidth * MaskHeight];
	double* Mask3 = new double[MaskWidth * MaskHeight];
	double* Mask4 = new double[MaskWidth * MaskHeight];

	arrObj->ArrayRotate(Mask1, Mask2, MaskWidth, MaskHeight, 90);
	arrObj->ArrayRotate(Mask1, Mask3, MaskWidth, MaskHeight, 180);
	arrObj->ArrayRotate(Mask1, Mask4, MaskWidth, MaskHeight, 270);

	Gray* D1 = new Gray[ImgWidth * ImgHeight];
	Gray* D2 = new Gray[ImgWidth * ImgHeight];
	Gray* D3 = new Gray[ImgWidth * ImgHeight];
	Gray* D4 = new Gray[ImgWidth * ImgHeight];

	Array<Gray>* arrObj1 = Array<Gray>::GetInstance();

	arrObj1->ArrayCopy(SrcArr, D1, ImgWidth, ImgHeight);
	arrObj1->ArrayCopy(SrcArr, D2, ImgWidth, ImgHeight);
	arrObj1->ArrayCopy(SrcArr, D3, ImgWidth, ImgHeight);
	arrObj1->ArrayCopy(SrcArr, D4, ImgWidth, ImgHeight);

	Gray* Temp1 = new Gray[ImgWidth * ImgHeight];
	Gray* Temp2 = new Gray[ImgWidth * ImgHeight];
	Gray* Temp3 = new Gray[ImgWidth * ImgHeight];
	Gray* Temp4 = new Gray[ImgWidth * ImgHeight];

	for (int j = 0; j < MaskHeight; ++j)
	{
		for (int i = 0; i < MaskWidth; ++i)
		{
			if (Mask1[j * MaskWidth + i] == 0)
			{
				std::cout << "0 ";
			}
			else if (Mask1[j * MaskWidth + i] == 1)
			{
				std::cout << "1 ";
			}
			else if (Mask1[j * MaskWidth + i] == -1)
			{
				std::cout << "-1 ";
			}
			//std::cout << CA[j * ImgWidth + i] << " ";
		}
		std::cout << std::endl;
	}

	while (true)
	{
		for (int j = 0; j < ImgHeight; ++j)
		{
			for (int i = 0; i < ImgWidth; ++i)
			{
				if (D1[j * ImgWidth + i] == 0)
				{
					std::cout << "0 ";
				}
				else if (D1[j * ImgWidth + i] == 1)
				{
					std::cout << "1 ";
				}
				//std::cout << CA[j * ImgWidth + i] << " ";
			}
			std::cout << std::endl;
		}
		getchar();
		BinaryErode(D1, D1, Mask1, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		for (int j = 0; j < ImgHeight; ++j)
		{
			for (int i = 0; i < ImgWidth; ++i)
			{
				if (D1[j * ImgWidth + i] == 0)
				{
					std::cout << "0 ";
				}
				else if (D1[j * ImgWidth + i] == 1)
				{
					std::cout << "1 ";
				}
				//std::cout << CA[j * ImgWidth + i] << " ";
			}
			std::cout << std::endl;
		}
		getchar();
		BinaryArrOR(D1, SrcArr, D1, ImgWidth, ImgHeight);


		if (CompareTwoBinaryArr(D1, Temp1, ImgWidth, ImgHeight))
		{
			break;
		}

		arrObj1->ArrayCopy(D1, Temp1, ImgWidth, ImgHeight);
	}

	while (true)
	{
		BinaryErode(D2, D2, Mask2, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		BinaryArrOR(D2, SrcArr, D2, ImgWidth, ImgHeight);

		if (CompareTwoBinaryArr(D2, Temp2, ImgWidth, ImgHeight))
		{
			break;
		}

		arrObj1->ArrayCopy(D2, Temp2, ImgWidth, ImgHeight);
	}
	while (true)
	{
		BinaryErode(D3, D3, Mask3, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		BinaryArrOR(D3, SrcArr, D3, ImgWidth, ImgHeight);

		if (CompareTwoBinaryArr(D3, Temp3, ImgWidth, ImgHeight))
		{
			break;
		}

		arrObj1->ArrayCopy(D3, Temp3, ImgWidth, ImgHeight);
	}
	while (true)
	{
		BinaryErode(D4, D4, Mask4, ImgWidth, ImgHeight, MaskWidth, MaskHeight, BLACK_0_WHITE_1);
		BinaryArrOR(D4, SrcArr, D4, ImgWidth, ImgHeight);

		if (CompareTwoBinaryArr(D4, Temp4, ImgWidth, ImgHeight))
		{
			break;
		}

		arrObj1->ArrayCopy(D4, Temp4, ImgWidth, ImgHeight);
	}

	Gray* CA = new Gray[ImgWidth * ImgHeight];
	BinaryArrOR(D1, D2, CA, ImgWidth, ImgHeight);
	BinaryArrOR(CA, D3, CA, ImgWidth, ImgHeight);
	BinaryArrOR(CA, D4, CA, ImgWidth, ImgHeight);



}

void TestThinAlgorithm()
{
	Gray IT[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	int ImgWidth = 10;
	int ImgHeight = 10;

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			std::cout << IT[j * ImgWidth + i] << " ";
		}
		std::cout << std::endl;
	}

	ParallelThin(IT, IT, ImgWidth, ImgHeight);

	for (int j = 0; j < ImgHeight; ++j)
	{
		for (int i = 0; i < ImgWidth; ++i)
		{
			if (IT[j * ImgWidth + i] == 0)
			{
				std::cout << "0" << " ";
			}
			else if (IT[j * ImgWidth + i] > 0)
			{
				std::cout << "1" << " ";
			}
		}
		std::cout << std::endl;
	}

	getchar();

	cv::Mat SrcMat = cv::imread("..\\..\\Data\\OtherImages\\Thin.jpg");

	ImgWidth = SrcMat.cols;
	ImgHeight = SrcMat.rows;

	cv::Mat GrayMat = cv::Mat::zeros(ImgHeight, ImgWidth, CV_8UC1);
	RGBMat2Gray(SrcMat, GrayMat);

	Gray* GrayArr = new Gray[ImgWidth * ImgHeight];
	GrayMat2Arr(GrayMat, GrayArr, ImgWidth, ImgHeight);

	Gray* ThinArr = new Gray[ImgWidth * ImgHeight];
	ParallelThin(GrayArr, ThinArr, ImgWidth, ImgHeight);

	ParallelThin(GrayArr, ThinArr, ImgWidth, ImgHeight);

	cv::Mat ThinMat = cv::Mat::zeros(ImgHeight, ImgWidth, CV_8UC1);
	GrayArr2Mat(ThinArr, ThinMat, ImgWidth, ImgHeight);

	cv::imshow("GrayMat", GrayMat);
	cv::namedWindow("GrayMat");

	cv::imshow("ThinMat", ThinMat);
	cv::namedWindow("ThinMat");

	cv::waitKey(0);
}