#include "gameplay/welcome.h"

#include "engine/libmath.h"
#include "engine/texture.h"
#include "engine/sprite.h"


void welcomeUpdate(Game * _game)
{
    // Quit game
    if(glfwGetKey(_game->engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        closeWindow(_game->engine->window);
    }

    if(glfwGetKey(_game->engine->window, GLFW_KEY_SPACE) == GLFW_PRESS)
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
    //drawSprite(_sprite, center, scale, _shaderID);

    // Texture makeTexture(const char* _path);
    // Sprite makeSprite(Texture * _texture, int _width, int _height, vecf2 _origin);
}
