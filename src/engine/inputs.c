#include "engine/inputs.h"

#include <string.h> // memset

// "state[KEY] & 0x4" == 1 if down during this frame
// "state[KEY] & 0x2" == 1 if up during this frame
// "state[KEY] & 0x1" == 1 if currently pressed
static int8 keyboardState[KEYS_COUNT] = { 0 };

typedef enum
{
    _KEY_DOWN_BITMASK = 0x4,    // Bitmask 0b100
    _KEY_UP_BITMASK = 0x2,      // Bitmask 0b010
    _KEY_PRESSED_BITMASK = 0x1, // Bitmask 0b001
} _KEY_BIT_STATUS;

void updateInputs()
{
    memset(keyboardState, 0, sizeof(keyboardState));
    glfwPollEvents();
}

void handleKeyEvents(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
    int keymap = 0;
    switch (_key) {
        case GLFW_KEY_SPACE:
            keymap = KEY_SPACE;
            break;
        case GLFW_KEY_0:
            keymap = KEY_0;
            break;
        case GLFW_KEY_1:
            keymap = KEY_1;
            break;
        case GLFW_KEY_2:
            keymap = KEY_2;
            break;
        case GLFW_KEY_3:
            keymap = KEY_3;
            break;
        case GLFW_KEY_4:
            keymap = KEY_4;
            break;
        case GLFW_KEY_5:
            keymap = KEY_5;
            break;
        case GLFW_KEY_6:
            keymap = KEY_6;
            break;
        case GLFW_KEY_7:
            keymap = KEY_7;
            break;
        case GLFW_KEY_8:
            keymap = KEY_8;
            break;
        case GLFW_KEY_9:
            keymap = KEY_9;
            break;

        case GLFW_KEY_A:
            keymap = KEY_A;
            break;
        case GLFW_KEY_B:
            keymap = KEY_B;
            break;
        case GLFW_KEY_C:
            keymap = KEY_C;
            break;
        case GLFW_KEY_D:
            keymap = KEY_D;
            break;
        case GLFW_KEY_E:
            keymap = KEY_E;
            break;
        case GLFW_KEY_F:
            keymap = KEY_F;
            break;
        case GLFW_KEY_G:
            keymap = KEY_G;
            break;
        case GLFW_KEY_H:
            keymap = KEY_H;
            break;
        case GLFW_KEY_I:
            keymap = KEY_I;
            break;
        case GLFW_KEY_J:
            keymap = KEY_J;
            break;
        case GLFW_KEY_K:
            keymap = KEY_K;
            break;
        case GLFW_KEY_L:
            keymap = KEY_L;
            break;
        case GLFW_KEY_M:
            keymap = KEY_M;
            break;
        case GLFW_KEY_N:
            keymap = KEY_N;
            break;
        case GLFW_KEY_O:
            keymap = KEY_O;
            break;
        case GLFW_KEY_P:
            keymap = KEY_P;
            break;
        case GLFW_KEY_Q:
            keymap = KEY_Q;
            break;
        case GLFW_KEY_R:
            keymap = KEY_R;
            break;
        case GLFW_KEY_S:
            keymap = KEY_S;
            break;
        case GLFW_KEY_T:
            keymap = KEY_T;
            break;
        case GLFW_KEY_U:
            keymap = KEY_U;
            break;
        case GLFW_KEY_V:
            keymap = KEY_V;
            break;
        case GLFW_KEY_W:
            keymap = KEY_W;
            break;
        case GLFW_KEY_X:
            keymap = KEY_X;
            break;
        case GLFW_KEY_Y:
            keymap = KEY_Y;
            break;
        case GLFW_KEY_Z:
            keymap = KEY_Z;
            break;

        case GLFW_KEY_F1:
            keymap = KEY_F1;
            break;
        case GLFW_KEY_F2:
            keymap = KEY_F2;
            break;
        case GLFW_KEY_F3:
            keymap = KEY_F3;
            break;
        case GLFW_KEY_F4:
            keymap = KEY_F4;
            break;
        case GLFW_KEY_F5:
            keymap = KEY_F5;
            break;
        case GLFW_KEY_F6:
            keymap = KEY_F6;
            break;
        case GLFW_KEY_F7:
            keymap = KEY_F7;
            break;
        case GLFW_KEY_F8:
            keymap = KEY_F8;
            break;
        case GLFW_KEY_F9:
            keymap = KEY_F9;
            break;
        case GLFW_KEY_F10:
            keymap = KEY_F10;
            break;
        case GLFW_KEY_F11:
            keymap = KEY_F11;
            break;
        case GLFW_KEY_F12:
            keymap = KEY_F12;
            break;

        case GLFW_KEY_RIGHT:
            keymap = KEY_RIGHT;
            break;
        case GLFW_KEY_LEFT:
            keymap = KEY_LEFT;
            break;
        case GLFW_KEY_DOWN:
            keymap = KEY_DOWN;
            break;
        case GLFW_KEY_UP:
            keymap = KEY_UP;
            break;

        case GLFW_KEY_ESCAPE:
            keymap = KEY_ESCAPE;
            break;
        case GLFW_KEY_ENTER:
            keymap = KEY_ENTER;
            break;
        case GLFW_KEY_TAB:
            keymap = KEY_TAB;
            break;
        case GLFW_KEY_BACKSPACE:
            keymap = KEY_BACKSPACE;
            break;
        case GLFW_KEY_INSERT:
            keymap = KEY_INSERT;
            break;
        case GLFW_KEY_DELETE:
            keymap = KEY_DELETE;
            break;

        case GLFW_KEY_HOME:
            keymap = KEY_HOME;
            break;
        case GLFW_KEY_PAUSE:
            keymap = KEY_PAUSE;
            break;

        default:
            // Means the GLFW key has no support in this engine
            break;
    }

    if (_action == GLFW_PRESS) {
        keyboardState[keymap] = 0x5;
    } else if (_action == GLFW_RELEASE) {
        keyboardState[keymap] = 0x2;
    } else if (_action == GLFW_REPEAT) {
        keyboardState[keymap] = 0x1;
    }
}

int isKeyDown(KEY_KEYBOARD _key)
{
    if ((keyboardState[_key] & _KEY_DOWN_BITMASK) != 0) {
        return TRUE;
    }
    return FALSE;
}

int isKeyUp(KEY_KEYBOARD _key)
{
    if ((keyboardState[_key] & _KEY_UP_BITMASK) != 0) {
        return TRUE;
    }
    return FALSE;
}

int isKeyPressed(KEY_KEYBOARD _key)
{
    if ((keyboardState[_key] & _KEY_PRESSED_BITMASK) != 0) {
        return TRUE;
    }
    return FALSE;
}
