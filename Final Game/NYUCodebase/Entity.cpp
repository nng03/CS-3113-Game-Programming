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

float lerp(float v0, float v1, float t)
{
    return (1.0 - t) * v0 + t * v1;
}

Entity::Entity()
{
    
}

void Entity::render(float scale)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    float scale_x = 1.0f;
    float scale_y = 1.0f;
    GLfloat quad[] = { -width * scale*scale_x, height * scale*scale_y, -width * scale*scale_x, -height * scale*scale_y,
        width * scale*scale_x, -height * scale*scale_y, width * scale*scale_x, height * scale*scale_y };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = { u, v, u, v + h, u + w, v + h, u + w, v };
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

bool Entity::collidedWith(Entity *other)
{
    return (
            x + (w * 0.5) > other->x - (other->w * 0.5) &&
            x - (w * 0.5) < other->x + (other->w * 0.5) &&
            y + (h * 0.5) > other->y - (other->h * 0.5) &&
            y - (h * 0.5) < other->y + (other->h * 0.5));
    return false;
}

//void Entity::createMatrix()
//{
//    Matrix scale;
//    Matrix rotate;
//    Matrix translate;
//    matrix.identity();
//    scale.identity();
//    rotate.identity();
//    translate.identity();
//    
//    scale.m[0][0] = xScale;
//    scale.m[1][1] = yScale;
//    scale.m[2][2] = 1.0f;
//    
//    rotate.m[0][0] = cos(rotation);
//    rotate.m[1][0] = -sin(rotation);
//    rotate.m[0][1] = sin(rotation);
//    rotate.m[1][1] = cos(rotation);
//    
//    translate.m[3][0] = x;
//    translate.m[3][1] = y;
//    translate.m[3][2] = 1.0f;
//    
//    //matrix = scale * rotate * translate;
//    matrix = translate * rotate * scale;
//}

void Entity::update()
{
    xVelocity = lerp(xVelocity, 0.0f, FIXED_TIMESTEP * xFriction);
    yVelocity = lerp(yVelocity, 0.0f, FIXED_TIMESTEP * yFriction);
    xVelocity += xAccel * FIXED_TIMESTEP;
    yVelocity += yAccel * FIXED_TIMESTEP;
    x = xVelocity * FIXED_TIMESTEP;
    y = yVelocity * FIXED_TIMESTEP;
}
