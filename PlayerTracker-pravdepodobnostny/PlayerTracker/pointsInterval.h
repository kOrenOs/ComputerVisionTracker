#include <opencv2/core/mat.hpp>

class pointsInterval
{
	cv::Point leftPoint;
	cv::Point rightPoint;

public:
	pointsInterval(cv::Point paLeft, cv::Point paRight)
	{
		leftPoint = paLeft;
		rightPoint = paRight;
	}

	cv::Point getLeft()
	{
		return leftPoint;
	}

	cv::Point getRight()
	{
		return rightPoint;
	}
};
