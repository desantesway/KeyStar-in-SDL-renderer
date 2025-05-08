#ifndef MACROS_H
#define MACROS_H

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