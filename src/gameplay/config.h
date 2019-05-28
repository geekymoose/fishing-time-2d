#pragma once


// -----------------------------------------------------------------------------
// Core
// -----------------------------------------------------------------------------

#define GAME_WINDOW_WIDTH  800
#define GAME_WINDOW_HEIGHT 800
#define GAME_WINDOW_TITLE  "Raining Sharks"

// Rec of vision of the camera, in world units
#define GAME_CAMERA_RECT_WIDTH 200.0f
#define GAME_CAMERA_RECT_HEIGHT 200.0f


// -----------------------------------------------------------------------------
// Anims
// -----------------------------------------------------------------------------

// Durations are the time that one frame lasts.
#define GAME_BOAT_ANIM_FRAME_DURATION_IN_SEC  0.250f
#define GAME_BOAT_ANIM_NB_FRAMES 4

#define GAME_EXPLOSION_ANIM_FRAME_DURATION_IN_SEC 0.150f
#define GAME_EXPLOSION_ANIM_NB_FRAMES 3

#define GAME_SHARK_ANIM_FRAME_DURATION_IN_SEC 0.250f
#define GAME_SHARK_ANIM_NB_FRAMES 3


// -----------------------------------------------------------------------------
// Gameplay
// -----------------------------------------------------------------------------

#define GAME_NB_MAX_SHARKS 15 // Max number of sharks simultaneously present

// In practice, speeds are in pixel per seconds
#define GAME_BOAT_SPEED 100.0f
#define GAME_SHARK_SPEED 100.0f
#define GAME_ANCHOR_SPEED 400.0f

