#include "TileMap.h"
#include "Textures.h"
#include "Game.h"
#include "debug.h"
#include <fstream>
using namespace std;


void CTileMap::LoadTileMap(LPDIRECT3DTEXTURE9 texTile)
{
	
	int n = widthTexTile / TILE_WIDTH;

	for (int i = 0; i < n; i++)
	{
		int left = i * TILE_WIDTH;
		int top = 0;
		sprites->Add(i, left, 0, left + 32, TILE_HEIGHT, texTile);
	}
}

void CTileMap::Render()
{
	map = new int*[column];
	for (int i = 0; i < column; i++)
		map[i] = new int[row];

	ifstream file{ "textures\\Level1Matrix.txt" };
	if (!file.is_open()) DebugOut(L"Can't read file txt");
	{
		for (int j = 0; j < row; j++)
		{
			for (int i = 0; i < column; i++)
			{
				file >> map[i][j];
			}
		}
	}
	file.close();

	for (int j = 0; j < row; j++)
	{
		for (int i = 0; i < column; i++)
		{
			sprites->Get(map[i][j])->Draw(i * 32, j * 32);
		}
	}
	
}

