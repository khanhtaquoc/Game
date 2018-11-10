#pragma once
#include "GameObject.h"

#define BBOX_WIDTH  32
#define BBOX_HEIGHT 64
class CTorch : public CGameObject
{
public:
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

