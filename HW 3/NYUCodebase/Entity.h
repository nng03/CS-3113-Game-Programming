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

class Entity
{
public:
    
    Entity(int texture, float xpos, float ypos, float rotate, float width, float height);
    Entity(int spriteSheet, float xpos, float ypos, float xdir, float ydir, float rotate, float us, float vs, float width, float height);
    ~Entity();
    
    void draw();
    void drawSprite(int index, int spriteCountX, int spriteCountY, float scale);
    void move(float elapsed);
    
    float x;
    float y;
    float rotation;
    int textureID;
    float w;
    float h;
    float uSprite;
    float vSprite;
    
    float speed;
    float xDirect;
    float yDirect;
    
};

#endif /* defined(__NYUCodebase__Entity__) */
