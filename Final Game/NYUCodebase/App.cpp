//
//  App.cpp
//  NYUCodebase
//
//  Created by fp on 4/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "App.h"

#define SPRITE_COUNT_X 12
#define SPRITE_COUNT_Y 13
#define TILE_SIZE 0.2f
#define GRAVITY -4

enum gameStates{ MAIN_MENU = 1, GAME_LEVEL = 2, GAME_OVER = 3};

GLuint App::loadTexture(const char* imagePath)
{
    SDL_Surface *surface = IMG_Load(imagePath);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void App::drawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y)
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

App::App()
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Jumping Boy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);

    glMatrixMode(GL_PROJECTION);
    //glOrtho(-1.58, 3.66, -10.5, 0.5, -1.0, 1.0);
    glOrtho(-1.33 * 2, 1.33 * 2, -1.0 * 2, 1.0 * 2, -1.0, 1.0);
   // glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    Init();
}

App::~App()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        delete entities[i];
    }
    for (size_t i = 0; i < enemies.size(); i++)
    {
        delete enemies[i];
    }
    SDL_Quit();
}

void App::loadMap()
{
    ifstream infile("tileMap.txt");
    string line;
    while (getline(infile, line))
    {
        if(line == "[header]")
        {
            cout << "read header" << endl;
            if(!readHeader(infile))
            {
                return;
            }
        }
        else if(line == "[layer]")
        {
            cout << "read layer" << endl;
            readLayerData(infile);
            for (int i = 0; i < mapHeight; i++)
            {
                for (int j = 0; j < mapWidth; j++)
                {
                    cout << (int)levelData[i][j] << " ";
                }
                cout << endl;
            }

        }
        else if(line == "[Player]" || line == "[Enemies]")
        {
            cout << "read entity" << endl;
            readEntityData(infile);
        }
    }
}

bool App::readHeader(ifstream &stream)
{
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
        levelData = new unsigned char *[mapHeight];
        for(int i = 0; i < mapHeight; ++i)
        {
            levelData[i] = new unsigned char[mapWidth];
        }
        return true;
    }
}

bool App::readLayerData(ifstream &stream)
{
    string line;
    while (getline(stream, line)) {
        if (line == "")
        {
            break;
        }
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if (key == "data") {
            for (int y = 0; y < mapHeight; y++) {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                for (int x = 0; x < mapWidth; x++) {
                    getline(lineStream, tile, ',');
                    unsigned char val = (unsigned char)atoi(tile.c_str());
                    if ((int)val > 0) {
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

bool App::readEntityData(ifstream &stream)
{
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
            float placeX = atoi(xPos.c_str()) / 70 * TILE_SIZE;
            float placeY = atoi(yPos.c_str()) / 70 *- TILE_SIZE;
            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}

void App::placeEntity(string type, float placeX, float placeY)
{
    if (type == "PlayerPos")
    {
        cout << "placed player at" << placeX << "," << placeY << endl;
        player->x = placeX;
        player->y = placeY;
    }
    else if (type == "enemies")
    {
        cout << "placed enemy" << endl;
        Entity *enemy = new Entity();
        enemy->textureID = enemyTexture;
        enemy->u = 0.0f/353;
        enemy->v = 32.0f/153;
        enemy->w = 72.0f/353;
        enemy->h = 36.0f/150;
        enemy->x = placeX;
        enemy->y = placeY;
        enemy->width = 0.72f;
        enemy->height = 0.36f;
        enemies.push_back(enemy);
    }
}

void App::Init()
{
    state = MAIN_MENU;
    
    playerTexture = loadTexture("p1_spritesheet.png");
    enemyTexture = loadTexture("enemies_spritesheet.png");
    tilesTexture = loadTexture("tiles_spritesheet.png");
    fontTexture = loadTexture("pixel_font.png");
    
    startedGame = Mix_LoadWAV("startedGame.wav");
    jumpSound = Mix_LoadWAV("jumpSound.wav");
    jumpedOnEnemy = Mix_LoadWAV("jumpedOn.wav");
    gotInjured = Mix_LoadWAV("injured.wav");
    gameWonMusic = Mix_LoadMUS("gameWon.mp3");
    gameLevelMusic = Mix_LoadMUS("gamePlaying.mp3");
    gameOverMusic = Mix_LoadMUS("gameOver.mp3");
    
    player = new Entity();
    player->textureID = playerTexture;
    player->u = 0.0f/508.0f;
    player->v = 196.0f/288.0f;
    player->w = 66.0f/508.0f;
    player->h = 92.0f/288.0f;
    player->width = 0.66f;
    player->height = 0.92f;
    player->xFriction = 0.2f;
    player->yFriction = 0.3f;
    entities.push_back(player);
    
    loadMap();
    
    Mix_PlayMusic(gameLevelMusic, -1);
}

void App::update(float elapsed)
{
    switch(state)
    {
        case MAIN_MENU:
            updateMainMenu(elapsed);
            break;
        case GAME_LEVEL:
            updateGameLevel(elapsed);
            break;
        case GAME_OVER:
            updateGameOver(elapsed);
            break;
    }
}

void App::updateMainMenu(float elapsed)
{
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
        else if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
                state = GAME_LEVEL;
            }
        }
    }
}

void App::updateGameLevel(float elapsed)
{
    animateTime += elapsed;
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            bool walking = false;
            if (((event.key.keysym.scancode == SDL_SCANCODE_UP) || (event.key.keysym.scancode == SDL_SCANCODE_W) || (event.key.keysym.scancode == SDL_SCANCODE_SPACE)) && player->collidedBottom)
            {
                cout << "just jumped" << endl;
                player->yVelocity = 3.0f;
                Mix_PlayChannel(-1, jumpSound, 0);
            }
            if ((event.key.keysym.scancode == SDL_SCANCODE_RIGHT) || (event.key.keysym.scancode == SDL_SCANCODE_D))
            {
                cout << "moved right" << endl;
                player->xAccel = 5.0f;
                walking = true;
            }
            else if ((event.key.keysym.scancode == SDL_SCANCODE_LEFT) || (event.key.keysym.scancode == SDL_SCANCODE_A))
            {
                cout << "moved left" << endl;
                player->xAccel = -5.0f;
                walking = true;
            }
            else
            {
                player->xAccel = 0.0f;
            }
            animatePlayer(walking);
            animateEnemies(elapsed);
            if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                state = GAME_OVER;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                reachedDoor = true;
                state = GAME_OVER;
            }
        }
    }
    if (player->y < -0.4f)
    {
        state = GAME_OVER;
    }
}

void App::updateGameOver(float elapsed)
{
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
        else if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat)
            {
                state = MAIN_MENU;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_1)
            {
                SDL_Quit();
            }
        }
    }
}

void App::fixedUpdate()
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->collidedTop = false;
        entities[i]->collidedBottom = false;
        entities[i]->collidedLeft = false;
        entities[i]->collidedRight = false;
        if (!entities[i]->collidedBottom)
        {
            entities[i]->yAccel = GRAVITY;
        }
        entities[i]->update();
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
    
    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i]->collidedTop = false;
        enemies[i]->collidedBottom = false;
        enemies[i]->collidedRight = false;
        enemies[i]->collidedLeft = false;
        enemies[i]->xVelocity = -2.0f;
        handleXCollision(enemies[i]);
        handleYCollision(enemies[i]);
        if (enemies[i]->collidedLeft)
        {
            enemies[i]->xVelocity = 2.0f;
        }
        if (enemies[i]->collidedRight)
        {
            enemies[i]->xVelocity = -2.0f;
        }
        enemies[i]->update();
    }
    
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (player->collidedWith(enemies[i]))
        {
            if (enemies[i]->collidedTop && player->collidedBottom)
            {
                cout << "jump really high" << endl;
                player->yVelocity = 10.0f;
                Mix_PlayChannel(-1, jumpedOnEnemy, 0);
            }
            if (player->collidedLeft || player->collidedRight ||player->collidedTop)
            {
                Mix_PlayChannel(-1, gotInjured, 0);
                state = GAME_OVER;
            }
        }
    }
}

void App::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    switch(state)
    {
        case MAIN_MENU:
            renderMainMenu();
            break;
        case GAME_LEVEL:
            renderGameLevel();
            break;
        case GAME_OVER:
            renderGameOver();
            break;
    }
    SDL_GL_SwapWindow(displayWindow);
}

void App::renderMainMenu()
{
    glLoadIdentity();
    glTranslatef(-TILE_SIZE * mapWidth/2, TILE_SIZE * mapHeight/2, 0.0f);
    drawText(fontTexture, "Jumping Boy! Press Enter to Play", 0.2, -0.01f, 0.0f, 1.0f, 1.0f, 0.3f, -1.5f, 0.3);
}

void App::renderGameLevel()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    renderTiles();
    
    glLoadIdentity();
    glTranslatef(-TILE_SIZE * mapWidth/2, TILE_SIZE * mapHeight/2, 0.0f);
    
    player->render(0.1f);
    
    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i]->render(0.2f);
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void App::renderGameOver()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (reachedDoor)
    {
        drawText(fontTexture, "You WIN!", 0.3, -0.01, 0.0, 1.0, 1.0, 0.3, -1.5, 0.0);
        drawText(fontTexture, "ENTER to play again, 1 to quit", 0.1, -0.01, 0.0, 1.0, 1.0, 0.3, -1.5, -0.3);
    }
    else
    {
        drawText(fontTexture, "You LOSE!", 0.3, -0.01, 0.0, 1.0, 1.0, 0.3, -1.5, 0.0);
        drawText(fontTexture, "ENTER to try again, 1 to quit", 0.1, -0.01, 0.0, 1.0, 1.0, 0.3, -1.5, -0.3);
    }
    SDL_GL_SwapWindow(displayWindow);
    glPopMatrix();
}

void App::renderTiles()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tilesTexture);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
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
                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x, -TILE_SIZE * y,
                    TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
                });
                texCoordData.insert(texCoordData.end(), { u, v,
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
    glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void App::animatePlayer(bool walking)
{
    if (abs(player->xVelocity) < 0.01f && abs(player->yVelocity) < 0.01f && !player->collidedTop)
    {
        player->u = 0.0f / 508.0f;
        player->v = 196.0f / 288.0f;
        player->w = 66.0f / 508.0f;
        player->h = 92.0f / 288.0f;
        player->width = 0.66f;
        player->height = 0.92f;
    }
    else if (player->yVelocity != 0.0f && !player->collidedBottom)
    {
        player->u = 438.0f / 508.0f;
        player->v = 93.0f / 288.0f;
        player->w = 67.0f / 508.0f;
        player->h = 94.0f / 288.0f;
        player->width = 0.67f;
        player->height = 0.94f;
        walking = false;
    }
    else
    {
        player->u = 67.0f / 508.0f;
        player->v = 196.0f / 288.0f;
        player->w = 66.0f / 508.0f;
        player->h = 92.0f / 288.0f;
        player->width = 0.66f;
        player->height = 0.92f;
    }
    
    if (walking)
    {
        player->w = 72.0f / 508.0f;
        player->h = 97.0f / 288.0f;
        player->width = 0.72f;
        player->height = 0.97f;
        float animationValue = fmod(animateTime, 1.1f);
        if (animationValue >= 0.0f && animationValue < 0.1f)
        {
            player->u = 0.0f / 508.0f; player->v = 0.0f / 288.0f;
        }
        else if (animationValue >= 0.1f && animationValue < 0.2f)
        {
            player->u = 73.0f / 508.0f; player->v = 0.0f / 288.0f;
        }
        else if (animationValue >= 0.2f && animationValue < 0.3f)
        {
            player->u = 146.0f / 508.0f; player->v = 0.0f / 288.0f;
        }
        else if (animationValue >= 0.3f && animationValue < 0.4f)
        {
            player->u = 0.0f / 508.0f; player->v = 98.0f / 288.0f;
        }
        else if (animationValue >= 0.4f && animationValue < 0.5f)
        {
            player->u = 73.0f / 508.0f; player->v = 98.0f / 288.0f;
        }
        else if (animationValue >= 0.5f && animationValue < 0.6f)
        {
            player->u = 146.0f / 508.0f; player->v = 98.0f / 288.0f;
        }
        else if (animationValue >= 0.6f && animationValue < 0.7f)
        {
            player->u = 219.0f / 508.0f; player->v = 0.0f / 288.0f;
        }
        else if (animationValue >= 0.7f && animationValue < 0.8f)
        {
            player->u = 292.0f / 508.0f; player->v = 0.0f / 288.0f;
        }
        else if (animationValue >= 0.8f && animationValue < 0.9f)
        {
            player->u = 219.0f / 508.0f; player->v = 98.0f / 288.0f;
        }
        else if (animationValue >= 0.9f && animationValue < 1.0f)
        {
            player->u = 365.0f / 508.0f; player->v = 0.0f / 288.0f;
        }
        else if (animationValue >= 1.0f && animationValue < 1.1f)
        {
            player->u = 292.0f / 508.0f; player->v = 98.0f / 288.0f;
        }
    }
}

void App::animateEnemies(float elapsed)
{
    float animationValue = fmod(animateTime, 0.2f);
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (animationValue >= 0.0f && animationValue < 0.1f)
        {
            enemies[i]->width = 0.72f;
            enemies[i]->height = 0.36f;
            enemies[i]->u = 0.0/353.0f;
            enemies[i]->v = 32.0/153.0f;
            enemies[i]->w = 72.0/353.0f;
            enemies[i]->h = 36.0/153.0f;
        }
        else if (animationValue >= 0.1f && animationValue < 0.2f)
        {
            enemies[i]->width = 0.75f;
            enemies[i]->height = 0.31f;
            enemies[i]->u = 0.0/353.0f;
            enemies[i]->v = 0.0/153.0f;
            enemies[i]->w = 75.0/353.0f;
            enemies[i]->h = 31.0/153.0f;
        }
    }
}

bool App::updateAndRender()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + timeLeftOver;
    if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP) {
        fixedElapsed -= FIXED_TIMESTEP;
        fixedUpdate();
    }
    timeLeftOver = fixedElapsed;
    
    update(elapsed);
    render();
    return done;
}

void App::worldToTileCoords(float worldX, float worldY, int* gridX, int* gridY)
{
    *gridX = (int)(worldX / 0.1f);
    *gridY = (int)(-worldY / 0.1f);
}


bool App::isSolid(unsigned char tile)
{
    switch (tile)
    {
        case 44:
            return true;
            break;
        default:
            return false;
            break;
    }

}

float App::checkXCoordCollision(float num1, float num2)
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
        float x = (gridX + 1) * TILE_SIZE;
        return x - num1;
    }
    
    return 0.0f;
}

float App::checkYCoordCollision(float num1, float num2)
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

void App::handleXCollision(Entity *entity)
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

void App::handleYCollision(Entity *entity)
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