#include "Box2.h"

void CBox2::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CBox2::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH;
	b = y + BBOX_HEIGHT;
}
