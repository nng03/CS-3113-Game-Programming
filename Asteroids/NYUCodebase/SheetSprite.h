//
//  SheetSprite.h
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__SheetSprite__
#define __NYUCodebase__SheetSprite__

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite
{
public:
    SheetSprite();
    SheetSprite(GLuint idText, float index, int countX, int countY, float scaleX = 0.5f, float scaleY = 0.5f);
    SheetSprite(GLuint textureId, float u, float v, float width, float height);
    
    void draw(float drW, float drH);
    
    GLuint textureID;
    float xScale;
    float yScale;
    float u;
    float v;
    float width;
    float height;
    float index;
    int spriteCountX;
    int spriteCountY;
};

#endif /* defined(__NYUCodebase__SheetSprite__) */
