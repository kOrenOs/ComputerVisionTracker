#include "stdafx.h"

using namespace std;

class matrix
{
	int **playerMatrix;
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

		playerMatrix = new int*[rows];
		for (int i = 0; i < rows; i++)
		{
			playerMatrix[i] = new int[columns];
		}
		resetMatrix();
	}

	bool addPlayer(player* paPlayer, int index)
	{
		if (playerMatrix[paPlayer->getDiscX()][paPlayer->getDiscY()] == -1)
		{
			playerMatrix[paPlayer->getDiscX()][paPlayer->getDiscY()] = index;
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
				if (playerMatrix[i][j] != -1){
					playerMatrix[i][j] = -1;
				}
			}
		}
	}

	int getPlayer(int i, int j)
	{
		return playerMatrix[i][j];
	}
};