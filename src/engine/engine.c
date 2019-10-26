#include "engine/engine.h"

#include "engine/types.h"
#include "engine/log.h"


int engineRun(Engine * _engine, void * _game)
{
    ASSERT_MSG(_engine != NULL, "[Engine] parameter should not be NULL");
    ASSERT_MSG(_game != NULL, "[Engine] parameter should not be NULL");

    // Init

    _engine->timescale = 1.0f;

    _engine->window = createWindow(600, 600, "Shark engine 2D"); // Default init values
    if(_engine->window == NULL)
    {
        LOG_ERR("[Engine] Failed to create the main window");
        ASSERT_MSG(FALSE, "[Engine] Failed to create the main window");
        return 42;
    }

    glfwSwapInterval(1); // vsync

    float timeBeginInSec = glfwGetTime();
    float timeEndInSec = 0.0f;

    float dt = 1.0f / 60.0f; // Initial dt at first frame (60 fps)

    float elapsedFixedDeltaTime = 0.0f;
    const float fixedDeltaTime = 1.0f / 45.0f; // Physic capped at 45 fps

    int success = _engine->gameInit(_game);
    if(success != 0)
    {
        LOG_ERR("[Engine] The engine failed to init the game");
        ASSERT_MSG(FALSE, "[Engine] The engine failed to init the game");
        return 42;
    }

    // Main Loop

    while(!isWindowClosed(_engine->window))
    {
        // const int fps = (int)(1.0f / dt);
        // LOG_DBG("dt = %f, fps = %d (fixed dt = %f)", dt, fps, fixedDeltaTime);

        clearWindow(_engine->window);

        dt = dt * _engine->timescale;

        elapsedFixedDeltaTime += dt;
        if(elapsedFixedDeltaTime >= fixedDeltaTime)
        {
            // DevNotes: in case of long render loop (high dt), we don't try to
            // recover the elapsed physics steps and only play one step instead.
            elapsedFixedDeltaTime = 0.0f;
            _engine->gameFixedUpdate(_game, fixedDeltaTime);
        }

        _engine->gameUpdate(_game, dt);
        _engine->gameRender(_game);

        swapWindow(_engine->window);

        glfwPollEvents();

        timeEndInSec = glfwGetTime();
        dt = timeEndInSec - timeBeginInSec;
        timeBeginInSec = timeEndInSec;
    }

    // Shutdown

    destroyWindow(_engine->window); // First, remove the window to look responsive

    success = _engine->gameDestroy(_game);
    if(success != 0)
    {
        LOG_ERR("[Engine] The engine failed to destroy the game");
        ASSERT_MSG(FALSE, "[Engine] The engine failed to destroy the game");
        return 42;
    }

    return 0;
}
