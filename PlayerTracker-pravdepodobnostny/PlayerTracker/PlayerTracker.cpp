// PlayerTracker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DataParser.h"
#include "outputGraphics.h"
#include "tracker.h"
#include "matrix.h"

class playerTracker
{
	vector<player*> players;

	int initXSizeOfModel = 425;
	int initYSizeOfModel = 266;
	
	int discterisedModelXSize = 100;
	int discterisedModelYSize = 60;

	char* initXMLFile = "Complete.xml";

	char* realPicturePath = "E:\\AnalyzaObrazu\\";

	int IDCount = 0;

	outputGraphics* graphics;
	tracker tracker;

	matrix mapMatrix = matrix(discterisedModelXSize, discterisedModelYSize);

public:
	playerTracker()
	{
		graphics = new outputGraphics("playgroundModel.jpg", NULL, initXSizeOfModel * 2, initYSizeOfModel * 2, 100, 64, Scalar(255, 0, 0));
		dataParser parser = dataParser(initXSizeOfModel, initYSizeOfModel, initXMLFile, true);

		vector<pointsInterval> parsedInformation = parser.getNextPicture();
		vector<Point> avgPoints = mapPlayers(parsedInformation);
		vector<Point> discretisedPoints = discretisePlayers(avgPoints);
		initPlayers(discretisedPoints, avgPoints);
		graphicsOutput(discretisedPoints, parser.getAactualImageName());

		while (parser.hasNext()){
			parsedInformation = parser.getNextPicture();
			avgPoints = mapPlayers(parsedInformation);
			discretisedPoints = discretisePlayers(avgPoints);
			stepOn(discretisedPoints, avgPoints, parser.getAactualImageName(),49);
			graphicsOutput(discretisedPoints, parser.getAactualImageName());
			cin.get();
		}
	}

private:
	vector<Point> mapPlayers(vector<pointsInterval> playersInfo)
	{
		ObjectMapper mapper("HomographyEstimate.txt");
		vector<Point> mapped;
		Point left;
		Point right;

		for (int i = 0; i < playersInfo.size(); i++){
			left = mapper.toMap(playersInfo[i].getLeft());
			right = mapper.toMap(playersInfo[i].getRight());

			mapped.push_back(avgPoints(left, right));
		}
		return mapped;
	}

	vector<Point> discretisePlayers(vector<Point> playersInfo)
	{
		vector<Point> mapped;
		for (int i = 0; i < playersInfo.size(); i++){

			mapped.push_back(discretisePoints(playersInfo[i]));
		}
		return mapped;
	}

	Point avgPoints(Point pointA, Point pointB)
	{
		return Point((pointA.x + pointB.x) / 2, (pointA.y + pointB.y) / 2);
	}

	Point discretisePoints(Point pointA)
	{
		return Point(floor(pointA.x / (initXSizeOfModel*1.0 / discterisedModelXSize)), 
			floor(pointA.y / (initYSizeOfModel*1.0 / discterisedModelYSize)));
	}

	void initPlayers(vector<Point> discretisePoints, vector<Point> avgPoints)
	{
		for (size_t i = 0; i < discretisePoints.size(); i++)
		{
			player* temp = new player(discretisePoints[i], avgPoints[i]);
			temp->setID(IDCount, Point(temp->getDiscX(), temp->getDiscY()));
			IDCount++;
			players.push_back(temp);
		}
		tracker.addTimeVector(players);
	}

	void stepOn(vector<Point> discretisePoints, vector<Point> avgPoints, const char* imageName, int threshold)
	{
		mapMatrix.resetMatrix();
		vector<player*> newPlayers;
		for (int i = 0; i < discretisePoints.size(); i++)
		{
			player* temp = new player(discretisePoints[i], avgPoints[i]);
			if (discretisePoints[i].x >= threshold){
				temp->setID(IDCount, Point(temp->getDiscX(), temp->getDiscY()));
				IDCount++;
			}
			newPlayers.push_back(temp);
			mapMatrix.addPlayer(temp, i);
		}

		player* temp= NULL;
		vector<player*> candidates;
		vector<playerIdentityVector> posibilities;
		for (int i = 0; i < players.size(); i++){
			temp = players[i];
			posibilities = temp->getPositions();
			for (int j = 0; j < posibilities.size(); j++){
				candidates = getPossiblePlayers(posibilities[j].getPoint(), 2, newPlayers);
				for (int k = 0; k< candidates.size(); k++){
					candidates[k]->setID(posibilities[j].getID(), calculateFuturePoint(
						Point(candidates[k]->getDiscX(), candidates[k]->getDiscY()), 
						Point(temp->getDiscX(), temp->getDiscY())));
				}
			}
		}
		players = newPlayers;
		tracker.addTimeVector(players);
		writeOutPlayersPosib(imageName);
	}

	vector<player*> getPossiblePlayers(Point expectedPosition, int distance, vector<player*> newPlayers)
	{
		int x = expectedPosition.x;
		int xBoundMin;
		int xBoundMax;
		if (x - distance > 0)
		{
			xBoundMin = x - distance;
		}
		else
		{
			xBoundMin = 0;
			xBoundMax = x + distance;
		}
		if (x + distance < discterisedModelXSize)
		{
			xBoundMax = x + distance;
		}
		else
		{
			xBoundMax = discterisedModelXSize-1;
		}
		int y = expectedPosition.y;
		int yBoundMin;
		int yBoundMax;
		if (y - distance > 0)
		{
			yBoundMin = y - distance;
		}
		else
		{
			yBoundMin = 0;
			yBoundMax = y + distance;
		}
		if (y + distance < discterisedModelYSize)
		{
			yBoundMax = y + distance;
		}
		else
		{
			yBoundMax = discterisedModelYSize-1;
		}

		vector<player*> output;
		
		for (int i = xBoundMin; i <= xBoundMax; i++)
		{
			for (int j = yBoundMin; j <= yBoundMax; j++)
			{
				if (mapMatrix.getPlayer(i, j) != -1){
					output.push_back(newPlayers[mapMatrix.getPlayer(i, j)]);
				}
			}
		}
		
		return output;
	}

	Point calculateFuturePoint(Point actual, Point last)
	{
		return Point(actual.x - (last.x - actual.x), actual.y - (last.y - actual.y));
	}

	void graphicsOutput(vector<Point> discretisePoints, const char* image)
	{
		char result[200];
		graphics->paintDiscretePlaygroundPoints(discretisePoints);
		graphics->paintIdentityPoints(players);
		strcpy_s(result, realPicturePath);
		strcat_s(result, image);
		graphics->paintRealImage(result);
	}

	void writeOutPlayersPosib(const char* imageName)
	{
		char* temp;
		cout << imageName << endl;
		for (int i = 0; i < players.size(); i++){
			cout << "Index " << i << " [" << players[i]->getDiscX() << "," << players[i]->getDiscY() << "] " << ": ";
			players[i]->writeOutPossibilities();
			cout << endl;
		}
		cout << endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	playerTracker tracker;
}



