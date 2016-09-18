#include "stdafx.h"
#include "player.h"

using namespace std;

class matrix
{
	player **playerMatrix;
	int rows;
	int columns;

public:
	matrix()
	{
	}
	matrix(int paRows, int paColumns)
	{
		rows = paRows;
		columns = paColumns;

		playerMatrix = new player*[rows];
		for (int i = 0; i < rows; i++)
		{
			playerMatrix[i] = new player[columns];
		}
	}

	bool addPlayer(player paPlayer)
	{
		if (playerMatrix[paPlayer.getX()][paPlayer.getY()].getX() < 0)
		{
			playerMatrix[paPlayer.getX()][paPlayer.getY()] = paPlayer;
			return true;
		}
		return false;
	}

	void resetMatrix()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if (playerMatrix[i][j].getX() > 0){
					playerMatrix[i][j] = player();
				}
			}
		}
	}

	player* getPlayer(int i, int j)
	{
		return &playerMatrix[i][j];
	}

	void newMatrix(matrix *incomeMatrix)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				playerMatrix[i][j] = *incomeMatrix->getPlayer(i,j);
			}
		}
	}
};