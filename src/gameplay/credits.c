#include "gameplay/credits.h"

#include "build_config.h"
#include "engine/window.h"
#include "engine/resources.h"


Sprite * s_creditsSprite = NULL;


void creditsInit(Game * _game)
{
    vecf2 origin = {0.0f, 0.0f};
    unsigned int texID = resourceLoadTexture(GAME_RESOURCES_DIR "/placeholder.png");
    unsigned int spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);

    s_creditsSprite = resourceGetSprite(spriteID);
}

void creditsDestroy(Game * _game)
{
    // Nothing to do
}

void creditsUpdate(Game * _game)
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

void creditsFixedUpdate(Game * _game)
{
    // Nothing to do
}

void creditsRender(Game * _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(s_creditsSprite, center, scale, _game->engine->shaderID);
}
