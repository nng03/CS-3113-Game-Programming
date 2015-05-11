//
//  Entity.h
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Entity__
#define __NYUCodebase__Entity__

#include "SheetSprite.h"
#include "Matrix.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity
{
public:
    
    Entity();
    
    void render(float scale);
    void update();
    bool collidedWith(Entity *other);
    
    GLuint textureID;
    float x = 0.0f;
    float y = 0.0f;
    float xScale = 1.0f;
    float yScale = 1.0f;
    float rotation = 0.0f;
    float width = 0.5f;
    float height = 0.5f;
    
    float xVelocity = 0.0f;
    float yVelocity = 0.0f;
    float rotationalVelocity = 0.0f;
    float xAccel = 0.0f;
    float yAccel = 0.0f;
    float rotationalAccel = 0.0f;
    float xFriction = 0.0f;
    float yFriction = 0.0f;
    float rotationalFric = 0.0f;
    
    float u;
    float v;
    float w;
    float h;
    
    bool isStatic = false;
    bool collidedBottom = false;
    bool collidedTop = false;
    bool collidedRight = false;
    bool collidedLeft = false;
};

#endif /* defined(__NYUCodebase__Entity__) */
