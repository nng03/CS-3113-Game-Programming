//
//  Platformer.cpp
//  NYUCodebase
//
//  Created by fp on 3/11/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "Platformer.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

#include "Entity.h"

using namespace std;

GLuint Platformer::loadTexture(const char* imagePath)
{
    SDL_Surface *surface = IMG_Load(imagePath);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void Platformer::drawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y)
{
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    float texture_size = 1.0/16.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, ((size+spacing) * i) + (-0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        
        texCoordData.insert(texCoordData.end(), {texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +texture_size, texture_y + texture_size, texture_x + texture_size, texture_y});
    }
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0f);
    
    glDisableClientState(GL_COLOR_ARRAY);
}

void Platformer::handleXCollision(Entity *entity, vector<Entity*> group)
{
    for(size_t i = 0; i < group.size(); i++)
    {
        if (entities[i] != entity && group[i]->isStatic && entity->collidedWith(group[i]))
        {
            float xPenetration = fabs(entity->x - group[i]->x) - (entity->w * 0.5) - (group[i]->w * 0.5) - COLLISION_OFFSET;
            if (entity->x < group[i]->x)
            {
                entity->x += xPenetration;
                entity->xVelocity = 0.0f;
                entity->collidedLeft = true;
            }
            if (entity->x > group[i]->x)
            {
                entity->x -= xPenetration;
                entity->xVelocity = 0.0f;
                entity -> collidedRight = true;
            }
        }
    }
}

void Platformer::handleYCollision(Entity *entity, vector<Entity*> group)
{
    for(size_t i = 0; i < group.size(); i++)
    {
        if (entities[i] != entity && group[i]->isStatic && entity->collidedWith(group[i]))
        {
            float yPenetration = fabs(entity->y - group[i]->y) - (entity->h * 0.5) - (group[i]->h * 0.5) - COLLISION_OFFSET;
            if (entity->y < group[i]->y)
            {
                entity->y += yPenetration;
                entity->yVelocity = 0.0f;
                entity->collidedTop = true;
            }
            if (entity->y > group[i]->y)
            {
                entity->y -= yPenetration;
                entity->yVelocity = 0.0f;
                entity -> collidedBottom = true;
            }
        }
    }
}

void Platformer::reloadHearts()
{
    GLuint spriteSheet = loadTexture("spritesheet.png");
    int spriteCountX = 30;
    int spriteCountY = 30;
    float spriteIndex = 373;
    for(size_t i = 0; i < MAX_HEARTS; i++)
    {
//        float x = (float(rand() % 200) - 99) / 100;
//        float y = (float(rand() % 160) - 79) / 100;
//        float uSprite = (float)(((int)spriteIndex) % spriteCountX) / (float)spriteCountX;
//        float vSprite = (float)(((int)spriteIndex) / spriteCountY) / (float)spriteCountY;
//        Entity *heart = new Entity(spriteSheet, x, y, 0.0f, spriteIndex, spriteCountX, spriteCountY, 0.1f, 0.1f, uSprite, vSprite, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, false);
        hearts[i]->textureID = spriteSheet;
        hearts[i]->isStatic = false;
        hearts[i]->indexSprite = spriteIndex;
        hearts[i]->x = (float(rand() % 200) - 99) / 100;
        hearts[i]->y = (float(rand() % 160) - 79) / 100;
        hearts[i]->uSprite = (float)(((int)spriteIndex) % spriteCountX) / (float)spriteCountX;
        hearts[i]->vSprite = (float)(((int)spriteIndex) / spriteCountY) / (float)spriteCountY;
        hearts[i]->wSprite = 1.0/(float)spriteCountX;
        hearts[i]->hSPrite = 1.0/(float)spriteCountY;
        hearts[i]->w = 0.1f;
        hearts[i]->h = 0.1f;
        entities.push_back(hearts[i]);
    }
}

Platformer::Platformer()
{
    init();
    
    done = false;
    lastFrameTicks = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);
    buildWorld();
}

Platformer::~Platformer()
{
    SDL_Quit();
}

void Platformer::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Single Screen Platformer", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void Platformer::buildWorld()
{
    GLuint playerSprite = loadTexture("link.png");
    GLuint spriteSheet = loadTexture("spritesheet.png");
    int spriteCountX = 30;
    int spriteCountY = 30;
    
    player = new Entity(playerSprite, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 4.0f, 4.0f, 2.0f, 1.0f, false);
    entities.push_back(player);
    
    // Tiles
    
    float tileSpriteIndex = 92;
    
    vector<pair<float, float>> tileCoordinates;
    float leftEnd = -1.0f;
    float rightEnd = 1.0f;
    while (leftEnd < rightEnd + 0.1)
    {
        tileCoordinates.push_back( {leftEnd, -0.8f} );
        tileCoordinates.push_back( {leftEnd, 0.8f} );
        if (leftEnd < -0.3f || leftEnd > 0.3f)
        {
            tileCoordinates.push_back( {leftEnd, 0.0f} );
        }
        if (leftEnd> -0.4f && leftEnd < 0.4f)
        {
            tileCoordinates.push_back( {leftEnd, 0.4f} );
            tileCoordinates.push_back( {leftEnd, -0.4f} );
        }
        leftEnd += 0.1f;
        leftEnd = roundf(leftEnd * 100) / 100;
    }
    float topEnd = 0.8f;
    float bottomEnd = -0.8f;
    while (topEnd > bottomEnd)
    {
        tileCoordinates.push_back( {-1.0f, topEnd} );
        tileCoordinates.push_back( {1.0f, topEnd} );
        topEnd -= 0.1f;
        topEnd = roundf(topEnd * 100) / 100;
    }
    for (int i = 0; i < tileCoordinates.size(); i++)
    {
        float uSprite = (float)(((int)tileSpriteIndex) % spriteCountX) / (float)spriteCountX;
        float vSprite = (float)(((int)tileSpriteIndex) / spriteCountY) / (float)spriteCountY;
        Entity *tile = new Entity(spriteSheet, tileCoordinates[i].first, tileCoordinates[i].second, 0.0f, tileSpriteIndex, spriteCountX, spriteCountY, 0.1f, 0.1f, uSprite, vSprite, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, true);
        tiles[i] = tile;
        entities.push_back(tiles[i]);
    }
    
    
    for (size_t i = 0; i < MAX_HEARTS; i++)
    {
        hearts[i] = new Entity();
    }
    
    reloadHearts();
}

void Platformer::render()
{
    glClearColor(94/255.0, 128/255.0, 160/255.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for(size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->render();
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Platformer::update(float elapsed)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat) {
                player->yVelocity += 3.0f;
            }
            if ( (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W) &&
                !event.key.repeat && player->collidedBottom == true) {
                player->yVelocity += 5.0f;
            }
        }
    }
    
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
        player->xAccel = 5.0f;
    }
    else if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
        player->xAccel = -5.0f;
    }
    else {
        player->xAccel = 0.0f;
    }
    
    for (size_t i = 0; i < MAX_HEARTS; i++)
    {
        if (player->collidedWith(hearts[i]))
        {
            if (gotAllHearts)
            {
                reloadHearts();
            }
            else
            {
                for (size_t i = 0; i < entities.size(); i++)
                {
                    if (entities[i] == hearts[i])
                    {
                        delete entities[i];
                    }
                    hearts[i] = nullptr;
                }
            }
        }
    }
}

void Platformer::fixedUpdate()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->fixedUpdate();
        entities[i]->collidedTop = false;
        entities[i]->collidedBottom = false;
        entities[i]->collidedLeft = false;
        entities[i]->collidedRight = false;
        if (!entities[i]->isStatic)
        {
            entities[i]->yVelocity += GRAVITY * FIXED_TIMESTEP;
        }
    }
    
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (!entities[i]->isStatic)
        {
            entities[i]->x += entities[i]->xVelocity * FIXED_TIMESTEP;
            handleXCollision(entities[i], entities);
        }
    }
    
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (!entities[i]->isStatic)
        {
            entities[i]-> y += entities[i]->yVelocity * FIXED_TIMESTEP;
            handleYCollision(entities[i], entities);
        }
    }
}

bool Platformer::updateAndRender()
{
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    float fixedElapsed = elapsed + timeRemaining;
    if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP) {
        fixedElapsed -= FIXED_TIMESTEP;
        fixedUpdate();
    }
    timeRemaining = fixedElapsed;
    update(elapsed);
    render();
    return done;
}

