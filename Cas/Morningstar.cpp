#include "Morningstar.h"
#include <algorithm>
#include "debug.h"
#include "Simon.h"
#include "Game.h"

void CMorningstar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSimon *simon;
	simon = CSimon::GetInstance();
	
	isAttacking = simon->GetisAttacking();
	nx = simon->nx;
	if (nx > 0)
	{
		x = simon->x - 24;
		y = simon->y + 14;
	}
	else if (nx < 0)
	{
		x = simon->x - 52;
		y = simon->y + 14;
	}
	SetPosition(x , y);

	// Time Attacking

	
}

void CMorningstar::Render()
{
	int ani;
	
	if (isAttacking == true)
	{
		if (nx > 0) ani = MORN_ANI_ACTION_RIGHT;
		else ani = MORN_ANI_ACTION_LEFT;

		int alpha = 255;
		if (untouchable) alpha = 128;
		animations[ani]->Render(x, y, alpha);

		RenderBoundingBox();
	}
}

void CMorningstar::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MORN_STATE_ACTION:
		break;
	}
}

void CMorningstar::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	right = x + MORN_BBOX_WIDTH;
	bottom = y + MORN_BBOX_HEIGHT;
}
