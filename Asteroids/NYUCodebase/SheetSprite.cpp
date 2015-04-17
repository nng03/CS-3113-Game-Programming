//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.h"
#include <iostream>
using namespace std;

SheetSprite::SheetSprite()
{
    
}

SheetSprite::SheetSprite(GLuint idText, float index, int countX, int countY, float scaleX, float scaleY) : textureID(idText), index(index), spriteCountX(countX), spriteCountY(countY), xScale(scaleX), yScale(scaleY)
{
    u = (float)(((int)index) % countX) / (float)countX;
    v = (float)(((int)index) / countX) / (float)countY;
    width = 1.0/(float)countX;
    height = 1.0/(float)countY;
}

SheetSprite::SheetSprite(GLuint textureId, float u, float v, float width, float height) : textureID(textureId), u(u), v(v), width(width), height(height)
{
    
}

void SheetSprite::draw(float drW, float drH)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    GLfloat quad[] = {
        -width * xScale, height * yScale,
        -width * xScale, -height * yScale,
        width * xScale, -height * yScale,
        width * xScale, height * yScale
    };
    
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
//    u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
//    v = (float)(((int)index) / spriteCountY) / (float)spriteCountY;
//    width = 1.0/(float)spriteCountX;
//    height = 1.0/(float)spriteCountY;

    GLfloat quadUVs[] = {
        u, v,
        u, v + height,
        u + width, v + height,
        u + width, v
    };
    
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}