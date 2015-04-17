//
//  Asteroids.cpp
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Asteroids.h"

using namespace std;

GLuint Asteroids::loadTexture(const char* imagePath)
{
    SDL_Surface *surface = IMG_Load(imagePath);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    SDL_FreeSurface(surface);
    
    return textureID;
}

Asteroids::Asteroids()
{
    init();
    done = false;
    lastFrameTicks = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);
    createWorld();
}

Asteroids::~Asteroids()
{
    SDL_Quit();
}

void Asteroids::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("ASTEROIDS", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);}

void Asteroids::createWorld()
{
    cout << "benig created" << endl;
    // Player Initialization
//    GLuint playerSprite = loadTexture("sheet.png");
//    player.sheetSprite = SheetSprite(playerSprite, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
    GLuint playerSprite = loadTexture("asteroids.png");
//    player.sheetSprite = SheetSprite(playerSprite, 0, 1, 2);
    player.textureID = playerSprite;
    player.indexSprite = 1;
    player.uSprite = 0.0;
    player.vSprite = 0.0;
    player.wSprite = 1.0;
    player.hSPrite = 0.33;
    player.x = 0.0f;
    player.y = 0.0f;
    player.scale = 1.0f;
    //player.yScale = 1.0f;
    player.rotation = 0.0f;
    player.w = 0.1f;
    player.h = 0.1f;
    player.xVelocity = 0.0f;
    player.yVelocity = 0.0f;
    player.rotationalVelocity = 0.0f;
    player.xAccel = 1.0f;
    player.yAccel = 1.0f;
    player.rotationalAccel = 0.0f;
    player.isStatic = false;
    player.xFriction = 2.0f;
    player.yFriction = 2.0f;
    player.rotationalFric = 2.0f;
    entities.push_back(&player);
    
    // Enemy Initialization
    
//    GLuint enemySprite = loadTexture("asteroids.png");
    GLuint enemySprite = loadTexture("asteroids.png");
    for (size_t i = 0; i < ASTEROIDS_MAX_NUM; i++)
    {
//        asteroids[i].sheetSprite = SheetSprite(enemySprite, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
//        asteroids[i].sheetSprite = SheetSprite(enemySprite, 1, 1, 3);
        asteroids[i].textureID = enemySprite;
        asteroids[i].indexSprite = 3;
        asteroids[i].uSprite = 0.0;
        asteroids[i].vSprite = 0.66;
        asteroids[i].wSprite = 1.0;
        asteroids[i].hSPrite = 0.33;
        asteroids[i].x = ((float)(rand() / (float)(RAND_MAX) * (1.33 + 1.33)) - 1.33);
        asteroids[i].y = ((float)(rand() / (float)(RAND_MAX) * (1 + 1)) - 1);
        asteroids[i].scale = 0.5f;
//        asteroids[i].yScale = 1.0f;
        asteroids[i].w = ((float)(rand() / (float)(RAND_MAX) * (0.5 - 0.2)) + 0.2);
        asteroids[i].h = ((float)(rand() / (float)(RAND_MAX) * (0.5 - 0.2)) + 0.2);
        asteroids[i].rotation = ((float)(rand() / (float)(RAND_MAX) * (360 - 0)) + 0);
        asteroids[i].isStatic = false;
        asteroids[i].xVelocity = 0.0f;
        asteroids[i].yVelocity = 0.0f;
        asteroids[i].rotationalVelocity = 0.0f;
        asteroids[i].xFriction = 2.0f;
        asteroids[i].yFriction = 2.0f;
        asteroids[i].rotationalFric = 2.0f;
        asteroids[i].xAccel = ((float)(rand() / (float)(RAND_MAX) * (250 + 250)) - 250);
        asteroids[i].yAccel = ((float)(rand() / (float)(RAND_MAX) * (250 + 250)) - 250);
        asteroids[i].rotationalAccel = ((float)(rand() / (float)(RAND_MAX) * (250 + 250)) - 250);
        entities.push_back(&asteroids[i]);
    }
}

void Asteroids::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->render();
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Asteroids::update(float elapsed)
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
    }
    
    
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
    {
        player.xAccel = 5.0f;
        player.yAccel = 5.0f;
    }
    else
    {
        player.xAccel = 0.0f;
        player.yAccel = 0.0f;
    }
    
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
    {
        player.rotationalAccel = 500.0f;
    }
    else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
    {
        player.rotationalAccel = -500.0f;
    }
    else
    {
        player.rotationalAccel = 0.0f;
    }
}

void Asteroids::fixedUpdate()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->fixedUpdate();
        for (size_t j = 0; j < entities.size(); j++)
        {
            if (checkCollision(entities[i], entities[j]))
            {
                Vector difference(entities[i]->x - entities[j]->x, entities[i]->y - entities[j]->y, 0.0f);
                difference.normalize();
                entities[i]->x += difference.x / 100;
                entities[i]->y += difference.y / 100;
            }
        }
        
        if (entities[i] == &player)
        {
            if (entities[i]->x > 1.33)
            {
                entities[i]->x = -1.33;
            }
            if (entities[i]->x < -1.33)
            {
                entities[i]->x = 1.33;
            }
            if (entities[i]->y > 1.0)
            {
                entities[i]->y = -1.0;
            }
            if (entities[i]->y < -1.0)
            {
                entities[i]->y = 1.0;
            }
        }
        else
        {
            if (entities[i]->x >= 1.33)
            {
                entities[i]->x = -1.33;
            }
            if (entities[i]->x <= -1.33)
            {
                entities[i]->x = 1.33;
            }
            if (entities[i]->y >= 1.0)
            {
                entities[i]->y = -1.0;
            }
            if (entities[i]->y <= -1.0)
            {
                entities[i]->y = 1.0;
            }
        }
    }
}

bool Asteroids::updateAndRender()
{
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + remainingTime;
    if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP) {
        fixedElapsed -= FIXED_TIMESTEP;
        fixedUpdate();
    }
    remainingTime = fixedElapsed;
    
    update(elapsed);
    render();
    return done;
}

bool Asteroids::checkCollision(Entity* entity, Entity* hitEntity)
{
    if (entity == hitEntity)
    {
        return false;
    }
    
    entity->createMatrix();
    hitEntity->createMatrix();
    Matrix inverseEntity = entity->matrix.inverse();
    Matrix inverseHitEntity = hitEntity->matrix.inverse();
    
    // Edge point vectors
    Vector hitEntityTopLeftCorner(-hitEntity->w * 0.5, hitEntity->h * 0.5, 0.0);
    Vector hitEntityBottomLeftCorner(-hitEntity->w * 0.5, -hitEntity->h * 0.5, 0.0);
    Vector hitEntityTopRightCorner(hitEntity->w * 0.5, hitEntity->h * 0.5, 0.0);
    Vector hitEntityBottomRightCorner(hitEntity->w * 0.5, -hitEntity->h * 0.5, 0.0);
    
    // Edge point vectors to world coords
    hitEntityTopLeftCorner = hitEntity->matrix * hitEntityTopLeftCorner;
    hitEntityBottomLeftCorner = hitEntity->matrix * hitEntityBottomLeftCorner;
    hitEntityTopRightCorner = hitEntity->matrix * hitEntityTopRightCorner;
    hitEntityBottomRightCorner = hitEntity->matrix * hitEntityBottomRightCorner;
    
    // Edge point vectors in entity's object space
    hitEntityTopLeftCorner = inverseEntity * hitEntityTopLeftCorner;
    hitEntityBottomLeftCorner = inverseEntity * hitEntityBottomLeftCorner;
    hitEntityTopRightCorner = inverseEntity * hitEntityTopRightCorner;
    hitEntityBottomRightCorner = inverseEntity * hitEntityBottomRightCorner;
    
    // Check for collisions
    float minX = fmin(fmin(fmin(hitEntityTopLeftCorner.x, hitEntityBottomLeftCorner.x), hitEntityTopRightCorner.x), hitEntityBottomRightCorner.x);
    float maxX = fmax(fmax(fmax(hitEntityTopLeftCorner.x, hitEntityBottomLeftCorner.x), hitEntityTopRightCorner.x), hitEntityBottomRightCorner.x);
    
    if (!(minX <= entity->w * 0.5 && maxX >= entity->w * 0.5))
    {
        return false;
    }
    
    float minY = fmin(fmin(fmin(hitEntityTopLeftCorner.y, hitEntityBottomLeftCorner.y), hitEntityTopRightCorner.y), hitEntityBottomRightCorner.y);
    float maxY = fmax(fmax(fmax(hitEntityTopLeftCorner.y, hitEntityBottomLeftCorner.y), hitEntityTopRightCorner.y), hitEntityBottomRightCorner.y);
    
    if (!(minY <= entity->h * 0.5 && maxY >= entity->h * 0.5))
        {
            return false;
        }
    
    // Same process with Entity as opposed to hitEntity
    
    Vector entityTopLeftCorner(-entity->w * 0.5, entity->h * 0.5, 0.0);
    Vector entityBottomLeftCorner(-entity->w * 0.5, -entity->h * 0.5, 0.0);
    Vector entityTopRightCorner(entity->w * 0.5, entity->h * 0.5, 0.0);
    Vector entityBottomRightCorner(entity->w * 0.5, -entity->h * 0.5, 0.0);
    
    entityTopLeftCorner = entity->matrix * entityTopLeftCorner;
    entityBottomLeftCorner = entity->matrix * entityBottomLeftCorner;
    entityTopRightCorner = entity->matrix * entityTopRightCorner;
    entityBottomRightCorner = entity->matrix * entityBottomRightCorner;
    
    entityTopLeftCorner = inverseHitEntity * entityTopLeftCorner;
    entityBottomLeftCorner = inverseHitEntity * entityBottomLeftCorner;
    entityTopRightCorner = inverseHitEntity * entityTopRightCorner;
    entityBottomRightCorner = inverseHitEntity * entityBottomRightCorner;
    
    float min2X = fmin(fmin(fmin(entityTopLeftCorner.x, entityBottomLeftCorner.x), entityTopRightCorner.x), entityBottomRightCorner.x);
    float max2X = fmax(fmax(fmax(entityTopLeftCorner.x, entityBottomLeftCorner.x), entityTopRightCorner.x), entityBottomRightCorner.x);
    
    if (!(min2X <= hitEntity->w * 0.5 && max2X >= hitEntity->w * 0.5))
    {
        return false;
    }
    
    float min2Y = fmin(fmin(fmin(entityTopLeftCorner.y, entityBottomLeftCorner.y), entityTopRightCorner.y), entityBottomRightCorner.y);
    float max2Y = fmax(fmax(fmax(entityTopLeftCorner.y, entityBottomLeftCorner.y), entityTopRightCorner.y), entityBottomRightCorner.y);
    
    if (!(min2Y <= hitEntity->h * 0.5 && max2Y >= hitEntity->h * 0.5))
    {
        return false;
    }
    
    return true;
}