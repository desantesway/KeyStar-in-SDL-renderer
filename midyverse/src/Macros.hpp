#ifndef MACROS_H
#define MACROS_H

// GENERAL
#define MAIN_FONT_SIZE 24
#define MAIN_FONT_COLOR (SDL_Color){255, 255, 255, 255}
#define MAIN_FONT_PATH "Liga-Sans/LigaSans-Bold.otf"
#define FONT_PATH "assets/fonts/"

#define ICON_PATH "logo.jpg"
#define ASSETS_IMAGES_PATH "assets/images/"

// SCENE 1

#define SCENE1_BACKGROUND "background.jpg"

#define CAT(a, b) \
    (a + std::string(b)).c_str()

// Converts a string to a char*
#define StrToPtr(str) (str).c_str()

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