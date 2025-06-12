#ifndef MACROS_H
#define MACROS_H

// GENERAL
#define NAME "KeyStar"

#define GREEN 88, 204, 2
#define DARKGREEN 88, 167, 0
#define ASSESTS_RES 4320

#define MAIN_FONT_SIZE 48
#define MAIN_FONT_PATH "Liga-Sans/LigaSans-Bold.otf"
#define FONT_PATH "assets/fonts/"

#define SDL_COLOR_FPS { 200, 200, 200, 255 }

#define ICON_PATH "icon.png"
#define ASSETS_IMAGES_PATH "assets/images/"

// PIANO
#define MIDI_NAME "KeyStar Output"
#define PIANO_FOLDER_PATH "piano/"
#define RWHITE_PATH "rwhite.png"
#define LWHITE_PATH "lwhite.png"
#define WHITE_MID_PATH "midwhite.png"
#define WHITE_ROUNDMID_PATH "roundwhite.png"
#define WHITE_SHADOW_PATH "whiteshadow.png"
#define BLACK_KEY_PATH "black.png"
#define BLACK_SHADOW_PATH "blackshadow.png"
#define BLACK_BLEND_PATH "blackblend.png"

// this is info is taken from the illustrator project that measures the pixels of the keys in 8k
#define WHITE_SHADOW_HEIGHT (83.0)
#define BLACK_SHADOW_HEIGHT (96.0) // getting when the black key height starts
#define WHITEB_KEY_HEIGHT (290.0) // getting when the black shadow key height starts

// DEBUG/RELEASE

// SCENE 1

#define SCENE1_BACKGROUND "main_menu/4320p_background.png"

#define CAT(a, b) \
    (a + std::string(b)).c_str()

// Converts a string to a char*
#define StrToPtr(str) (str).c_str()

// Check for errors
// IF INPUT TRUE -> DOESNT RAISE AN ERROR
#define CHECK_RESULT_RET(expr, error_msg, ...) \
    if (!(expr)) { \
        __VA_ARGS__; \
        return false; \
    }

// Check for SDL errors
// IF INPUT TRUE -> DOESNT RAISE AN ERROR
#define CHECK_RESULT(expr, error_msg, ...) \
    if (!(expr)) { \
        SDL_Log("%s %s", error_msg, SDL_GetError()); \
        __VA_ARGS__; \
        return false; \
    }

// Above function but for void
#define VOID_CHECK_RESULT(expr, error_msg, ...) \
    if (!(expr)) { \
        SDL_Log("%s %s", error_msg, SDL_GetError()); \
        __VA_ARGS__; \
    }

#define PRINT(msg) \
    SDL_Log(msg); \

#define PRINTI(msg) \
    SDL_Log("%i",msg); \

#endif