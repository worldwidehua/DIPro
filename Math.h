#pragma once

#define PI 3.1415926
#define EPSION 0.001

struct Point
{
	int x;
	int y;
};

struct Complex
{
	double Real;
	double Imagin;
};

int Round(double d)
{
	double p = d - (int)d;
	if (p >= 0.5)
	{
		return (int)std::ceil(d);
	}
	else
	{
		return (int)std::floor(d);
	}
	return -1;
}

void ComplexAdd(Complex* Comp1, Complex* Comp2, Complex* DstComp)
{
	DstComp->Real = Comp1->Real + Comp2->Real;
	DstComp->Imagin = Comp1->Imagin + Comp2->Imagin;
}

void ComplexSub(Complex* Comp1, Complex* Comp2, Complex* DstComp)
{
	DstComp->Real = Comp1->Real - Comp2->Real;
	DstComp->Imagin = Comp1->Imagin - Comp2->Imagin;
}

void ComplexMulty(Complex* Comp1, Complex* Comp2, Complex* DstComp)
{
	DstComp->Real = Comp1->Real * Comp2->Real - Comp1->Imagin * Comp2->Imagin;
	DstComp->Imagin = Comp1->Real * Comp2->Imagin + Comp1->Imagin * Comp2->Real;
}

void ComplexCopy(const Complex* Comp1, Complex* Comp2)
{
	Comp2->Real = Comp1->Real;
	Comp2->Imagin = Comp1->Imagin;
}