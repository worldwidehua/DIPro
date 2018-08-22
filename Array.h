#pragma once

// opencv
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

// c++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

//DIPro
#include "Color.h"

#define EPSION 0.001


template <class T>
class Array
{
public:
	static Array* GetInstance()
	{
		if (arr == NULL)
		{
			arr = new Array();
		}
		return arr;
	}

	static void DestoryInstance()
	{
		if (arr != NULL)
		{
			delete arr;
			arr = NULL;
		}
	}

	void ArrInit(T* Arr, int width, int height);
	void ArrayCopy(const T* SrcArr, T* DstArr, int width, int height);

	void ArrayRotate(T* SrcArr, T* DstArr, int width, int height, int angle);

	void ArrayAdd(T* Arr1, T* Arr2, T* Arr3, int width, int height);
	void ArraySub(T* Arr1, T* Arr2, T* Arr3, int width, int height);
	void ArrayMulti(T* Arr1, T* Arr2, double k, int width, int height);

	void ArrayMIN(const T* Arr, T& MIN, int width, int height);
	void ArrayMAX(const T* Arr, T& MAX, int width, int height);

	void ArrayT2Gray(const T* SrcArr, Gray* DstArr, int Len, Gray K = 255);
	void ArrayGray2T(const Gray* SrcArr, T* DstArr, int width, int height);

private:
	static Array* arr;
};

template<class T>
Array<T>* Array<T>::arr = NULL;

template<class T>
void Array<T>::ArrInit(T* Arr, int width, int height)
{
	for (int i = 0; i < width * height; ++i)
	{
		Arr[i] = 0;
	}
}

template<class T>
void Array<T>::ArrayCopy(const T* SrcArr, T* DstArr, int width, int height)
{
	for (int i = 0; i < width * height; ++i)
	{
		DstArr[i] = SrcArr[i];
	}
}

template<class T>
void Array<T>::ArrayRotate(T* SrcArr, T* DstArr, int width, int height, int angle)
{
	assert(width == height);
	assert(angle % 90 == 0);
	ArrayCopy(SrcArr, DstArr, width, height);

	T* Temp = new T[width * height];

	for (int k = 0; k < angle / 90; k++)
	{
		ArrayCopy(DstArr, Temp, width, height);
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				DstArr[j * width + i] = Temp[(height - 1 - i) * width + j];
			}
		}
	}
}

template<class T>
void Array<T>::ArrayAdd(T* Arr1, T* Arr2, T* Arr3, int width, int height)
{
	T* Temp1 = new T[width * height];
	T* Temp2 = new T[width * height];
	ArrayCopy(Arr1, Temp1, width, height);
	ArrayCopy(Arr2, Temp2, width, height);

	for (int i = 0; i < width * height; ++i)
	{
		Arr3[i] = Temp1[i] + Temp2[i] > 255 ? 255 : Temp1[i] + Temp2[i];
		if (Arr3[i] < 0)
		{
			Arr3[i] = 0;
		}
	}
}

template<class T>
void Array<T>::ArraySub(T* Arr1, T* Arr2, T* Arr3, int width, int height)
{
	T* Temp1 = new T[width * height];
	T* Temp2 = new T[width * height];
	ArrayCopy(Arr1, Temp1, width, height);
	ArrayCopy(Arr2, Temp2, width, height);

	for (int i = 0; i < width * height; ++i)
	{
		Arr3[i] = Temp1[i] - Temp2[i] < 0 ? 0 : Temp1[i] - Temp2[i];
		if (Arr3[i] > 255)
		{
			Arr3[i] = 255;
		}
	}
}

template<class T>
void Array<T>::ArrayMulti(T* Arr1, T* Arr2, double k, int width, int height)
{
	for (int i = 0; i < width * height; ++i)
	{
		Arr2[i] = Arr1[i] * k > 255 ? 255 : Arr1[i] * k;
	}
}

template<class T>
void Array<T>::ArrayMIN(const T* Arr, T& MIN, int width, int height)
{
	MIN = 255;
	for (int i = 0; i < width * height; ++i)
	{
		if (Arr[i] < MIN)
		{
			MIN = Arr[i];
		}
	}
}

template<class T>
void Array<T>::ArrayMAX(const T* Arr, T& MAX, int width, int height)
{
	MAX = 0;
	for (int i = 0; i < width * height; ++i)
	{
		if (Arr[i] > MAX)
		{
			MAX = Arr[i];
		}
	}
}

template<class T>
void Array<T>::ArrayT2Gray(const T* SrcArr, Gray* DstArr, int Len, Gray K = 255)
{
	T* Temp = new T[Len];

	T SrcMin = 255;
	for (int i = 0; i < Len; ++i)
	{
		if (SrcArr[i] < SrcMin)
		{
			SrcMin = SrcArr[i];
		}
	}

	for (int i = 0; i < Len; ++i)
	{
		Temp[i] = SrcArr[i] - SrcMin;
	}

	T TempMax = 0;
	for (int i = 0; i < Len; ++i)
	{
		if (Temp[i] > TempMax)
		{
			TempMax = Temp[i];
		}
	}

	for (int i = 0; i < Len; ++i)
	{
		DstArr[i] = (Gray)Round(K * (Temp[i] / (TempMax + EPSION)));
	}
}

template<class T>
void Array<T>::ArrayGray2T(const Gray* SrcArr, T* DstArr, int width, int height)
{
	for (int i = 0; i < width * height; ++i)
	{
		DstArr[i] = (T)SrcArr[i];
	}
}