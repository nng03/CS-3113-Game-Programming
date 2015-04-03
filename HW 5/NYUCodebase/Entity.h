//
//  Entity.h
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Entity__
#define __NYUCodebase__Entity__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity
{
public:
    
    Entity();
    Entity(int texture, float xpos, float ypos, float rotate, float width, float height, float xvel, float yvel, float xaccel, float yaccel, float xfric, float yfric, float mas, float scal, bool staticq);
    Entity(int spriteSheet, float xpos, float ypos, float rotate, float spriteIndex, int spriteCountX, int spriteCountY, float width, float height, float spritew, float spriteh, float xvel, float yvel, float xaccel, float yaccel, float xfric, float yfric, float mas, float scal, bool staticq);
    ~Entity();
    
    void render();
    void fixedUpdate();
    
    bool collidedWith(Entity *other);
    
    float x;
    float y;
    float rotation;
    int textureID;
    float scale;
    float w;
    float h;
    float uSprite;
    float vSprite;
    float wSprite;
    float hSPrite;
    float indexSprite;
    
    float xVelocity;
    float yVelocity;
    float xAccel;
    float yAccel;
    float xFriction;
    float yFriction;
    float mass;
    bool isStatic;
    bool isVisible;
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedRight;
    bool collidedLeft;
    
};

#endif /* defined(__NYUCodebase__Entity__) */
