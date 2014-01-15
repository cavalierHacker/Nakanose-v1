/*Nakanose - Mobile

Hammerhead.h
Created by Carlos E. Vanegas on 09/08/13
Black Squirrel TM All Rights Reserved
 
 Updated 11/09/2013
*/


#ifndef __HAMMERHEAD_H__
#define __HAMMERHEAD_H__

#define HAMMERHEAD_INITIAL_SPEED 8 
#define HAMMERHEAD_JUMP 42 //optional
#define GRAVITY 1.5
#define FLOATING_GRAVITY 0.4
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f //optional -- as no air involved.

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;


typedef enum {
	kHammerheadMoving,
	kHammerheadFalling,
	kHammerheadDying

} HammerheadState;

class Hammerhead : public GameSprite {

    //properties
    

    CCAction * _rideAnimation;
    CCAction * _floatAnimation;
	float _speed;
    int _floatingTimerMax;
	float _floatingTimer;
	int _floatingInterval;
	bool _hasFloated;
    
	CCSize _screenSize;
	
	void initHammerhead (void);
	

	
public:
    
    Hammerhead(void);
	~Hammerhead(void);
    
      /*Then we add five synthesized properties. In Cocos2d-x, these are macros for creating getters and setters. You declare the type, the protected variable name, and the words that will be appended to the get and set methods. So in the first CC_SYNTHESIZE method, a getState and setState method will be created to deal with the CCPoint value inside the _nextPosition protected variable.*/
    
    CC_SYNTHESIZE(HammerheadState, _state, State);
	CC_SYNTHESIZE(bool, _inAir, InAir);
	CC_SYNTHESIZE_READONLY(bool, _floating, Floating);
	CC_SYNTHESIZE(bool, _jumping, Jumping);
	CC_SYNTHESIZE(float, _maxSpeed, MaxSpeed);



	static Hammerhead * create (void);
	
	virtual void update (float dt);
	
	void setFloating (bool value);
	
	void reset (void);
	
    
	
    inline virtual void place() {
        
        this->setPositionY(_nextPosition.y);
        if (_vector.x > 0 && this->getPositionX() <_screenSize.width * 0.2f) {
            this->setPositionX(this->getPositionX() + _vector.x);
            if (this->getPositionX()> _screenSize.width * 0.2f){
                this->setPositionX(_screenSize.width * 0.2f);
            }
        }
    };

inline int left() {
    return this->getPositionX()- _width *0.5f;
}

inline int right() {
    return this->getPositionX() + _width * 0.5f;
}

inline int top() {
    return this->getPositionY();
}

inline int bottom() {
    return this->getPositionY() -_height;
}

inline int next_left() {
    return _nextPosition.x - _width *0.5f;
}


inline int next_right() {
    return _nextPosition.x + _width * 0.5f;
}

inline int next_top() {
    return _nextPosition.y;
}

inline int next_bottom() {
    return _nextPosition.y - _height;
}

};
#endif // __HAMMERHEAD_H__

