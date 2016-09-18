#include "possibilities.h"
#include <map>

class player
{
	cv::Point discPosition;
	cv::Point avgPosition;
	possibilities* positionPossibilities;

public:
	player(cv::Point discretisedPosition, cv::Point averagePosition)
	{
		discPosition = discretisedPosition;
		avgPosition = averagePosition;
	}

	int getDiscX()
	{
		return discPosition.x;
	}

	int getDiscY()
	{
		return discPosition.y;
	}

	int getAvgX()
	{
		return avgPosition.x;
	}

	int getAvgY()
	{
		return avgPosition.y;
	}

	void setID(int paID, Point futurePosition)
	{
		if (positionPossibilities == NULL)
		{
			positionPossibilities = new possibilities(paID, futurePosition);
		} else
		{
			positionPossibilities->addCandidate(paID, futurePosition);
		}
	}

	std::vector<playerIdentityVector> getPositions()
	{
		if (positionPossibilities != NULL){
			return positionPossibilities->getPositions();
		}
		vector<playerIdentityVector> temp;
		return temp;
	}

	void writeOutPossibilities()
	{
		char IDnumber[32];
		vector<int> posibilieties = joinIDTogether(getPositions());
		for (int i = 0; i < posibilieties.size(); i++){
			_itoa_s(posibilieties[i], IDnumber, 10);
			cout << IDnumber;
			if (i < posibilieties.size() - 1){
				cout << ",";
			}
		}
	}

	vector<int> joinIDTogether(vector<playerIdentityVector> posibilities)
	{
		map<int, int> IDcounts;
		for (int i = 0; i < posibilities.size(); i++)
		{
			if (IDcounts.find(posibilities[i].getID()) != IDcounts.end())
			{
				IDcounts[posibilities[i].getID()]++;
			}
			else
			{
				IDcounts[posibilities[i].getID()] = 1;
			}
		}

		vector<int> allIDs;
		for (map<int, int>::iterator it = IDcounts.begin(); it != IDcounts.end(); ++it) {
			allIDs.push_back(it->first);
		}

		return allIDs;
	}
};
