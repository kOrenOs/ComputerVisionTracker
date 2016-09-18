#include "stdafx.h";

class view
{
	int XL;
	int YL;
	int XR;
	int YR;

public:
	view(int paXL, int paYL, int paXR, int paYR)
	{
		XL = paXL;
		XR = paXR;
		YL = paYL;
		YR = paYR;
	}

	int getXL()
	{
		return XL;
	}
	int getYL()
	{
		return YL;
	}
	int getXR()
	{
		return XR;
	}
	int getYR()
	{
		return YR;
	}
};