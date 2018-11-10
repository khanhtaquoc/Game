#include "Torch.h"

void CTorch::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CTorch::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH;
	b = y + BBOX_HEIGHT;
}
