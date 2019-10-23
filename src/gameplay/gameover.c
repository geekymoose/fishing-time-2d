#include "gameplay/gameover.h"

#include "build_config.h"
#include "engine/window.h"
#include "engine/resources.h"


Sprite * s_gameoverSprite = NULL;


void gameoverInit(GameApp * _game)
{
    vecf2 origin = {0.0f, 0.0f};
    unsigned int texID = resourceLoadTexture(GAME_RESOURCES_DIR "/placeholder.png");
    unsigned int spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);

    s_gameoverSprite = resourceGetSprite(spriteID);
}

void gameoverDestroy(GameApp * _game)
{
    // Nothing to do
}

void gameoverUpdate(GameApp * _game)
{
    if(glfwGetKey(_game->engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_WELCOME;
    }
    else if(glfwGetKey(_game->engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_WELCOME;
    }
}

void gameoverFixedUpdate(GameApp * _game)
{
    // Nothing to do
}

void gameoverRender(GameApp * _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(s_gameoverSprite, center, scale, _game->engine->shaderID);
}
