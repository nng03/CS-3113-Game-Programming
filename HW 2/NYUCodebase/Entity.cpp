//
//  Entity.cpp
//  NYUCodebase
//
//  Created by fp on 2/16/15.
//

#include "Entity.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

Entity::Entity() {}

void Entity::draw() const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    
    GLfloat quad[] = { -width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}