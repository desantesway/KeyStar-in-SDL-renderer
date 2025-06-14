#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "../Utils/Libs.h"

class Animations
{
public:

    Animations();
    ~Animations();

    enum class AnimationCurve {
        Linear,
        EaseIn,
        EaseOut,
        EaseInOut,
        EaseOutBounce
    };

    void StartAnimation(std::string name, double duration, AnimationCurve curve);
    void ReverseAnimation(std::string name, AnimationCurve curve);
    double ApplyCurve(double t, AnimationCurve curve);
    float AnimationState(std::string name);
    void Animate(float delta);

private:

    struct Animation {
        bool direction; // true for forward, false for reverse
        double duration;    // total duration of animation in seconds
        double progress;    // current progress [0, 1]
        double elapsed;     // time elapsed in seconds
        bool finished;      // whether animation is complete
        AnimationCurve curve;
    };

    std::map<std::string, Animation> animationStates;
    std::vector<std::string> reversedAnimations;
};
#endif
