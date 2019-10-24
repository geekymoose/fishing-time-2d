#include "gameplay/game_menus.h"

#include "engine/window.h"
#include "engine/sprite.h"
#include "engine/resources.h"

void creditsUpdate(GameApp * _game)
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
    if(glfwGetKey(_game->engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_WELCOME;
    }
    else if(glfwGetKey(_game->engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_WELCOME;
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
    if(glfwGetKey(_game->engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        closeWindow(_game->engine->window);
    }
    else if(glfwGetKey(_game->engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_FISHING;
    }
    else if(glfwGetKey(_game->engine->window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        _game->currentScreen = GAME_SCREEN_CREDITS;
    }
}

void welcomeRender(GameApp * _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = {0.0f, 0.0f};
    const vecf2 scale = {1.0f, 1.0f};
    drawSprite(_game->resources.welcome, center, scale, _game->engine->shaderID);
}

