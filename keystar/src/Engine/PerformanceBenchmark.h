#ifndef PERFORMANCEBENCHMARK_H
#define PERFORMANCEBENCHMARK_H

#include "../Utils/Libs.h"

class PerfBench
{
public:

    PerfBench();
    ~PerfBench();

    void BenchmarkSimulation();
    void BenchmarkRendering(SDL_Renderer* renderer, TTF_Font* mainFont);

    float GetMaxFPS();
    float GetFPS();
    void SetFPS(float fps);

private:
    void CalculateFPS();
    void FPS(SDL_Renderer* renderer, TTF_Font* mainFont);

    float fps; // set to -1 to dehactivate ingame fps
    float maxfps;
    SDL_Texture* FpsTexture;
};

#endif