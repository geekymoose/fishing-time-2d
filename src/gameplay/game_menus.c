#include "gameplay/game_menus.h"

#include "engine/inputs.h"
#include "engine/resources.h"
#include "engine/sprite.h"

void gameoverUpdate(GameApp* _game)
{
    if (isKeyDown(KEY_ESCAPE) == TRUE) {
        enterWelcomeScreen(_game);
    } else if (isKeyDown(KEY_ENTER) == TRUE) {
        enterFishingScreen(_game);
    } else if (isKeyDown(KEY_R) == TRUE) {
        enterFishingScreen(_game);
    }
}

void gameoverRender(GameApp* _game)
{
    // The world center 0:0 is the center of the screen.
    // UI position is in World units (as beeing seen by camera).
    // Positions are hard coded (simply based on the visual results on screen)

    // Background
    const vecf2 scale = { 1.0f, 1.0f };
    vecf2 center = { 0.0f, 0.0f };
    drawSprite(_game->resources.gameover, center, scale, _game->engine->shaderID);

    // Foreground UI
    center.x = 0.0f;
    center.y = 82.5f;
    drawSprite(_game->resources.foreground, center, scale, _game->engine->shaderID);

    // Score
    vecf2 score_position = { -14.0f, GAME_CAMERA_RECT_HEIGHT / 2.1f };

    int score = _game->currentScore;
    int digit_hundred = (score / 100) % 10; // 100 -> gives 1
    int digit_ten = (score / 10) % 10;      // 92 -> gives 9
    int digit_unit = score % 10;            // 92 -> gives 2

    // Score in UI
    drawSprite(_game->resources.fontsBitmap[digit_hundred], score_position, scale, _game->engine->shaderID);
    score_position.x += _game->resources.fontsBitmap[digit_hundred]->size.x;
    drawSprite(_game->resources.fontsBitmap[digit_ten], score_position, scale, _game->engine->shaderID);
    score_position.x += _game->resources.fontsBitmap[digit_ten]->size.x;
    drawSprite(_game->resources.fontsBitmap[digit_unit], score_position, scale, _game->engine->shaderID);

    // Score
    score_position.x = -50;
    score_position.y = -20;
    drawSprite(_game->resources.fontsBitmap[digit_hundred], score_position, scale, _game->engine->shaderID);
    score_position.x += _game->resources.fontsBitmap[digit_hundred]->size.x;
    drawSprite(_game->resources.fontsBitmap[digit_ten], score_position, scale, _game->engine->shaderID);
    score_position.x += _game->resources.fontsBitmap[digit_ten]->size.x;
    drawSprite(_game->resources.fontsBitmap[digit_unit], score_position, scale, _game->engine->shaderID);

    // Best score
    score = _game->bestScore;
    digit_hundred = (score / 100) % 10; // 100 -> gives 1
    digit_ten = (score / 10) % 10;      // 92 -> gives 9
    digit_unit = score % 10;            // 92 -> gives 2

    score_position.x = 50;
    score_position.y = -20;
    drawSprite(_game->resources.fontsBitmap[digit_hundred], score_position, scale, _game->engine->shaderID);
    score_position.x += _game->resources.fontsBitmap[digit_hundred]->size.x;
    drawSprite(_game->resources.fontsBitmap[digit_ten], score_position, scale, _game->engine->shaderID);
    score_position.x += _game->resources.fontsBitmap[digit_ten]->size.x;
    drawSprite(_game->resources.fontsBitmap[digit_unit], score_position, scale, _game->engine->shaderID);
}

void welcomeUpdate(GameApp* _game)
{
    if (isKeyDown(KEY_ESCAPE) == TRUE) {
        leaveApp(_game);
    } else if (isKeyDown(KEY_ENTER) == TRUE) {
        enterFishingScreen(_game);
    } else if (isKeyDown(KEY_R) == TRUE) {
        enterFishingScreen(_game);
    }
}

void welcomeRender(GameApp* _game)
{
    // The world center 0:0 is the center of the screen.
    const vecf2 center = { 0.0f, 0.0f };
    const vecf2 scale = { 1.0f, 1.0f };
    drawSprite(_game->resources.welcome, center, scale, _game->engine->shaderID);
}
