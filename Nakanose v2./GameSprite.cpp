#include "GameSprite.h"

USING_NS_CC;


GameSprite::GameSprite(void):
_vector(ccp(0,0))
_screenSize(CCDirector::sharedDirector()->getWinSize()){
}


GameSprite::~GameSprite(void){}

	
