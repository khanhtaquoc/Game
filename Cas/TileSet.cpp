#include "TileSet.h"


CTileSet::CTileSet(int spriteId)
{
	CSprites* sp = CSprites::GetInstance();
	sp->Get(spriteId);
	this->_tileImage = sp;
	this->_heighttile = ;
	this->_widthtile = sp->getFrameWidth();
}


CTileSet::~CTileSet()
{
}


