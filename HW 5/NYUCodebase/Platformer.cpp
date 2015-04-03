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
#include <SDL_mixer.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

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

bool Platformer::readHeader(std::ifstream &stream) {
    string line;
    mapWidth = -1;
    mapHeight = -1;
    while(getline(stream, line))
    {
        if(line == "")
        {
            break;
        }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "width")
        {
            mapWidth = atoi(value.c_str());
        }
        else if(key == "height")
        {
            mapHeight = atoi(value.c_str());
        }
    }
    if (mapWidth == -1 || mapHeight == -1)
    {
        return false;
    }
    else
    {
        levelData = new unsigned char*[mapHeight];
        for(int i = 0; i < mapHeight; ++i)
        {
            levelData[i] = new unsigned char[mapWidth];
        }
        return true;
    }
}

bool Platformer::readLayerData(std::ifstream &stream) {
    string line;
    while(getline(stream, line))
    {
        if(line == "")
        {
            break;
        }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data")
        {
            for(int y = 0; y < mapHeight; y++)
            {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                for(int x = 0; x < mapWidth; x++)
                {
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if (val > 0)
                    {
                        levelData[y][x] = val;
                    }
                    else
                    {
                        levelData[y][x] = 0;
                    }
                }
            }
        }
    }
    return true;
}

bool Platformer::readEntityData(std::ifstream &stream) {
    string line;
    string type;
    while(getline(stream, line))
    {
        if(line == "")
        {
            break;
        }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "type")
        {
            type = value;
        }
        else if(key == "location")
        {
            istringstream lineStream(value);
            string xPos, yPos;
            getline(lineStream, xPos, ',');
            getline(lineStream, yPos, ',');
            float placeX = atoi(xPos.c_str()) / 16 * TILE_SIZE;
            float placeY = atoi(yPos.c_str()) / 16 * -TILE_SIZE;
            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}

void Platformer::placeEntity(string type, float placeX, float placeY)
{
    if (type == "Player")
    {
        player->x = placeX;
        player->y = placeY;
    }
    else if (type == "Hearts")
    {
        Entity* heart = new Entity();
        heart->textureID = spriteSheet;
        heart->x = placeX;
        heart->y = placeY;
        heart->w = 0.1f;
        heart->h = 0.1f;
        heart->isStatic = true;
        heart->indexSprite = 5;
        heart->uSprite = 0.0f;
        heart->vSprite = -0.6f;
        heart->wSprite = 0.5f;
        heart->hSPrite = 0.5f;
        hearts.push_back(heart);
    }
}

void Platformer::loadMap()
{
    ifstream infile("mapFile.txt");
    string line;
    while (getline(infile, line))
    {
        if(line == "[header]")
        {
            if(!readHeader(infile))
            {
                return;
            }
        }
        else if(line == "[layer]")
        {
            readLayerData(infile);
        }
        else if(line == "[Hearts]" || line == "[Player]")
        {
            readEntityData(infile);
        }
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
    Mix_FreeChunk(jump);
    Mix_FreeChunk(gotHeart);
    Mix_FreeMusic(bgm);
    SDL_Quit();
}

void Platformer::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Scrolling Platformer", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0, 2.66, -4.5, -0.5, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    spriteSheet = loadTexture("sprites.png");
}

void Platformer::buildWorld()
{    
    gotHeart = Mix_LoadWAV("gotHeartSound.wav");
    jump = Mix_LoadWAV("jumpSound.wav");
    bgm = Mix_LoadMUS("sariasSong.mp3");
    Mix_PlayMusic(bgm, -1);
    
    player = new Entity();
    player->textureID = spriteSheet;
    player->scale = 2.0f;
    player->x = 0.0f;
    player->y = 0.0f;
    player->w = 0.1f;
    player->h = 0.1f;
    player->isStatic = false;
    player->xVelocity = 0.0f;
    player->yVelocity = 0.0f;
    player->xAccel = 0.0f;
    player->yAccel = 0.0f;
    player->xFriction = 6.0f;
    player->yFriction = 2.0f;
    player->indexSprite = 2;
    player->uSprite = 0.4f;
    player->vSprite = 0.0f;
    player->wSprite = 0.5f;
    player->hSPrite = 0.5f;
    
    entities.push_back(player);
    
    // Tiles
    
    tileSprites = loadTexture("sprites.png");
    loadMap();
    
    //reloadHearts();
}

void Platformer::worldToTileCoords(float worldX, float worldY, int* gridX, int* gridY)
{
    *gridX = (int)(worldX / 0.1f);
    *gridY = (int)(-worldY / 0.1f);
}

bool Platformer::isSolid(unsigned char tile)
{
    switch (tile)
    {
        case 9:
            return true;
            break;
        default:
            return false;
            break;
    }
}

void Platformer::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float xTranslate = -player->x + 2.66;
    float yTranslate = -player->y - 0.66;
    
    if (yTranslate > 0)
    {
        yTranslate = 0;
    }
    if (xTranslate > 0)
    {
        xTranslate = 0;
    }
    if (xTranslate < -0.1f * (mapWidth/4) - 0.66)
    {
        xTranslate = -0.1f * (mapWidth/4) - 0.66;
    }
    glTranslatef(xTranslate, yTranslate, 0.0f);
    
    // Render Map
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glBindTexture(GL_TEXTURE_2D, spriteSheet);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    vector<float> vertexData;
    vector<float> texCoordData;
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (levelData[y][x])
            {
                float u = (float)(((int)levelData[y][x] - 1) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
                float v = (float)(((int)levelData[y][x] - 1) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
                float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
                float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
                vertexData.insert(vertexData.end(),
                {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
                });
                texCoordData.insert(texCoordData.end(),
                                    { u, v,
                    u, v + (spriteHeight),
                    u + spriteWidth, v + (spriteHeight),
                    u + spriteWidth, v
                });
            }
        }
    }
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, (GLsizei)vertexData.size()/2);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
    
    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->render();
    }
    
    for (size_t i = 0; i < hearts.size(); i++)
    {
        hearts[i]->render();
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
    
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP]) && player->collidedBottom)
    {
        player->yVelocity = 5.0f;
        player->collidedBottom = false;
        Mix_PlayChannel(-1, jump, 0);
    }
    
    if (player->y < -4.5 || player->x < 0.0 || player->x > 6.0)
    {
        player->x = 0.0f;
        player->y = 0.0f;
    }

    for (size_t i = 0; i < hearts.size(); i++)
    {
        if (player->collidedWith(hearts[i]))
        {
            Mix_PlayChannel(-1, gotHeart, 0);
            hearts[i]->isVisible = false;
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
            handleXCollision(entities[i]);
        }
    }
    
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (!entities[i]->isStatic)
        {
            entities[i]-> y += entities[i]->yVelocity * FIXED_TIMESTEP;
            handleYCollision(entities[i]);
        }
    }
}

float Platformer::checkXCoordCollision(float num1, float num2)
{
    int gridX;
    int gridY;
    
    worldToTileCoords(num1, num2, &gridX, &gridY);
    
    if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1)
    {
        return 0.0f;
    }
    
    if (isSolid(levelData[gridY][gridX]))
    {
        float x = (gridX + 1) * 0.1f;
        return x - num1;
    }
    
    return 0.0f;
}

float Platformer::checkYCoordCollision(float num1, float num2)
{
    int gridX;
    int gridY;
    
    worldToTileCoords(num1, num2, &gridX, &gridY);
    
    if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1)
    {
        return 0.0f;
    }
    
    if (isSolid(levelData[gridY][gridX]))
    {
        float y = (gridY) * 0.1f;
        return -num2 - y;
    }
    
    return 0.0f;
}

void Platformer::handleXCollision(Entity *entity)
{
    float penetration = checkXCoordCollision(entity->x - entity->w * 0.5, entity->y);
    if (penetration != 0.0f)
    {
        entity->x += penetration;
        entity->xVelocity = 0.0f;
        entity->collidedLeft = true;
    }
    
    penetration = checkXCoordCollision(entity->x + entity->w * 0.5, entity->y);
    if (penetration != 0.0f)
    {
        entity->x += (penetration - 0.1f);
        entity->xVelocity = 0.0f;
        entity->collidedRight = true;
    }
}

void Platformer::handleYCollision(Entity *entity)
{
    float penetration = checkYCoordCollision(entity->x, entity->y - (entity->h * 0.5));
    if (penetration != 0.0f)
    {
        entity->y += penetration;
        entity->yVelocity = 0.0f;
        entity->collidedBottom = true;
    }
    
    penetration = checkYCoordCollision(entity->x, entity->y + (entity->h * 0.5));
    if (penetration != 0.0f)
    {
        entity->y += (penetration - 0.1f);
        entity->yVelocity = 0.0f;
        entity->collidedTop = true;
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

