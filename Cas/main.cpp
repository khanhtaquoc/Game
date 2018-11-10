/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Simon.h"
#include "Box2.h"
#include "Torch.h"
#include "Morningstar.h"


#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define MAX_FRAME_RATE 60

#define ID_TEX_SIMON 0
#define ID_TEX_SIMON_DEATH 2
#define ID_TEX_ACTION 1
#define ID_TEX_TORCH 8
#define ID_TEX_BBOX_2 9


CGame *game;
CSimon *simon;
CMorningstar *morningstar;


vector<LPGAMEOBJECT> objects;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_UP:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_SPACE:
		if (simon->vx == 0)
		{
			morningstar->SetState(MORN_STATE_ACTION);
			simon->SetState(SIMON_STATE_ACTION);	
		}
		break;
	case DIK_Q:
		simon->SetState(SIMON_STATE_DIE);
		break;
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetPosition(50.0f,0.0f);
		simon->SetSpeed(0, 0);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);;
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
			simon->SetState(SIMON_STATE_DOWN);
	else
		simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, L"textures\\simon.png",D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ACTION, L"textures\\morningstar.png", D3DCOLOR_XRGB(255, 0, 255)); 
	textures->Add(ID_TEX_SIMON_DEATH, L"textures\\simondeath.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX_2, L"textures\\ground\\2.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_TORCH, L"textures\\ground\\0.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	// simon
	sprites->Add(10001, 10, 0, 45, 65, texSimon);			// idle left

	sprites->Add(10002, 75, 0, 103, 65, texSimon);			// walk
	sprites->Add(10003, 134, 0, 166, 65, texSimon);
	sprites->Add(10004, 195, 0, 223, 65, texSimon);

	sprites->Add(10005, 250, 0, 286, 48, texSimon);			// jump

	sprites->Add(10006, 311, 0, 345 , 65, texSimon);		// action 
	sprites->Add(10007, 372, 0, 406, 65, texSimon);
	sprites->Add(10008, 432, 0, 466, 65, texSimon);

	sprites->Add(20001, 480, 0, 515, 65, texSimon);			// down

	sprites->Add(10011, 435, 204, 470, 270, texSimon);		// idle right

	sprites->Add(10012, 376, 204, 404, 270, texSimon);		// walk
	sprites->Add(10013, 312, 204, 345, 270, texSimon);
	sprites->Add(10014, 258, 204, 287, 270, texSimon);

	sprites->Add(10015, 193, 204, 230, 255, texSimon);		// jump

	sprites->Add(10016, 135, 204, 169, 270, texSimon);		// action 
	sprites->Add(10017, 75, 204, 109, 270, texSimon);
	sprites->Add(10018, 15, 204, 49, 270, texSimon);

	sprites->Add(20011, 480, 204, 515, 270, texSimon);		// down
	
	//morn
	LPDIRECT3DTEXTURE9 texAction = textures->Get(ID_TEX_ACTION);
	sprites->Add(11006, 43, 14, 155, 70, texAction);		// left
	sprites->Add(11007, 202, 10, 314, 52, texAction);
	sprites->Add(11008, 351, 15, 455, 35, texAction);

	sprites->Add(11016, 362, 229, 397, 280, texAction);		// right
	sprites->Add(11017, 208, 223, 243, 265, texAction);
	sprites->Add(11018, 60, 225, 172, 245, texAction);

	//die
	LPDIRECT3DTEXTURE9 texDeath = textures->Get(ID_TEX_SIMON_DEATH);
	sprites->Add(11110, 0, 0, 64, 68, texDeath);

	//torch
	LPDIRECT3DTEXTURE9 texTorch = textures->Get(ID_TEX_TORCH);
	sprites->Add(80000, 0, 0, 32, 64, texTorch);
	sprites->Add(80001, 32, 0, 64, 64, texTorch);

	LPDIRECT3DTEXTURE9 texBBox = textures->Get(ID_TEX_BBOX_2);
	sprites->Add(20002, 0, 0, 32, 32, texBBox);

	LPANIMATION ani;

	ani = new CAnimation(100);	// 0 idle right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// 1 idle left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(120);	// 2 walk right
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(10004);
	animations->Add(500, ani);

	ani = new CAnimation(120);	// 3 walk left
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(10014);
	animations->Add(501, ani);

	ani = new CAnimation(120);	// 4 jump right
	ani->Add(10005);
	animations->Add(600, ani);

	ani = new CAnimation(120);	// 5 jump left
	ani->Add(10015);
	animations->Add(601, ani);

	ani = new CAnimation(120);	// 7 action right
	ani->Add(10016);
	ani->Add(10017);
	ani->Add(10018);
	animations->Add(700, ani);

	ani = new CAnimation(120);	// 6 action left 
	ani->Add(10006);
	ani->Add(10007);
	ani->Add(10008);
	animations->Add(701, ani);

	ani = new CAnimation(120); // 8 death right
	ani->Add(11110);
	animations->Add(000, ani);

	ani = new CAnimation(120); // 8 death left
	ani->Add(11110);
	animations->Add(000, ani);

	ani = new CAnimation(120);	//  down right
	ani->Add(20001);
	animations->Add(211, ani);

	ani = new CAnimation(120);	//  down left 
	ani->Add(20011);
	animations->Add(201, ani);


	ani = new CAnimation(100);
	ani->Add(20002);
	animations->Add(222, ani); //Box

	ani = new CAnimation(120);	// 1
	ani->Add(11006);
	ani->Add(11007);
	ani->Add(11008);
	animations->Add(720, ani);

	ani = new CAnimation(120);	// 2
	ani->Add(11016);
	ani->Add(11017);
	ani->Add(11018);
	animations->Add(710, ani);

	ani = new CAnimation(100);	// Torch
	ani->Add(80000);
	ani->Add(80001);
	animations->Add(800, ani);

	for (int i = 1; i < 4; i++)
	{
		CTorch *torch = new CTorch();
		torch->AddAnimation(800);
		torch->SetPosition(400.0f * i, 235.0f);
		objects.push_back(torch);
	}
	
	simon = CSimon::GetInstance();
	simon->AddAnimation(401);		// 0 idle right 
	simon->AddAnimation(400);		// 1 idle left 

	simon->AddAnimation(501);		// 2 walk right 
	simon->AddAnimation(500);		// 3 walk left

	simon->AddAnimation(601);		// 4 jump right 
	simon->AddAnimation(600);		// 5 jump left

	simon->AddAnimation(700);		// 6 action right
	simon->AddAnimation(701);		// 7 action left
	
	simon->AddAnimation(000);		// 8 death
	simon->AddAnimation(000);		// 9 death

	simon->AddAnimation(201);		// 10 death right
	simon->AddAnimation(211);		// 11 death left


	simon->SetPosition(50.0f, 0);
	objects.push_back(simon);

	morningstar = new CMorningstar();
	morningstar->AddAnimation(710);
	morningstar->AddAnimation(720);
	objects.push_back(morningstar);



	for (int i = 0; i < 30; i++)
	{
		CBox2 *box2 = new CBox2();
		box2->AddAnimation(222);
		box2->SetPosition(0 + i * 32.0f, 300.0f);
		objects.push_back(box2);
	}

	
	
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt,&coObjects);
	}

	float cameraX = simon->x - SCREEN_WIDTH / 2;
	float cameraY = 0;
	game->setCamera(cameraX, cameraY);
	
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}