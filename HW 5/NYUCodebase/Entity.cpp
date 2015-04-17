//
//  Entity.cpp
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include <iostream>

using namespace std;

Entity::Entity()
{
    x = 0.0f;
    y = 0.0f;
    rotation = 0.0f;
    textureID = 0;
    scale = 1.0f;
    w = 0.0f;
    h = 0.0f;
    uSprite = 0.0f;
    vSprite = 0.0f;
    wSprite = 0.0f;
    hSPrite = 0.0f;
    indexSprite = 0.0f;
    xVelocity = 0.0f;
    yVelocity = 0.0f;
    xAccel = 0.0f;
    yAccel = 0.0f;
    xFriction = 0.0f;
    yFriction = 0.0f;
    mass = 0;
    isStatic = false;
    isVisible = true;
    collidedTop = false;
    collidedBottom = false;
    collidedRight = false;
    collidedLeft = false;
}

Entity::Entity(int texture, float xpos, float ypos, float rotate, float width, float height, float xvel, float yvel, float xaccel, float yaccel, float xfric, float yfric, float mas, float scal, bool staticq = false)
{
    textureID = texture;
    x = xpos;
    y = ypos;
    rotation = rotate;
    w = width;
    h = height;
    xVelocity = xvel;
    yVelocity = yvel;
    xAccel = xaccel;
    yAccel = yaccel;
    xFriction = xfric;
    yFriction = yfric;
    mass = mas;
    scale = scal;
    isStatic = staticq;
}

Entity::Entity(int spriteSheet, float xpos, float ypos, float rotate, float spriteIndex, int spriteCountX, int spriteCountY, float width, float height, float spritew, float spriteh, float xvel, float yvel, float xaccel, float yaccel, float xfric, float yfric, float mas, float scal, bool staticq = false)
{
    textureID = spriteSheet;
    x = xpos;
    y = ypos;
    rotation = rotate;
    w = width;
    h = height;
    indexSprite = spriteIndex;
    uSprite = (float)(((int)spriteIndex) % spriteCountX) / (float)spriteCountX;
    vSprite = (float)(((int)spriteIndex) / spriteCountY) / (float)spriteCountY;
    wSprite = 1.0f/(float)spritew;
    hSPrite = 1.0f/(float)spriteh;
    xVelocity = xvel;
    yVelocity = yvel;
    xAccel = xaccel;
    yAccel = yaccel;
    xFriction = xfric;
    yFriction = yfric;
    mass = mas;
    scale = scal;
    isStatic = staticq;
}

Entity::~Entity()
{
    
}

float lerp(float v0, float v1, float t)
{
    return (1.0 - t) * v0 + t * v1;
}

void Entity::render()
{
    if (isVisible)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(x, y, 0.0f);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        GLfloat quad[] = {
            -w * scale, h * scale,
            -w * scale, -h * scale,
            w * scale, -h * scale,
            w * scale, h * scale
        };
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = {
            uSprite, vSprite,
            uSprite, vSprite + hSPrite,
            uSprite + wSprite, vSprite + hSPrite,
            uSprite + wSprite, vSprite
        };
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
        
        glPopMatrix();
    }
}

bool Entity::collidedWith(Entity *other)
{
    if (isVisible)
    {
    return (
            x + (w * 0.5) > other->x - (other->w * 0.5) &&
            x - (w * 0.5) < other->x + (other->w * 0.5) &&
            y + (h * 0.5) > other->y - (other->h * 0.5) &&
            y - (h * 0.5) < other->y + (other->h * 0.5));
    }
    return false;
}

void Entity::fixedUpdate()
{
    if (!isStatic)
    {
        xVelocity = lerp(xVelocity, 0.0f, FIXED_TIMESTEP * xFriction);
        yVelocity = lerp(yVelocity, 0.0f, FIXED_TIMESTEP * yFriction);
        xVelocity += xAccel * FIXED_TIMESTEP;
        yVelocity += yAccel * FIXED_TIMESTEP;
    }
}

