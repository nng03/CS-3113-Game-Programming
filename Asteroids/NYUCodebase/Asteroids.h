//
//  Asteroids.h
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Asteroids__
#define __NYUCodebase__Asteroids__

#include "Entity.h"
#include "Vector.h"
#include <SDL.h>
#include <vector>

#define PI 3.14159265
#define ASTEROIDS_MAX_NUM 12

class Asteroids
{
public:
    Asteroids();
    ~Asteroids();
    
    void init();
    void createWorld();
    bool updateAndRender();
    
    void render();
    void update(float elapsed);
    void fixedUpdate();
    
    bool checkCollision(Entity* entity, Entity* hitEntity);
    
    GLuint loadTexture(const char* imagePath);
private:
    bool done;
    SDL_Window* displayWindow;
    float lastFrameTicks;
    float remainingTime = 0.0f;
    const Uint8* keys;
    Entity player;
    Entity asteroids[ASTEROIDS_MAX_NUM];
    std::vector<Entity*> entities;
};

#endif /* defined(__NYUCodebase__Asteroids__) */
