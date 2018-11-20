#pragma once
#include "Sprites.h"
using namespace std;

#define ID_TEX_TILE 5
#define TILE_WIDTH 32
#define TILE_HEIGHT	32

class CTileMap
{
	int width = column * TILE_WIDTH;
	int height = row * TILE_HEIGHT;

	int **map;
	int row;
	int column;
	int widthTexTile = 2720;

	CSprites *sprites = CSprites::GetInstance();

	
public:
	void GetSize(float &Width, float &Height) { Width = width; Height = height; }
	void SetColRow(float col, float row) { this->row = row; this->column = col; }
	void SetwidthTexTile(float widthTexTile) { this->widthTexTile = widthTexTile;}

	void LoadTileMap(LPDIRECT3DTEXTURE9 texTile);
	void Render();
};

