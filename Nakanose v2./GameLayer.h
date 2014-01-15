// GameLayer.h
//
//Nakanose 1.0
//Created by Carlos E. Vanegas
//Black Squirrel TM 2013 All Rights Reserved.
//



#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "Hammerhead.h"
#include "Terrain.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "Terrain.h"
#include "Hammerhead.h"

USING_NS_CC;
using namespace CocosDenshion;


typedef enum {
	
    kGameIntro,
    kGamePlay,
    kGameOver,
    kGameTutorial,
    kGameTutorialLift,
    kGameTutorialFloat,
    kGameTutorialDrop,
	kSpriteWaste,
	kSpriteHealth,
	kSpriteHalo,
	kSpriteSparkle
} GameState;

class GameLayer: public cocos2d::CCLayer
{
    Terrain *_terrain;
    Hammerhead *_hammerhead;
    CCLabelBMFont * _scoreDisplay;
    
    CCSprite *_intro;
    CCSprite *_tryAgain;
    CCSprite *_background;
    CCSprite *_fin;
    CCSprite *_foreground;
	
	CCArray * _backgroundFish /*_Clouds*/
		
	CCSpriteBatchNode * _gameBatchNode;
	CCLabelTTF * _tutorialLabel;
	CCMeu * _mainMenu;
	
	CCSize _screenSize;
	
	GameState _state;
	
	bool _running;
	float _score;
	int _speedIncreaseInterval;
	float _speedIncreaseTimer;
	
	void createGameScreen();
	void resetGame();
	
public:
	
	~GameLayer();
	
	//'Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	//there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	//preprocessor macro for "static create()" create constructort (node () deprecated)
	
	void update (float dt);
	
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* revent);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* revent);
    
  	virtual showTutorial (CCObject* pSender);
	virtual startGame (CCObject* pSender);
};




class GameLayer : public CCLayer
{
private:
	
	CCArray * _wastePool;
	int _wastePoolIndex;
	
	CCArray * _healthPool;
	int _healthPoolIndex;
	
	CCArray * _fallingObjects;
	CCArray * _fish;
	
	CCSpriteBatchNode * _gameBatchNode;
	
	CCLabelBMFont * _energyDisplay;
	CCLabelBMFont * _scoreDisplay;
	
	CCAction * _rotateSprite;
	CCAction * _swingHealth;
	
<<<<<<< Local Changes
	CCAction * _groundHit; //waste hits the ground surface
	CCAction * _explosion; //when waste hits the ground
	CCAction * _hammerheadPlus; // hammerhead eats fish, gains energy
	CCAction * _hammerheadMinus; //hammerhead gets hit by waste, losses energy;
=======
	CCAction * _groundHit; //might no be useful
	CCAction * _explosion;
    CCAction * _hammerheadHits;
    CCAction *_hammerheadPlus;
    CCAction *_hammerheadMinus;
>>>>>>> External Changes
	

	
	CCSize _screenSize;
    
    GameState _state;
    
    bool _running; //to define game state
    float _score;
    int _speedIncreaseInterval;
    float _speedIncreaseTimer;
	
    
    
	
	//nuclear waste attributes
	
	float _wasteInterval;
	float _wasteTimer;
	float _wasteSpeed;
	float _healthInterval;
	float _healthSpeed;
	float _fishInterval; //adding the fish-interval from fish pool 
	float _fishTimer; //fish timer.
	float _difficultyInterval; //fish should also have float _fishDifficultyInterval;
	
	int _energy;
	int _score;
	int _hammerheadHits;
	
    //reset waste
    void resetWaste(void);
    void resetHealth(void);
    
    //reset game
    void resetGame(void);
    //reset fish
	void resetFish(void);
    
    void stopGame(void);
    void increaseDifficulty(void);
    
    void createGameScreen(void);
    void createPools(void);
    void createActions(void);
    
public:
    
    ~GameLayer(void);
    
    //Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (which is an object pointer)
    
    virtua bool init();
    
    //there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    

    //preprocessor macro for "static create()" constructor (node() deprecated)
    CREATE_FUNC(GameLayer);
    
      void update (float dt);
    
    virtual void ccTouchesBegan(CCSet * pTouches, CCEvent *event);
    virtual void ccTouchesEnded(CCSet * pTouches, CCEvent *event);

    
    void showTutorial (CCObject* pSender);
    void startGame (CCObject * pSender);
    void fallingObjectDone(CCNode* pSender);
    void animationDone(CCNode* pSender);
  
    
    
    
  
    
};

#endif// __GAMELAYER_H__