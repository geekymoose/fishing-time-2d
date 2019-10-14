#include "gameplay/welcome.h"

#include "build_config.h"
#include "engine/window.h"
#include "engine/resources.h"

Sprite * s_welcomeSprite = NULL;


void welcomeInit(Game * _game)
{
    vecf2 origin = {0.0f, 0.0f};
    unsigned int texID = resourceLoadTexture(GAME_RESOURCES_DIR "/welcome.png");
    unsigned int spriteID = resourceLoadSprite(resourceGetTexture(texID), 200, 200, origin);

    s_welcomeSprite = resourceGetSprite(spriteID);
}

void welcomeDestroy(Game * _game)
{
}

void welcomeUpdate(Game * _game)
{
    // Quit game
    if(glfwGetKey(_game->engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        closeWindow(_game->engine->window);
    }

    if(glfwGetKey(_game->engine->window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_FISHING;
    }
}

void welcomeFixedUpdate(Game * _game)
{
    // Nothing to do
}

void welcomeRender(Game * _game)
{
    // The world center 0:0 is the center of the screen.

    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(s_welcomeSprite, center, scale, _game->engine->shaderID);
}
