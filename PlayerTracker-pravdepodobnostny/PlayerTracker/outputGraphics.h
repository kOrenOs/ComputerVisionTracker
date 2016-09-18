#include <opencv2/core/mat.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "player.h"

using namespace cv;

class outputGraphics
{
	char* playgroundImagePath;
	char* imageFolder;
	int sizeXWindow;
	int sizeYWindow;
	int discColumns;
	int discRows;
	Scalar color;

public:
	outputGraphics(char* paPlaygroundImage, char* paImageFolder, int sizeX, int sizeY, int paDiscColumns, int paDiscRows, Scalar paColor)
	{
		playgroundImagePath = paPlaygroundImage;
		imageFolder = paImageFolder;
		sizeXWindow = sizeX;
		sizeYWindow = sizeY;
		discColumns = paDiscColumns;
		discRows = paDiscRows;
		color = paColor;
	}

	void paintDiscretePlaygroundPoints(std::vector<Point> discretePoints)
	{
		Mat imagePosition = imread(playgroundImagePath);
		resize(imagePosition, imagePosition, cv::Size(sizeXWindow, sizeYWindow));

		for (int i = 0; i < sizeXWindow; i += sizeXWindow / discColumns)
		{
			line(imagePosition, Point(i, 0), Point(i, sizeYWindow), Scalar(0, 0, 0), 1);
		}
		for (int i = 0; i < sizeYWindow; i += sizeYWindow / discRows)
		{
			line(imagePosition, Point(0, i), Point(sizeXWindow, i), Scalar(0, 0, 0), 1);
		}

		for (int i = 0; i < discretePoints.size(); i++){
			circle(imagePosition, Point(discretePoints[i].x * (sizeXWindow / discColumns) + sizeXWindow / (discColumns * 2),
				discretePoints[i].y * (sizeYWindow / discRows) + sizeYWindow / (discRows * 2)), 1, color, 3);
		}

		imshow("possition", imagePosition);
		waitKey(30);
	}

	void paintIdentityPoints(std::vector<player*> palyers)
	{
		Mat imagePosition = imread(playgroundImagePath);
		resize(imagePosition, imagePosition, cv::Size(sizeXWindow, sizeYWindow));

		for (int i = 0; i < sizeXWindow; i += sizeXWindow / discColumns)
		{
			line(imagePosition, Point(i, 0), Point(i, sizeYWindow), Scalar(0, 0, 0), 1);
		}
		for (int i = 0; i < sizeYWindow; i += sizeYWindow / discRows)
		{
			line(imagePosition, Point(0, i), Point(sizeXWindow, i), Scalar(0, 0, 0), 1);
		}

		for (int i = 0; i < palyers.size(); i++){
			putText(imagePosition, to_string(i), Point(palyers[i]->getDiscX() * (sizeXWindow / discColumns),
				palyers[i]->getDiscY() * (sizeYWindow / discRows)), FONT_HERSHEY_SIMPLEX, 0.35, Scalar(82, 233, 245));
		}

		imshow("possition", imagePosition);
		waitKey(30);
	}
	
	void paintRealImage(const char* image)
	{
		Mat imageReal = imread(image);
		resize(imageReal, imageReal, cv::Size(sizeXWindow, sizeYWindow));
		imshow("real picture", imageReal);
		waitKey(30);
	}

	/*void paintTrackedMap(tracker paTracker)
	{
		
	}*/
};
