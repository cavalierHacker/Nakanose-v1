//GameLayer.cpp
//Nakanose 1.0
//
//Created by Carlos Enrique Vanegas
//Copyright Black-Squirrel Tm 2013 All Rights Reserved.

/*
choose soundtrack
 */

#include "GameLayer.h"
#Include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace cocosDenshion;

/*GameLayer::~GameLayer()Game
    
    CC_SAFE_RELEASE(_)
}    */


CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }
    
    //get screen size
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    createGameScreen();
    
    
    resetGame();
    
    //listen for touches
    this->setTouchEnabled(true);
    
    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
    
    
    
    return true;
}


void GameLayer::resetGame () {
    
    _score = 0;
    _speedIncreaseInterval = 15;
	_speedIncreaseTimer = 0;
    
    char szValue[100] = {0};
    sprintf(szValue, "%i", (int) _score);
    _scoreDisplay->setString (szValue);
    _scoreDisplay->setAnchorPoint(ccp(1,0));
    _scoreDisplay->setPosition(ccp(_screenSize.width * 0.95f, _screenSize.height * 0.88f));
    
    _state = kGameIntro;
    
    _intro->setVisible(true);
    _mainMenu->setVisible(true);
    
    _jam->setPosition(ccp(_screenSize.width * 0.19f, _screenSize.height * 0.47f));
    _jam->setVisible(true);
    _jam->runAction(_jamAnimate);
    
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);
    _running = true;
}

void GameLayer::update(float dt) {
    
    if (!_running) return;
    
    
    if (_player->getPositionY() < -_player->getHeight() ||
        _player->getPositionX() < -_player->getWidth() * 0.5f) {
        
        if (_state == kGamePlay) {
            
            _running = false;
            
            //create GAME OVER state
            
            _state = kGameOver;
            
            _tryAgain->setVisible(true);
            _scoreDisplay->setAnchorPoint(ccp(0.5f, 0.5f));
            _scoreDisplay->setPosition(ccp(_screenSize.width * 0.5f,
                                           _screenSize.height * 0.88f));
            _hat->setPosition(ccp(_screenSize.width * 0.2f, -_screenSize.height * 0.1f));
            _hat->setVisible(true);
            CCAction * rotate = CCRotateBy::create(2.0f, 660);
            CCAction * jump = CCJumpBy::create(2.0f, ccp(0,10), _screenSize.height * 0.8f, 1);
            _hat->runAction(rotate);
            _hat->runAction(jump);
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            SimpleAudioEngine::sharedEngine()->playEffect("crashing.wav");
            
        }
    }
    
    _player->update(dt);
    
    _terrain->move(_player->getVector().x);
    
    if (_player->getState() != kPlayerDying) _terrain->checkCollision(_player);
    
    _player->place();
    
    //the parallax effect begins
    
    
    if (_player->getNextPosition().y > _screenSize.height * 0.6f) {
        _gameBatchNode->setPositionY( (_screenSize.height * 0.6f - _player->getNextPosition().y) * 0.8f);
    } else {
        _gameBatchNode->setPositionY  ( 0 );
    }
    
    
    /* first we move the _background sprite, which contains the cityscape texture repeated three times
     along the X axis, and we move it at one fourth the speed of the _player sprite */
    
    
    if (_player->getVector().x > 0) {
        _background->setPositionX(_background->getPosition().x - _player->getVector().x * 0.25f);
        
        /*The _background scrolls to the left, and as soon as the first cityscape texture is off the screen, we shift
         the entire _background container to the right at precisely the spot where the second cityscape texture
         would appear if allowed to continue.*/
        
        float diffx;
        
        if (_background->getPositionX() < -_background->getContentSize().width) {
            diffx = fabs(_background->getPositionX()) - _background->getContentSize().width;
            _background->setPositionX(-diffx); //We must subtract where the sprite would be from a total width of the sprite
        }
        
        /*A similar process is repeated with the _foreground sprite and the three lamppost sprites it contains, only the _foreground
         moves at four times the speed of the _player sprite*/
        
        _foreground->setPositionX(_foreground->getPosition().x - _player->getVector().x * 4);
        
        if (_foreground->getPositionX() < -_foreground->getContentSize().width * 4) {
            diffx = fabs(_foreground->getPositionX()) - _foreground->getContentSize().width * 4;
            _foreground->setPositionX(-diffx);
        }
        
        /*clouds follow the same parallax effect on this occassion*/
        
        int count = _clouds->count();
        CCSprite * cloud;
        for (int i = 0; i < count; i++) {
            cloud = (CCSprite *) _clouds->objectAtIndex(i);
            cloud->setPositionX(cloud->getPositionX() - _player->getVector().x * 0.15f);
            if (cloud->getPositionX() + cloud->boundingBox().size.width * 0.5f < 0 )
                cloud->setPositionX(_screenSize.width + cloud->boundingBox().size.width * 0.5f);
        }
    }
    
    if (_jam->isVisible()) {
        if (_jam->getPositionX() < -_screenSize.width * 0.2f) {
            _jam->stopAllActions();
            _jam->setVisible(false);
        }
    }
    
    
    //update score
    if (_terrain->getStartTerrain() && _player->getVector().x > 0) {
        
        _score += dt * 50;
        char szValue[100] = {0};
        sprintf(szValue, "%i", (int) _score);
        _scoreDisplay->setString (szValue);
        
        _speedIncreaseTimer += dt;
        if (_speedIncreaseTimer > _speedIncreaseInterval) {
            _speedIncreaseTimer = 0;
            _player->setMaxSpeed (_player->getMaxSpeed() + 4);
        }
    }
    
    if (_state > kGameTutorial) {
        if (_state == kGameTutorialJump) {
            if (_player->getState() == kPlayerFalling && _player->getVector().y < 0) {
                _player->stopAllActions();
                _jam->setVisible(false);
                _jam->stopAllActions();
                _running = false;
                _tutorialLabel->setString("While in the air, tap the screen to float.");
                _state = kGameTutorialFloat;
            }
        } else if (_state == kGameTutorialFloat) {
            if (_player->getPositionY() < _screenSize.height * 0.95f) {
                _player->stopAllActions();
                _running = false;
                _tutorialLabel->setString("While floating, tap the screen again to drop.");
                _state = kGameTutorialDrop;
            }
        } else {
            _tutorialLabel->setString("That's it. Tap the screen to play.");
            _state = kGameTutorial;
        }
    }
    
    
}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event) {
    
    
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
    
    if (touch) {
	    
	    CCPoint tap = touch->getLocation();
        
        switch (_state) {
                
            case kGameIntro:
                break;
            case kGameOver:
                
                if (_tryAgain->boundingBox().containsPoint(tap)) {
                    _hat->setVisible(false);
                    _hat->stopAllActions();
                    _tryAgain->setVisible(false);
                    _terrain->reset();
                    _player->reset();
                    
                    resetGame();
                }
                break;
                
            case kGamePlay:
                
                if (_player->getState() == kPlayerFalling) {
                    _player->setFloating ( _player->getFloating() ? false : true );
                    
                } else {
                    if (_player->getState() !=  kPlayerDying) {
                        SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
                        _player->setJumping(true);
                    }
                }
                
                _terrain->activateChimneysAt(_player);
                
                break;
            case kGameTutorial:
                _tutorialLabel->setString("");
                _tutorialLabel->setVisible(false);
                _terrain->setStartTerrain ( true );
                _state = kGamePlay;
                break;
            case kGameTutorialJump:
                if (_player->getState() == kPlayerMoving) {
                    SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
                    _player->setJumping(true);
                }
                break;
            case kGameTutorialFloat:
                if (!_player->getFloating()) {
                    _player->setFloating (true);
                    _running = true;
                }
                break;
            case kGameTutorialDrop:
                _player->setFloating (false);
                _running = true;
                break;
        }
    }
}

void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* event) {
    
    if (_state == kGamePlay) {
        _player->setJumping(false);
    }
    
}

void GameLayer::showTutorial (CCObject* pSender) {
    _tutorialLabel->setString("Tap the screen to make the player jump.");
    _state = kGameTutorialJump;
    _jam->runAction(_jamMove);
    _intro->setVisible(false);
    _mainMenu->setVisible(false);
    SimpleAudioEngine::sharedEngine()->playEffect("start.wav");
    _tutorialLabel->setVisible(true);
    
}

void GameLayer::startGame (CCObject* pSender) {
    _tutorialLabel->setVisible(false);
    _intro->setVisible(false);
    _mainMenu->setVisible(false);
    
    _jam->runAction(_jamMove);
    SimpleAudioEngine::sharedEngine()->playEffect("start.wav");
    _terrain->setStartTerrain ( true );
    _state = kGamePlay;
}

void GameLayer::createGameScreen () {
    
    
    CCSprite * bg = CCSprite::create("bg.png");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, kBackground);
    
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 200);
    this->addChild(_gameBatchNode, kMiddleground);
    
    CCSprite * repeat;
    
    _background = CCSprite::createWithSpriteFrameName("background.png");
    _background->setAnchorPoint(ccp(0,0));
    _gameBatchNode->addChild(_background, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("background.png");
    repeat->setAnchorPoint(ccp(0,0));
    repeat->setPosition(ccp(repeat->getContentSize().width - 1, 0));
    _background->addChild(repeat, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("background.png");
    repeat->setAnchorPoint(ccp(0,0));
    repeat->setPosition(ccp(2 * (repeat->getContentSize().width - 1), 0));
    _background->addChild(repeat, kBackground);
    
    _foreground = CCSprite::createWithSpriteFrameName("lamp.png");
    _foreground->setAnchorPoint(ccp(0,0));
    _gameBatchNode->addChild(_foreground, kForeground);
    
    repeat = CCSprite::createWithSpriteFrameName("lamp.png");
    repeat->setAnchorPoint(ccp(0,0));
    repeat->setPosition(ccp(repeat->getContentSize().width * 4, 0));
    _foreground->addChild(repeat, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("lamp.png");
    repeat->setAnchorPoint(ccp(0,0));
    repeat->setPosition(ccp(repeat->getContentSize().width * 8, 0));
    _foreground->addChild(repeat, kBackground);
    
    
    //add clouds
    CCSprite * cloud;
    _clouds = CCArray::createWithCapacity(4);
    _clouds->retain();
    float cloud_y;
    for (int i = 0; i < 4; i++) {
        cloud_y = i % 2 == 0 ? _screenSize.height * 0.7f : _screenSize.height * 0.8f;
        cloud = CCSprite::createWithSpriteFrameName("cloud.png");
        cloud->setPosition(ccp (_screenSize.width * 0.15f + i * _screenSize.width * 0.25f,  cloud_y));
        _gameBatchNode->addChild(cloud, kBackground);
        _clouds->addObject(cloud);
    }
    
    
    _terrain = Terrain::create();
    _gameBatchNode->addChild(_terrain, kMiddleground);
    
    _player = Player::create();
    _gameBatchNode->addChild(_player, kBackground);
    
    _intro = CCSprite::createWithSpriteFrameName("logo.png");
    _intro->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.7f));
    _gameBatchNode->addChild(_intro, kForeground);
    
    
    _tryAgain = CCSprite::createWithSpriteFrameName("label_tryagain.png");
    _tryAgain->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.7f));
    _tryAgain->setVisible(false);
    this->addChild(_tryAgain, kForeground);
    
    
    _scoreDisplay = CCLabelBMFont::create("000000", "font.fnt", _screenSize.width * 0.3f, kCCTextAlignmentCenter);
    _scoreDisplay->setAnchorPoint(ccp(1,0));
    _scoreDisplay->setPosition(ccp(_screenSize.width * 0.95f, _screenSize.height * 0.88f));
    this->addChild(_scoreDisplay, kBackground);
    
    
    _hat = CCSprite::createWithSpriteFrameName("hat.png");
    _hat->setVisible(false);
    _gameBatchNode->addChild(_hat, kMiddleground);
    
    
    
    _jam = CCSprite::createWithSpriteFrameName("jam_1.png");
    
    CCAnimation* animation;
    animation = CCAnimation::create();
    CCSpriteFrame * frame;
    int i;
    for(i = 1; i <= 3; i++) {
        char szName[100] = {0};
        sprintf(szName, "jam_%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(0.2f / 3.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    
    _jamAnimate = CCAnimate::create(animation);
    _jamAnimate->retain();
    _gameBatchNode->addChild(_jam, kBackground);
    
    _jam->setPosition(ccp(_screenSize.width * 0.19f, _screenSize.height * 0.47f));
    _jamMove = CCMoveTo::create(6.0f, ccp(-_screenSize.width * 0.3f, _jam->getPositionY()));
    _jamMove->retain();
    
    
    
    //add menu
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_new_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_new_off.png");
    
    CCMenuItemSprite * starGametItem = CCMenuItemSprite::create(
                                                                menuItemOff,
                                                                menuItemOn,
                                                                this,
                                                                menu_selector(GameLayer::startGame));
    
    
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_howto_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_howto_off.png");
    
    CCMenuItemSprite * howToItem = CCMenuItemSprite::create(
                                                            menuItemOff,
                                                            menuItemOn,
                                                            this,
                                                            menu_selector(GameLayer::showTutorial));
    
    _mainMenu = CCMenu::create(howToItem, starGametItem, NULL);
    _mainMenu->alignItemsHorizontallyWithPadding(120);
    //_mainMenu->alignItemsVerticallyWithPadding(50);
    _mainMenu->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.54));
    
    this->addChild(_mainMenu, kForeground);
    
    _tutorialLabel = CCLabelTTF::create("", "Times New Roman", 80);
    _tutorialLabel->setPosition(ccp (_screenSize.width * 0.5f, _screenSize.height * 0.6f) );
    this->addChild(_tutorialLabel, kForeground);
    _tutorialLabel->setVisible(false);
    
}



/*void GameLayer::createGameScreen() {
	
	//add bg
	CCSprite *bg = CCSprite::create("bg.png");
	bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height*0.5f));
	this->addChild(bg);
	
	//create spritebatch node
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
	_gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png");
	this->addChild(_gameBatchNode);
	
	 //create cityscape == create two layers of deep-sea elements.
	
    CCSprite * sprite;
    for (int i = 0; i < 2; i++) {
        sprite = CCSprite::createWithSpriteFrameName("city_dark.png");
        sprite->setPosition(ccp(_screenSize.width * (0.25f + i * 0.5f),
                              sprite->boundingBox().size.height * 0.5f));
        _gameBatchNode->addChild(sprite, kForeground);
        
        sprite = CCSprite::createWithSpriteFrameName("city_light.png");
        sprite->setPosition(ccp(_screenSize.width * (0.25f + i * 0.5f),
                              sprite->boundingBox().size.height * 0.9f));
        _gameBatchNode->addChild(sprite, kBackground);
    }
	
	
	//add seaweed
	
	for (int i=0; i<3; i++) {
		
		sprite = CCSprite::createWithSpriteFrameName("seaweed.png");
		sprite->setPosition(ccp(_screenSize.width * (0.2f + i*0.3f),
		sprite->boundingBox().size.height*0.5f));
		_gameBatchNode->addChild(sprite, kForeground);
		
	}
		//add HUD
		
		_scoreDisplay = CCLabelBMFont::create("0", "font.fnt", _screenSize.width * 0.3f);
		_scoreDisplay->setAnchorPoint(ccp(1,0.5));
		_scoreDisplay->setPosition(ccp(_screenSize.width * 0.8f, _screenSize.height * 0.94f));
		this->addChild(_scoreDisplay);
		
		_energyDisplay = CCLabelBMFont::create("100%", "font.fnt", _screenSize.width * 0.1f, kCCTextAlignmentRight);
		_energyDisplay->setPosition(ccp(_screenSize.width * 0.3f, _screenSize.height * 0.94f));
		this->addChild(_energyDisplay);
		
		CCSprite * icon = CCSprite::createWithSpriteFrameName("health_icon.png");
		icon->setPosition(ccp(_screenSize.width * 0.15f, _screenSize.height*0.94f));
		_gameBatchNode->addChild(icon, kBackground);
		
		//add fish (left-to-right) ** increase speed accordingly
		CCSprite *fish;
		_fish = CCArray::createWithCapacity(16); //increase with difficulty
		_fish->retain(); //maybe not necessary to retain
		float fish_y;
		for (int =0; i < 4; i++) {
			fish_y = 1 % 2 == - ? _screenSize.height * 0.4f : _screenSize.height * 0.5f;
			
			fish = CCSprite::createWithSpriteFrameName("fish.png");
			fish->setPosition(ccp (_screenSize.width * 0.1f + i * _screenSize.width * 0.3f, fish_y));
			_gameBatchNode->addChild(fish, kBackground);
			_fish->addObject(fish);
		}
		
       	/* consider adding shockwave + halo later */
		
		//intro message
		_introMessage = CCSprite::createWithSpriteFrameName("logo.png");
		_introMessage ->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.65));
		_introMessage->setVisible(true);
		this->addChild(_introMessage, kForeground);
		
		//game over message
		_gameOverMessage = CCSprite::createWithSpriteFrameName("gameover.png");
		_gameOverMessage->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.65f));
		_gameOverMessage->setVisible(false);
		this->addChild(_gameOverMessage, kForeground);
		
	}
	
	void GameLayer::createPools() {
		
		CCSprite * sprite;
		int i;
		
		//create waste pool
		_wastePool = CCArray::createWithCapacity(50);
		_wastePool->retain();
		_wastePoolIndex=0;
		for (i=0; i<50; i++) {
			
			sprite = CCSprite::createWithSpriteFrameName("waste.png");
			sprite->setVisible(false);
			_gameBatchNode->addChild(sprite, kMiddleground, kSpriteWaste);
			_wastePool->addObject(sprite);
			
		}
		
		//create health pool
		_healthPool = CCArray::createWithCapacity(20);
		_healthPool->retain();
		_healthPoolIndex =0;
		for (i=0; i<20;i++) {
			
			sprite = CCSprite::createWithSpriteFrameName("health.png");
			sprite->setVisible(false);
			sprite->setAnchorPoint(ccp(0.5f, 0.8f));
			_gameBatchNode->addChild(sprite, kMiddleground, kSpritehealth);
			_healthPool->addObject(sprite);
			
		}
		
	}
	
	void GameLayer::createActions() {
		
		//swing action for health drops
		CCFiniteTimeAction* easeSwing = CCSequence::create(
			CCEaseInOut::create(CCRotateTo::create(1.2f, 10), 2), CCEaseInOut::create(CCRotateTo::create(1.2f, 10), 2), NULL);
		_swingHealth = CCRepeatForever::create((CCActionInterval *)easeSwing);
		_swingHealth->retain();
		
		/* //action sequence for shockwave: fade out, call back when done
		
		_shockwaveSequence = CCSequence::create(
                    CCFadeOut::create(1.0f),
                    CCCallFunc::create(this, callfunc_selector(GameLayer::shockwaveDone)),
                    NULL);
    _shockwaveSequence->retain();
    
    //action to grow bomb
    _growBomb = CCScaleTo::create(6.0f, 1);
    _growBomb->retain();
    
    //action to rotate sprites
    CCActionInterval*  rotate = CCRotateBy::create(0.5f ,  -90);
    _rotateSprite = CCRepeatForever::create( rotate );
    _rotateSprite->retain();
    
    
    //sprite animations
    CCAnimation* animation;
    animation = CCAnimation::create();
    CCSpriteFrame * frame;
    int i;
    //animation for ground hit
    for(i = 1; i <= 10; i++) {
        char szName[100] = {0};
        sprintf(szName, "boom%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(1 / 10.0f);
    animation->setRestoreOriginalFrame(true);
    _groundHit = CCSequence::create(
                CCMoveBy::create(0, ccp(0,_screenSize.height * 0.12f)),
                CCAnimate::create(animation),
                CCCallFuncN::create(this, callfuncN_selector(GameLayer::animationDone)),
                NULL
                );
   _groundHit->retain();

    animation = CCAnimation::create();
    //animation for falling object explosion
    for(i = 1; i <= 7; i++) {
        char szName[100] = {0};
        sprintf(szName, "explosion_small%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(0.5 / 7.0f);
    animation->setRestoreOriginalFrame(true);
    _explosion = CCSequence::create(
            CCAnimate::create(animation),
            CCCallFuncN::create(this, callfuncN_selector(GameLayer::animationDone)),
            NULL
            );
    ;
    _explosion->retain();
}
		*/

		
		
		
    /*
		
	
	
