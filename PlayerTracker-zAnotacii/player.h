#include "playerType.h"

class player
{
protected:
	PlayerType type;
	vector<player> playersInside;
	vector<player*> playersCandidate;
	int count = 0;
	int ID = -1;
	cv::Point actualPossition = cv::Point(-1, -1);
	cv::Point nextExpectedPossition = cv::Point(-1, -1);
	cv::Point realPossition = cv::Point(-1, -1);

public:
	player()
	{
	}

	player(int paXL, int paYL, int paXR, int paYR, int realX, int realY)
	{
		actualPossition.x = round((paXL + paXR) / 2.0);
		actualPossition.y = round((paYL + paYR) / 2.0);
		count++;
		nextExpectedPossition = actualPossition;
		realPossition.x = realX;
		realPossition.y = realY;
	}

	player(int paXL, int paYL, int paXR, int paYR, PlayerType paType, int paID, int realX, int realY)
	{
		type = paType;
		ID = paID;
		actualPossition.x = round((paXL + paXR) / 2.0);
		actualPossition.y = round((paYL + paYR) / 2.0);
		nextExpectedPossition = actualPossition;
		count++;
		realPossition.x = realX;
		realPossition.y = realY;
	}

	player(int paXL, int paYL, int paXR, int paYR, PlayerType paType, int paID, cv::Point oldPossition, int realX, int realY)
	{
		type = paType;
		ID = paID;
		actualPossition.x = round((paXL + paXR) / 2.0);
		actualPossition.y = round((paYL + paYR) / 2.0);
		count++;
		setNextPossition(oldPossition);
		realPossition.x = realX;
		realPossition.y = realY;
	}

	void addCandidate(vector<player*> newCandidate)
	{
		for (int i = 0; i < newCandidate.size(); i++){
			playersCandidate.push_back(newCandidate[i]);
		}
	}
	void resetCandidates()
	{
		playersCandidate.clear();
	}
	int candidatesCount()
	{
		return playersCandidate.size();
	}
	player* getCandidate(int possition)
	{
		return playersCandidate[possition];
	}
	void removeCandidate(int possition)
	{
		playersCandidate.erase(playersCandidate.begin() + possition);
	}
	void chackPlayerCandidate()
	{
		vector<int> indexes;
		for (int i = 0; i < playersCandidate.size(); i++)
		{
			if (playersCandidate[i]->getID() != -1)
			{
				indexes.push_back(i);
			}
		}
		for (int i = indexes.size() - 1; i >= 0; i--)
		{
			playersCandidate.erase(playersCandidate.begin() + indexes[i]);
		}
	}
	player* getBestCandidate()
	{
		double distance = 10000;
		double temp = 0;
		player init = player();
		player *best = &init;
		for (int i = 0; i < playersCandidate.size(); i++)
		{
			if (playersCandidate[i]->getID() == -1){
				temp = sqrt(pow((nextExpectedPossition.x - playersCandidate[i]->getX()), 2) + pow((nextExpectedPossition.y - playersCandidate[i]->getY()), 2));
				if (temp < distance)
				{
					best = playersCandidate[i];
					distance = temp;
				}
			}
		}
		return best;
	}
	cv::Point getNextPossition()
	{
		return nextExpectedPossition;
	}
	int getX()
	{
		return actualPossition.x;
	}
	int getY()
	{
		return actualPossition.y;
	}
	cv::Point getPossition()
	{
		return actualPossition;
	}
	int getID()
	{
		return ID;
	}
	vector<player> getPlayers()
	{
		return playersInside;
	}
	PlayerType getType()
	{
		return type;
	}
	void setType(PlayerType paType)
	{
		type = paType;
	}
	void setID(int paID)
	{
		ID = paID;
	}
	void setNextPossition(cv::Point oldPossition)
	{
		nextExpectedPossition.x = actualPossition.x - (oldPossition.x - actualPossition.x);
		nextExpectedPossition.y = actualPossition.y - (oldPossition.y - actualPossition.y);
	}
	int getCount()
	{
		return count;
	}
	bool addPlayer(PlayerType paType, int ID, cv::Point oldPossition, int realX, int realY)
	{
		if (type == group)
		{
			vector<player>::iterator iterator;
			iterator = playersInside.begin();
			playersInside.insert(iterator, player(actualPossition.x, actualPossition.y, actualPossition.x, actualPossition.y, paType, ID, oldPossition, realX, realY));
			count++;
			return true;
		}
		return false;
	}
	int getRealX()
	{
		return realPossition.x;
	}
	int getRealY()
	{
		return realPossition.y;
	}
};
