//
//  Entity.cpp
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include <math.h>
#include <iostream>

using namespace std;

//float lerp(float v0, float v1, float t)
//{
//    return (1.0 - t) * v0 + t * v1;
//}
//
//Entity::Entity()
//{
//    
//}
//
//void Entity::render()
//{
//    createMatrix();
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glMultMatrixf(matrix.ml);
//    sheetSprite.draw(width, height);
//    glPopMatrix();
//}
//
//void Entity::createMatrix()
//{
//    Matrix scale;
//    Matrix rotate;
//    Matrix translate;
//    
//    scale.m[0][0] = xScale;
//    scale.m[1][1] = yScale;
//    scale.m[2][2] = 1.0f;
//    
//    rotate.m[0][0] = cos(rotation * 3.14159265 / 180.0f);
//    rotate.m[1][0] = -sin(rotation * 3.14159265 / 180.0f);
//    rotate.m[0][1] = sin(rotation * 3.14159265 / 180.0f);
//    rotate.m[1][1] = cos(rotation * 3.14159265 / 180.0f);
//    
//    translate.m[3][0] = x;
//    translate.m[3][1] = y;
//    translate.m[3][2] = 0.0f;
//    
//    matrix = scale * rotate * translate;
//}
//
//void Entity::fixedUpdate()
//{
//    if (!isStatic)
//    {
//        xVelocity = lerp(xVelocity, 0.0f, FIXED_TIMESTEP * xFriction);
//        yVelocity = lerp(yVelocity, 0.0f, FIXED_TIMESTEP * yFriction);
//        rotationalVelocity = lerp(rotationalVelocity, 0.0f, FIXED_TIMESTEP * rotationalFric);
//        xVelocity += xAccel * FIXED_TIMESTEP;
//        yVelocity += yAccel * FIXED_TIMESTEP;
//        rotationalVelocity += rotationalAccel * FIXED_TIMESTEP;
//        
//        x += xVelocity * FIXED_TIMESTEP;
//        y += yVelocity * FIXED_TIMESTEP;
//        rotation += rotationalVelocity * FIXED_TIMESTEP;
//    }
//}
//

Entity::Entity()
{
    
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
    createMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glMultMatrixf(matrix.ml);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

void Entity::createMatrix()
{
    cout << "X: " << x << endl;
    cout << "Y: " << y << endl;
    cout << "rotation: " << rotation << endl;
    Matrix sca;
    Matrix rotate;
    Matrix translate;
    matrix.identity();

    sca.m[0][0] = scale;
    sca.m[1][1] = scale;
    sca.m[2][2] = 1.0f;

    rotate.m[0][0] = cos(rotation * 3.14159265 / 180);
    rotate.m[1][0] = -sin(rotation * 3.14159265 / 180);
    rotate.m[0][1] = sin(rotation * 3.14159265 / 180);
    rotate.m[1][1] = cos(rotation * 3.14159265 / 180);

    translate.m[3][0] = x;
    translate.m[3][1] = y;
    translate.m[3][2] = 0.0f;
    
    matrix = sca * rotate * translate;
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
    xVelocity = lerp(xVelocity, 0.0f, FIXED_TIMESTEP * xFriction);
    yVelocity = lerp(yVelocity, 0.0f, FIXED_TIMESTEP * yFriction);
    xVelocity += xAccel * FIXED_TIMESTEP;
    yVelocity += yAccel * FIXED_TIMESTEP;
    rotationalVelocity += rotationalAccel * FIXED_TIMESTEP;
    
    x += xVelocity * FIXED_TIMESTEP;
    y += yVelocity * FIXED_TIMESTEP;
    rotation += rotationalVelocity * FIXED_TIMESTEP;
}

