#include "PerformanceBenchmark.h"

PerfBench::PerfBench() {
    this->fps = -1.0f;
    this->maxfps = 240.0f;
    
    this->FpsTexture = NULL;
}

PerfBench::~PerfBench() {
    if (this->FpsTexture) {
        SDL_DestroyTexture(this->FpsTexture);
        this->FpsTexture = NULL;
    }
}

void PerfBench::SetFPS(float fps) { this->fps = fps; }
float PerfBench::GetFPS() { return this->fps; }
float PerfBench::GetMaxFPS() { return this->maxfps; }

// calcutes every benchmark in the background
void PerfBench::BenchmarkSimulation() {
    if (GetFPS() != -1.0f) {
        CalculateFPS();
    }
}

// This function renders the benchmarks
void PerfBench::BenchmarkRendering(SDL_Renderer* renderer, TTF_Font* mainFont) {
    if (GetFPS() != -1.0f) {
        FPS(renderer, mainFont);
    }
    else {
        if (this->FpsTexture) {
            SDL_DestroyTexture(this->FpsTexture);
            this->FpsTexture = NULL;
        }
    }
}

// This function sets and shows the FPS of the program.
void PerfBench::FPS(SDL_Renderer* renderer, TTF_Font* mainFont) {
    
        static int frameCount = 0;
        frameCount++;

        static SDL_FRect fps_rect;

        if (frameCount == 100) {

            if (this->FpsTexture) {
                SDL_DestroyTexture(this->FpsTexture);
                this->FpsTexture = NULL;
            }

            SDL_Surface* fps_surface = TTF_RenderText_Blended(mainFont, CAT("FPS: ", std::to_string((int)this->GetFPS())), 0, SDL_COLOR_FPS);
            VOID_CHECK_RESULT(fps_surface, "Error rendering FPS surface: ", SDL_GetError());

            if (fps_surface) {
                fps_rect.x = 5.0f;
                fps_rect.w = (float)fps_surface->w;
                fps_rect.h = (float)fps_surface->h;
            }
            this->FpsTexture = SDL_CreateTextureFromSurface(renderer, fps_surface);

            SDL_DestroySurface(fps_surface);
            fps_surface = NULL;

            VOID_CHECK_RESULT(this->FpsTexture, "Error rendering FPS texture: ", SDL_GetError());
            //VOID_CHECK_RESULT(SDL_SetTextureScaleMode(this->FpsTexture, SDL_SCALEMODE_NEAREST), "Error setting FPS texture scale mode: ", SDL_GetError());

            frameCount = 0;
        }

        if (this->FpsTexture) {
            SDL_RenderTexture(renderer, this->FpsTexture, NULL, &fps_rect); // Render the FPS texture
        }
    
}

// This function calculates the FPS of the program.
void PerfBench::CalculateFPS() {
    static const int NUM_SAMPLES = 100;
    static float fps_samples[NUM_SAMPLES];
    static int currentFrame = 0;

    static float prevTick = (float)SDL_GetTicks();
    float currentTick;
    currentTick = (float)SDL_GetTicks();

    float frameTime = currentTick - prevTick;

    if (currentFrame >= NUM_SAMPLES) {
        currentFrame = 0;
    }
    fps_samples[currentFrame] = frameTime;


    int count = std::min(currentFrame, NUM_SAMPLES);

    float frameAverage = 0;
    for (int i = 0; i < count; i++) {
        frameAverage += fps_samples[i];
    }
    frameAverage /= count;

    if (frameAverage > 0) {
        this->fps = 1000.0f / frameAverage;
    }
    else {
        this->fps = 0.0f;
    }

    prevTick = currentTick;
    currentFrame++;
}