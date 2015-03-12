//
//  Platformer.h
//  NYUCodebase
//
//  Created by fp on 3/11/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Platformer__
#define __NYUCodebase__Platformer__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>

#include "Entity.h"

#define GRAVITY -9
#define MAX_HEARTS 6
#define MAX_TILES 102
#define COLLISION_OFFSET 0.001f

class Platformer
{
public:
    
    Platformer();
    ~Platformer();
    
    void init();
    void buildWorld();
    bool updateAndRender();
    
    void render();
    void update(float elapsed);
    void fixedUpdate();
    
    GLuint loadTexture(const char* imagePath);
    void drawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
    
    void handleXCollision(Entity *entity, std::vector<Entity*> group);
    void handleYCollision(Entity *entity, std::vector<Entity*> group);
    void reloadHearts();

private:
    
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    float timeRemaining = 0.0f;
    const Uint8* keys;
    std::vector<Entity*> entities;
    Entity *player;
    Entity *tiles[MAX_TILES];
    Entity *hearts[MAX_HEARTS];
    bool gotAllHearts = false;
    
};
#endif /* defined(__NYUCodebase__Platformer__) */
