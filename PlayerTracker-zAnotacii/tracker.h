#include <vector>
#include <map>
#include <opencv2/core/mat.hpp>

using namespace std;
class tracker
{
private:
	map<int, vector<cv::Point>*> tracks;
	map<int, cv::Scalar> colours;

public:
	void addValue(cv::Point point, int ID, cv::Scalar colour)
	{
		if (ID == -1)
		{
			return;
		}

		vector<cv::Point> *temp;
		if (tracks.find(ID) != tracks.end()) {
			temp = tracks.find(ID)->second;
			temp->push_back(point);
		} else
		{
			temp = new vector<cv::Point>;
			temp->push_back(point);
			tracks.insert({ ID, temp });
			colours.insert({ ID, colour });
		}
	}

	vector<cv::Point>* getTrack(int ID)
	{
		if (tracks.find(ID) != tracks.end()) {
			return tracks.find(ID)->second;
		}
		return NULL;
	}

	cv::Scalar getColour(int ID)
	{
		if (colours.find(ID) != colours.end()) {
			return colours.find(ID)->second;
		}
		return NULL;
	}
};