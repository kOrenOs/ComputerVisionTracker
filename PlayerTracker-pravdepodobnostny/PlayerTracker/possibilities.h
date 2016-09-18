#include "playerIdentityVector.h"

class possibilities
{
	//vector<double> chance;
	std::vector<playerIdentityVector> points;
	int count = 0;

public:
	possibilities(int paID, cv::Point paPrediction)
	{
		addCandidate(paID, paPrediction);
	}

	void addCandidate(int paID, cv::Point paPrediction)
	{
		points.push_back(playerIdentityVector(paID, paPrediction));
	}

	std::vector<playerIdentityVector> getPositions()
	{
		return points;
	}

	/*vector<int> getID()
	{
		map<int, int> IDcounts;
		for (int i = 0; i < ID.size(); i++)
		{
			if (IDcounts.find(ID[i]) != IDcounts.end())
			{
				IDcounts[ID[i]]++;
			} else
			{
				IDcounts[ID[i]] = 1;
			}
		}

		vector<int> allIDs;
		for (map<int, int>::iterator it = IDcounts.begin(); it != IDcounts.end(); ++it) {
			allIDs.push_back(it->first);
		}

		return allIDs;
	}

	vector<double> getPossibilities()
	{
		map<int, int> IDcounts;
		for (int i = 0; i < ID.size(); i++)
		{
			if (IDcounts.find(ID[i]) != IDcounts.end())
			{
				IDcounts[ID[i]]++;
			}
			else
			{
				IDcounts[ID[i]] = 1;
			}
		}

		vector<double> allIDs;
		for (map<int, int>::iterator it = IDcounts.begin(); it != IDcounts.end(); ++it) {
			allIDs.push_back(it->second*(1.0*count));
		}

		return allIDs;
	}*/
};
