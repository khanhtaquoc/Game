#pragma once
#include "GameObject.h"
#include "Simon.h"

#define MORN_STATE_ACTION			0

#define MORN_ANI_ACTION_RIGHT		0
#define MORN_ANI_ACTION_LEFT		1

#define	MORN_LEVEL_1		11
#define	MORN_LEVEL_2		21
#define	MORN_LEVEL_3		31

#define MORN_BBOX_WIDTH  75	
#define MORN_BBOX_HEIGHT 60

#define MORN_UNTOUCHABLE_TIME 5000



class CMorningstar : public CGameObject
{
	
	int level;
	int untouchable;
	int attacking_start;
	int attacking_end;
	DWORD untouchable_start;
	bool isAttacking;
	
public:
	
	CMorningstar() : CGameObject()
	{
		level = MORN_LEVEL_1;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

