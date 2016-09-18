// parse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tinyxml.h"
#include "objectmapper.h"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include "matrix.h"
#include "dirent.h"
#include "tracker.h"

using namespace cv;
using namespace std;

class parse{
	TiXmlElement* root;
	player playerObject[30];
	int count = 0;
	matrix *oldMatrix = NULL;
	matrix *newMatrix = NULL;
	int maxCount = 0;
	tracker tracker;
	Point ball;

public:

	parse(const char* fileName)
	{
		TiXmlDocument doc;
		string* pictures;
		if (!doc.LoadFile(fileName))
		{
			cout << doc.ErrorDesc() << endl;
			cin.get();
			return;
		}

		root = doc.FirstChildElement();
		if (root == NULL)
		{
			cout << "Failed to load file: No root element."
				<< endl;
			doc.Clear();
			cin.get();
			return;
		}

		root = root->FirstChildElement();
		pictures = dirFiles("E:\\AnalyzaObrazu\\Vysledky\\");

		for (int i = 0; i < 200; i++)
		{
			char result[200];
			strcpy_s(result, "E:\\AnalyzaObrazu\\Vysledky\\");
			strcat_s(result, pictures[i].c_str());

			findPicture(pictures[i].c_str());
			compareMatrix();
			paintPossition(result);
			changeMatrix();
			cerr << i << endl;
			Sleep(500);
			//cin.get();
			if (i == 30)
			{
				oldMatrix = NULL;
				newMatrix = NULL;
				maxCount = 0;
			}
		}

		//converteCoordinates();
		cin.get();
	}

private:
	void findPicture(const char* pictureName)
	{
		const char* attr;
		TiXmlElement* elemTemp;
		TiXmlElement* classElem;
		const char* text;
		const char* typBB;
		const char* specifikaciaBB;

		int xSize = 425 * 2;
		int ySize = 266 * 2;

		int XL = 0;
		int YL = 0;
		int XR = 0;
		int YR = 0;

		for (int i = 0; i < 30; i++)
		{
			playerObject[i] = player();
		}

		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			ObjectMapper mapper("HomographyEstimate.txt");
			elemTemp = elem->FirstChildElement();
			//cout << loadValue(elemTemp, NULL);
			text = loadValue(elemTemp, NULL);
			attr = strstr(text, pictureName);
			if (attr != NULL)
			{
				count = 0;
				cout << text;
				elemTemp = elem->FirstChildElement("boundingboxes");
				for (elemTemp = elemTemp->FirstChildElement(); elemTemp != NULL; elemTemp = elemTemp->NextSiblingElement())
				{
					text = loadValue(elemTemp, "x_left_top");
					cout << text;
					cout << endl;

					XL = stoi(text);

					text = loadValue(elemTemp, "y_left_top");
					cout << text;
					cout << endl;

					YL = stoi(text);

					text = loadValue(elemTemp, "width");
					cout << text;
					cout << endl;

					XR = stoi(text) + XL;

					text = loadValue(elemTemp, "height");
					cout << text;
					cout << endl;

					YL += stoi(text);
					YR = YL;


					classElem = elemTemp->FirstChildElement("class")->FirstChildElement();
					typBB = loadValue(classElem, NULL);
					specifikaciaBB = "";
					if (strcmp(typBB, "lopta") == 0)
					{
						Point left = mapper.toMap(Point(XL, YL));
						ball.x = left.x / (xSize / 200);
						ball.y = left.y / (ySize / 128);
						continue;
					}
					if (strcmp(typBB, "hrac") == 0 || strcmp(typBB, "brankar") == 0)
					{
						classElem = classElem->NextSiblingElement();
						specifikaciaBB = loadValue(classElem, NULL);
					}

					Point left = mapper.toMap(Point(XL, YL));
					Point right = mapper.toMap(Point(XR, YR));
					cout << left.x << endl << left.y << endl;
					cout << right.x << endl << right.y << endl;
					cout << endl;

					playerObject[count] = player(left.x / (xSize / 200), left.y / (ySize / 128), right.x / (xSize / 200), right.y / (ySize / 128), left.x, left.y);
					if (strcmp(typBB, "hrac") == 0)
					{
						if (strcmp(specifikaciaBB, "oranzovo-cierny"))
						{
							playerObject[count].setType(playerOrange);
						}
						else
						{
							playerObject[count].setType(playerYellow);
						}
					}
					if (strcmp(typBB, "brankar") == 0)
					{
						if (strcmp(specifikaciaBB, "oranzovo-cierny"))
						{
							playerObject[count].setType(goalkeeperOrange);
						}
						else
						{
							playerObject[count].setType(goalkeeperYellow);
						}
					}
					if (strcmp(typBB, "skupina") == 0)
					{
						playerObject[count].setType(group);
					}
					if (strcmp(typBB, "rozhodca") == 0)
					{
						playerObject[count].setType(referee);
					}
					count++;
				}
			}
		}
	}

	void paintPossition(char* picturePath)
	{
		int xSize = 425 * 2;
		int ySize = 266 * 2;
		Scalar farba = NULL;

		Mat imagePosition = imread("702780_938122019599422_608388132_n.jpg");
		Mat imageIdentity = imread("702780_938122019599422_608388132_n.jpg");
		Mat imageReal = imread(picturePath);
		Mat trackerImage = imread("702780_938122019599422_608388132_n.jpg");

		resize(imagePosition, imagePosition, Size(xSize, ySize));
		resize(imageIdentity, imageIdentity, Size(xSize, ySize));
		resize(imageReal, imageReal, Size(xSize, ySize));
		resize(trackerImage, imagePosition, Size(xSize, ySize));

		for (int i = 0; i < xSize; i += xSize / 100)
		{
			line(imagePosition, Point(i, 0), Point(i, ySize), Scalar(0, 0, 0), 1);
			line(imageIdentity, Point(i, 0), Point(i, ySize), Scalar(0, 0, 0), 1);
		}
		for (int i = 0; i < ySize; i += ySize / 64)
		{
			line(imagePosition, Point(0, i), Point(xSize, i), Scalar(0, 0, 0), 1);
			line(imageIdentity, Point(0, i), Point(xSize, i), Scalar(0, 0, 0), 1);
		}
		for (int i = 0; i < count; i++)
		{
			farba = getColour(playerObject[i]);
			circle(imagePosition, Point(playerObject[i].getX() * (xSize / 100) + xSize / (100 * 2), playerObject[i].getY() * (ySize / 64) + ySize / (64 * 2)), 1, farba, 3);
			putText(imageIdentity, to_string(playerObject[i].getID()), Point(playerObject[i].getX() * (xSize / 100), (playerObject[i].getY()+1) * (ySize / 64)), FONT_HERSHEY_SIMPLEX, 0.35, Scalar(82,233,245));

			
			if (playerObject[i].getType() == group)
			{
				for (auto &play : playerObject[i].getPlayers()){
					farba = getColour(play);
					tracker.addValue(Point(play.getRealX(), play.getRealY()), play.getID(), farba);
				}
			} else
			{
				farba = getColour(playerObject[i]);
				tracker.addValue(Point(playerObject[i].getRealX(), playerObject[i].getRealY()), playerObject[i].getID(), farba);
			}
		}

		circle(imagePosition, Point(ball.x * (xSize / 100) + xSize / (100 * 2), ball.y * (ySize / 64) + ySize / (64 * 2)), 1, Scalar(255, 255, 255), 3);

		addTracks(&trackerImage);

		imshow("possition", imagePosition);
		imshow("identity", imageIdentity);
		imshow("real picture", imageReal);
		imshow("tracker", trackerImage);

		waitKey(30);
	}

	Scalar getColour(player play)
	{
		switch (play.getType())
		{
		case playerOrange: return Scalar(0, 0, 255);
		case playerYellow: return Scalar(0, 255, 255);
		case goalkeeperOrange: return Scalar(255, 0, 0);
		case goalkeeperYellow: return Scalar(210, 219, 40);
		case referee: return Scalar(0, 0, 0);
		default: return Scalar(255, 0, 255);
		}
	}

	void addTracks(Mat *image)
	{
		vector<Point> *track;
		for (int i = 0; i < 100; i++)
		{
			track = tracker.getTrack(i);
			if (track != NULL){
				for (int j = 1; j < track->size(); j++){
					line(*image, track->at(j - 1), track->at(j), tracker.getColour(i), 1);
				}
			}
		}
	}

	void compareMatrix()
	{
		if (newMatrix == NULL)
		{
			return;
		}

		vector<player*> playersAround;
		vector<player> temp;
		vector<player> lastPlayers;
		double min;
		double distanceResult;
		bool stop = false;

		for (int l = 0; l < 100; l++)
		{
			for (int j = 0; j < 64; j++)
			{
				if (newMatrix->getPlayer(l, j)->getX() >= 0)
				{
					temp.clear();
					if (newMatrix->getPlayer(l, j)->getType() == group)
					{
						temp = newMatrix->getPlayer(l, j)->getPlayers();
					}
					else
					{
						vector<player>::iterator iter;
						iter = temp.begin();
						if (newMatrix->getPlayer(l, j)->getID() == -1)
						{
							newMatrix->getPlayer(l, j)->setID(maxCount);
							maxCount++;
						}
						temp.insert(iter, *newMatrix->getPlayer(l, j));
					}
					for (int i = 0; i < temp.size(); i++)
					{										//tu, vsetky, ktore sa najdu, do kandidatov hraca
						if (temp[i].getX() >= 0)			//mozne ohranicienie ihriska, resp. vstupno vystupne miesta
						{
							lastPlayers.push_back(temp[i]);
							playersAround = getPossiblePlayers(temp[i], 6);
							lastPlayers.back().addCandidate(playersAround);
						}
					}
				}
			}
		}
		//tu sa moze zacat preberat
		player* tempPlayer;
		while (!stop){
			stop = true;
			for (int i = 0; i < lastPlayers.size(); i++)
			{
				if (lastPlayers[i].candidatesCount() == 1)
				{
					tempPlayer = lastPlayers[i].getCandidate(0);
					if (tempPlayer->getID() == -1){
						stop = false;
						if (tempPlayer->getType() == group)
						{
							tempPlayer->addPlayer(lastPlayers[i].getType(), lastPlayers[i].getID(), lastPlayers[i].getPossition(), tempPlayer->getRealX(), tempPlayer->getRealY());
						}
						else{
							tempPlayer->setID(lastPlayers[i].getID());
							tempPlayer->setNextPossition(lastPlayers[i].getPossition());
						}
						lastPlayers[i].removeCandidate(0);
					}
				}
			}
			if (!stop){
				for (int i = 0; i < lastPlayers.size(); i++)
				{
					lastPlayers[i].chackPlayerCandidate();
				}
			}
		}

		for (int i = 0; i < lastPlayers.size(); i++)
		{
			if (lastPlayers[i].candidatesCount() != 0){
				tempPlayer = lastPlayers[i].getBestCandidate();
				if (tempPlayer->getX() >= 0){
					if (tempPlayer->getType() == group)
					{
						tempPlayer->addPlayer(lastPlayers[i].getType(), lastPlayers[i].getID(), lastPlayers[i].getPossition(), tempPlayer->getRealX(), tempPlayer->getRealY());
					}
					else{
						tempPlayer->setID(lastPlayers[i].getID());
						tempPlayer->setNextPossition(lastPlayers[i].getPossition());
					}
				}
			}
		}
		/*for (int l = 0; l < 100; l++)
		{
			for (int j = 0; j < 64; j++)
			{
				if (newMatrix->getPlayer(l, j)->getX() >= 0)
				{
					temp.clear();
					if (newMatrix->getPlayer(l, j)->getType() == group)
					{
						temp = newMatrix->getPlayer(l, j)->getPlayers();
					}
					else
					{
						vector<player>::iterator iter;
						iter = temp.begin();
						if (newMatrix->getPlayer(l, j)->getID() == -1)
						{
							newMatrix->getPlayer(l, j)->setID(maxCount);
							maxCount++;
						}
						temp.insert(iter, *newMatrix->getPlayer(l, j));
					}
					for (int i = 0; i < temp.size(); i++)
					{
						if (temp[i].getX() >= 0)
						{
							if (temp[i].getX() == temp[i].getNextPossition().x && temp[i].getY() == temp[i].getNextPossition().y)
							{
								playersAround = getPossiblePlayers(temp[i], 4);
								if (playersAround.size() == 0)
								{
									playersAround = getPossiblePlayers(temp[i], 5);
								}
							}
							else{
								playersAround = getPossiblePlayers(temp[i], 3);
								if (playersAround.size() == 0)
								{
									playersAround = getPossiblePlayers(temp[i], 4);
								}
							}
							if (playersAround.size() == 0)
							{
								continue;
							}
							if (playersAround.size() == 1)
							{
								if (playersAround[0]->getID() != -1)
								{
									continue;
								}
								if (playersAround[0]->getType() == group)
								{
									playersAround[0]->addPlayer(temp[i].getType(), temp[i].getID(), temp[i].getPossition(), temp[i].getRealX(), temp[i].getRealY());
								}
								else
								{
									playersAround[0]->setID(temp[i].getID());
									playersAround[0]->setNextPossition(temp[i].getPossition());
								}
								continue;
							}

							player* bestPlayer = &player();
							min = 6400;
							for (int k = 0; k < playersAround.size(); k++)
							{
								if (playersAround[k]->getType() == group || playersAround[k]->getID() != -1)
								{
									continue;
								}
								Point nextPossition = temp[i].getNextPossition();
								distanceResult = distanceCalculate(playersAround[k]->getX(), nextPossition.x, playersAround[k]->getY(), nextPossition.y);
								if (distanceResult < min)
								{
									min = distanceResult;
									bestPlayer = playersAround[k];
								}
							}
							if (bestPlayer->getX() > 0)
							{
								bestPlayer->setID(temp[i].getID());
								bestPlayer->setNextPossition(temp[i].getPossition());
							}
						}
					}
				}
			}
		}*/
	}

	vector<player*> getPossiblePlayers(player oldPosition, int distance)
	{
		Point nextPossition = oldPosition.getNextPossition();
		int x = nextPossition.x;
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
		if (x + distance < 99)
		{
			xBoundMax = x + distance;
		}
		else
		{
			xBoundMax = 99;
		}
		int y = nextPossition.y;
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
		if (y + distance < 63)
		{
			yBoundMax = y + distance;
		}
		else
		{
			yBoundMax = 63;
		}

		PlayerType type = oldPosition.getType();
		vector<player*> output = vector<player*>();
		player* temp;

		for (int i = 0; i < count; i++)
		{
			temp = &playerObject[i];
			if (temp->getX() >= 0 && (temp->getType() == type || temp->getType() == group))
			{
				if (temp->getX() > xBoundMin && temp->getX() < xBoundMax && temp->getY() > yBoundMin && temp->getY() < yBoundMax){
					output.push_back(temp);
				}
			}
		}
		return output;
	}

	double distanceCalculate(int X1, int X2, int Y1, int Y2)
	{
		return sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2));
	}

	/*void converteCoordinates()
	{
	ObjectMapper mapper("HomographyEstimate.txt");
	Point temPoint1;
	Point temPoint2;

	for (int i = 0; i <= count; i++)
	{
	temPoint1 = mapper.toMap(Point(playerObject[i].getX(), playerObject[i].getY()));
	for (int j = temPoint1.x; j <= temPoint2.x; j++)
	{
	//appearMatrix[int(j / 100)][int(temPoint1.y / 100)] = true;					//TODO: ZISTIT MAX RESOLUTION A DOROBIT PREPOCET
	int temp1 = int(j);
	int temp2 = int(temPoint1.y);
	cout << int(j) << "  " << int(temPoint1.y) << endl;
	}
	}
	}*/

	const char* loadValue(TiXmlElement* elem, const char* elemName)
	{
		TiXmlElement* elemTemp2;
		TiXmlNode* elemTempNode;
		const char* text;
		if (elemName != NULL)
		{
			elemTemp2 = elem->FirstChildElement(elemName);
		}
		else
		{
			elemTemp2 = elem;
		}
		elemTempNode = elemTemp2->FirstChild();

		text = elemTempNode->Value();
		if (text == NULL){
			cout << "No text here.";
			return NULL;
		}
		return text;
	}

	void changeMatrix()
	{
		/*if (oldMatrix == NULL)
		{
			oldMatrix = new matrix(100, 64);
			markPlayers();
			fillMatrix(oldMatrix);
			return;
		}*/
		if (newMatrix == NULL)
		{
			oldMatrix = new matrix(100, 64);
			newMatrix = new matrix(100, 64);
			markPlayers();
			fillMatrix(newMatrix);
			return;
		}
		oldMatrix->newMatrix(newMatrix);
		newMatrix->resetMatrix();
		
		int i = 0;
		for (int l = 0; l < 100; l++)
		{
			for (int j = 0; j < 64; j++)
			{
				if (newMatrix->getPlayer(l, j)->getX() >= 0)
				{
					i++;
				}
			}
		}
		cerr << i<<endl;

		fillMatrix(newMatrix);

		i = 0;
		for (int l = 0; l < 100; l++)
		{
			for (int j = 0; j < 64; j++)
			{
				if (newMatrix->getPlayer(l, j)->getX() >= 0)
				{
					i++;
				}
			}
		}
		cerr << i << endl;
	}

	void fillMatrix(matrix *insertMatrix)
	{
		for (int i = 0; i < count; i++)
		{
			insertMatrix->addPlayer(playerObject[i]);
		}
	}

	void markPlayers()
	{
		for (int i = 0; i < count; i++, maxCount++)
		{
			if (playerObject[i].getType() == group)
			{
				vector<player> temp = playerObject[i].getPlayers();
				if (temp.size() == 0)
				{
					maxCount--;
				}
				for (int i = 0; i < temp.size(); i++)
				{
					temp[i].setID(maxCount);
					maxCount++;
				}
			}
			else{
				playerObject[i].setID(maxCount);
			}
		}
	}

	string* dirFiles(const char* dirPath)
	{
		string* picturesPath = new string[300];
		int iterator = 0;
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(dirPath)) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL) {
				picturesPath[iterator] = ent->d_name;
				if (picturesPath[iterator].length()>2){
					iterator++;
				}
			}
			closedir(dir);
		}
		else {
			/* could not open directory */
			perror("");
		}
		return picturesPath;
	}
};

int main()
{
	parse* ready = new parse("Complete.xml");
}
