#pragma once
#include "GameObject.h"
#include "Morningstar.h"

#define SIMON_WALKING_SPEED		0.1f 
#define SIMON_JUMP_SPEED_Y		0.4f
#define SIMON_JUMP_DEFLECT_SPEED 0.1f
#define SIMON_GRAVITY			0.001f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			400
#define SIMON_STATE_ACTION			600
#define SIMON_STATE_DIE				800
#define SIMON_STATE_DOWN			1

#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1
#define SIMON_ANI_WALKING_RIGHT		2
#define SIMON_ANI_WALKING_LEFT		3
#define SIMON_ANI_JUMP_RIGHT		4
#define SIMON_ANI_JUMP_LEFT			5
#define SIMON_ANI_ACTION_RIGHT		6
#define SIMON_ANI_ACTION_LEFT		7
#define SIMON_ANI_DIE_RIGHT			8
#define SIMON_ANI_DIE_LEFT			9
#define SIMON_ANI_DOWN_RIGHT		10
#define SIMON_ANI_DOWN_LEFT			11

#define	SIMON_LEVEL_1		11
#define	SIMON_LEVEL_2		21
#define	SIMON_LEVEL_3		31

#define SIMON_BBOX_WIDTH  40	
#define SIMON_BBOX_HEIGHT 65

#define SIMON_UNTOUCHABLE_TIME 50000
#define SIMON_ATTACKING_TIME 360



class CSimon : public CGameObject
{
	static CSimon * __instance;
	int level;
	int untouchable;
	int attacking_start;
	int attacking_end;
	DWORD untouchable_start;
	bool isAttacking;
public:
	CSimon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	
	static CSimon * GetInstance();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual bool GetisAttacking() { return isAttacking; }
};

