//
//  Entity.cpp
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

Entity::Entity(int texture, float xpos, float ypos, float rotate, float width, float height)
{
    textureID = texture;
    x = xpos;
    y = ypos;
    rotation = rotate;
    w = width;
    h = height;
}

Entity::Entity(int spriteSheet, float xpos, float ypos, float xdir, float ydir, float rotate, float us, float vs, float width, float height)
{
    textureID = spriteSheet;
    x = xpos;
    y = ypos;
    rotation = rotate;
    speed = 0.5f;
    xDirect = xdir;
    yDirect = ydir;
    w = width;
    h = height;
    uSprite = us;
    vSprite = vs;
}

Entity::~Entity()
{
    
}

void Entity::draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glScalef(w, h, 1.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    
    GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

void Entity::drawSprite(int index, int spriteCountX, int spriteCountY, float scale)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    
    GLfloat quad[] = {-w * scale, h * scale, -w * scale, -h * scale, w * scale, -h * scale, w * scale, h * scale};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
    float v = (float)(((int)index) / spriteCountY) / (float)spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    
    GLfloat quadUVs[] = {u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

void Entity::move(float elapsed)
{
    x += xDirect * speed * elapsed;
    y += yDirect * speed * elapsed;
}

