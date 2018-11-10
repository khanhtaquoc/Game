#include <algorithm>
#include "debug.h"
#include "Simon.h"
#include "Game.h"
#include "Morningstar.h"

CSimon::CSimon()
{
	level = SIMON_LEVEL_1;
	untouchable = 0;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;	
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


	// Time Attacking
	if (GetTickCount() - attacking_start > attacking_end)
	{
		isAttacking = false;
	}
	
		
}


void CSimon::Render()
{
	int ani;
	
	if (state == SIMON_STATE_DIE)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_ACTION_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_ACTION_LEFT;
		}
	}
	else
	{
		if (vx == 0)
		{
			 if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
			else ani = SIMON_ANI_IDLE_LEFT;	
		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
			else ani = SIMON_ANI_WALKING_LEFT;
	}

	if (vy < 0)
	{
		if (nx > 0) ani = SIMON_ANI_JUMP_RIGHT;
		else ani = SIMON_ANI_JUMP_LEFT;
	}

	if (state == SIMON_STATE_DOWN)
	{
		if (nx > 0) ani = SIMON_ANI_DOWN_RIGHT;
		else ani = SIMON_ANI_DOWN_LEFT;
	}

	if (isAttacking == true)
	{
		if (nx > 0) ani = SIMON_ANI_ACTION_RIGHT;
		else ani = SIMON_ANI_ACTION_LEFT;
	}


	

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:	
		vy = -SIMON_JUMP_SPEED_Y;
		attacking_end = 240;
		break;
	case SIMON_STATE_DOWN:
		vx = 0;
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_ACTION:
		isAttacking = true;
		attacking_end = SIMON_ATTACKING_TIME;
		attacking_start = GetTickCount();
		vx = 0;
		break;
	}
}

CSimon * CSimon::__instance = NULL;


CSimon * CSimon::GetInstance()
{
	if (__instance == NULL) __instance = new CSimon();
	return __instance;
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}







