#include <opencv2/core/mat.hpp>

class playerIdentityVector
{
	int ID;
	cv::Point prediction;

public:
	playerIdentityVector(int paID, cv::Point paPrediction)
	{
		ID = paID;
		prediction = paPrediction;
	}

	int getID()
	{
		return ID;
	}

	cv::Point getPoint()
	{
		return prediction;
	}
};
