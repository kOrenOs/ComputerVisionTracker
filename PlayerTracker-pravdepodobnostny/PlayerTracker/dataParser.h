#include "tinyxml.h"
#include "objectmapper.h"
#include "pointsInterval.h"

using namespace cv;

class dataParser
{
	int xModelSixe;
	int yModelSixe;
	bool informationMsg;
	TiXmlElement* root;
	TiXmlElement* mainRoot;
	TiXmlDocument doc;
	const char* actualImageName;

public:
	dataParser(int paXModelSize, int paYModelSize, const char* fileName, bool paInformationMsg)
	{
		xModelSixe = paXModelSize;
		yModelSixe = paYModelSize;

		informationMsg = paInformationMsg;

		if (!doc.LoadFile(fileName))
		{
			msgWriteOut(doc.ErrorDesc(), "");
			cin.get();
			return;
		}
		msgWriteOut("File loaded.", "");

		root = doc.FirstChildElement();
		mainRoot = doc.FirstChildElement();
		if (root == NULL)
		{
			msgWriteOut("Failed to load file: No root element.", "");
			doc.Clear();
			cin.get();
			return;
		}
		msgWriteOut("Root element found.", "");

		mainRoot = root->FirstChildElement();
		root = root->FirstChildElement();
		root = root->FirstChildElement();

		msgWriteOut(loadValue(root, NULL), "");
	}

	vector<pointsInterval> getNextPicture()
	{
		vector<pointsInterval> temp = parsePicture();
		root = root->NextSiblingElement();
		return temp;
	}

	vector<pointsInterval> parsePicture(){
		vector<pointsInterval> points;

		TiXmlElement* elemTemp;
		TiXmlElement* classElem;
		const char* text;

		int XL = 0;
		int YL = 0;
		int XR = 0;
		int YR = 0;

		elemTemp = root->FirstChildElement();
		actualImageName = loadValue(elemTemp, NULL);

		msgWriteOut(actualImageName, "");
		elemTemp = root->FirstChildElement("boundingboxes");
		for (elemTemp = elemTemp->FirstChildElement(); elemTemp != NULL; elemTemp = elemTemp->NextSiblingElement())
		{
			classElem = elemTemp->FirstChildElement("class")->FirstChildElement();
			text = loadValue(classElem, NULL);
			if (strcmp(text, "lopta") == 0)
			{
				continue;
			}
			text = loadValue(elemTemp, "x_left_top");
			msgWriteOut("x_left_top: ", text);

			text = loadValue(elemTemp, "x_left_top");
			msgWriteOut("x_left_top: ", text);

			XL = stoi(text);

			text = loadValue(elemTemp, "y_left_top");
			msgWriteOut("y_left_top: ", text);

			YL = stoi(text);

			text = loadValue(elemTemp, "width");
			msgWriteOut("width: ", text);

			XR = stoi(text) + XL;

			text = loadValue(elemTemp, "height");
			msgWriteOut("height: ", text);

			YR = stoi(text) + YL;;

			cout << endl;

			points.push_back(pointsInterval(Point(XL, YR), Point(XR, YR)));
		}

		return points;
	}

	vector<pointsInterval> findAndParsePicture(const char* pictureName)
	{
		vector<pointsInterval> points;

		const char* attr;
		TiXmlElement* elemTemp;
		const char* text;

		int XL = 0;
		int YL = 0;
		int XR = 0;
		int YR = 0;

		msgWriteOut(loadValue(mainRoot, NULL), "");

		for (TiXmlElement* elem = mainRoot->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			elemTemp = elem->FirstChildElement();
			text = loadValue(elemTemp, NULL);
			attr = strstr(text, pictureName);
			if (attr != NULL)
			{
				msgWriteOut(text, "");
				elemTemp = elem->FirstChildElement("boundingboxes");
				for (elemTemp = elemTemp->FirstChildElement(); elemTemp != NULL; elemTemp = elemTemp->NextSiblingElement())
				{
					text = loadValue(elemTemp, "x_left_top");
					msgWriteOut("x_left_top: ", text);

					XL = stoi(text);

					text = loadValue(elemTemp, "y_left_top");
					msgWriteOut("y_left_top: ", text);

					YL = stoi(text);

					text = loadValue(elemTemp, "width");
					msgWriteOut("width: ", text);

					XR = stoi(text) + XL;

					text = loadValue(elemTemp, "height");
					msgWriteOut("height: ", text);

					YR = stoi(text) + YL;;

					cout << endl;

					points.push_back(pointsInterval(Point(XL, YR), Point(XR, YR)));
				}
			}
		}
		return points;
	}

	bool hasNext()
	{
		if (root != NULL)
		{
			return true;
		}
		return false;
	}

	const char* getAactualImageName()
	{
		return actualImageName;
	}

private:
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
			msgWriteOut("No text here.", NULL);
			return NULL;
		}
		return text;
	}

	void msgWriteOut(const char* msg1, const char* msg2)
	{
		if (informationMsg)
		{
			cout << msg1 << msg2
				<< endl;
		}
	}
};