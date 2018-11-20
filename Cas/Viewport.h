#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "Simon.h"
#include "debug.h"

class CViewport
{
	D3DXVECTOR3 _positionWorld;
	float _width;
	float _height;
	D3DXVECTOR3 posViewport;
public:
	CViewport();
	CViewport(float x, float y);
	CViewport(float x, float y, float width, float height);
	~CViewport();

	void setPositionWorld(D3DXVECTOR3 position);
	D3DXVECTOR3 getPositionWorld();

	float getWidth();
	float getHeight();

	D3DXVECTOR3 getPositionInViewport(D3DXVECTOR3* position);
	
	bool isContains(const RECT &rect);


	// Lấy kích thước của viewport bằng HCN, tinhs trong world lớn
	RECT getBounding();


};

