#pragma once
#include "GameObject.h"

#define BBOX_WIDTH  32
#define BBOX_HEIGHT 32

class CBox2 : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};