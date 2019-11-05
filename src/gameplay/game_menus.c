#include "gameplay/game_menus.h"

#include "engine/sprite.h"
#include "engine/resources.h"
#include "engine/inputs.h"


void creditsUpdate(GameApp * _game)
{
    if(isKeyDown(KEY_ESCAPE) == TRUE)
    {
        enterWelcomeScreen(_game);
    }
    else if(isKeyDown(KEY_SPACE) == TRUE)
    {
        enterWelcomeScreen(_game);
    }
}

void creditsRender(GameApp * _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_game->resources.credits, center, scale, _game->engine->shaderID);
}


// -----------------------------------------------------------------------------

void gameoverUpdate(GameApp * _game)
{
    if(isKeyDown(KEY_ESCAPE) == TRUE)
    {
        enterWelcomeScreen(_game);
    }
    else if(isKeyDown(KEY_SPACE) == TRUE)
    {
        enterWelcomeScreen(_game);
    }
}

void gameoverRender(GameApp * _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_game->resources.gameover, center, scale, _game->engine->shaderID);
}


// -----------------------------------------------------------------------------

void welcomeUpdate(GameApp * _game)
{
    if(isKeyDown(KEY_ESCAPE) == TRUE)
    {
        leaveApp(_game);
    }
    else if(isKeyDown(KEY_SPACE) == TRUE)
    {
        enterFishingScreen(_game);
    }
    else if(isKeyDown(KEY_ENTER) == TRUE)
    {
        enterCreditScreen(_game);
    }
}

void welcomeRender(GameApp * _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_game->resources.welcome, center, scale, _game->engine->shaderID);
}

