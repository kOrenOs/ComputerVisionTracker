#include <vector>

class tracker
{
	vector<vector<player*>> timeVectors;

public:
	void addTimeVector(vector<player*> timeVector)
	{
		timeVectors.push_back(timeVector);
	}

	vector<player*> getTimeVectorFromEnd(int countBack)
	{
		return timeVectors[timeVectors.size() - 1 - countBack];
	}
};
