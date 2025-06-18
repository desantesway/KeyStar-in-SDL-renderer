#include <cstdint>
#include <SDL3/SDL.h>  

struct Clock
{
    Uint64 last_tick_time = 0;
    float delta = 0;

    void tick()
    {
        Uint64 tick_time = SDL_GetTicks();
        delta = (tick_time - last_tick_time) / 1000.0f;
        last_tick_time = tick_time;
    }
};