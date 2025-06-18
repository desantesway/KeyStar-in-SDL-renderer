
#include "Animations.h"
#define _USE_MATH_DEFINES
#include <math.h>

Animations::~Animations() {
	this->animationStates.clear();
	this->reversedAnimations.clear();
}

Animations::Animations() {

}

// Start an animation with a name, duration, and curve type
void Animations::StartAnimation(std::string name, double duration, Animations::AnimationCurve curve) {
    if (!animationStates.count(name)) {
        animationStates.insert({ name, { true, duration, 0.0, 0.0, false, curve} }); // (name -> direction, duration, progress, elapsed, finished, curve)
    }

}

// returns the state of an animation
double Animations::AnimationState(std::string name) {
    if (animationStates.count(name)) {
        return animationStates[name].progress;
    }
    else {
        return 0;
    }
    return 0;
}

// reverse an existing animation
void Animations::ReverseAnimation(std::string name, Animations::AnimationCurve curve) {
    if (animationStates.count(name) && !(count(reversedAnimations.begin(), reversedAnimations.end(), name) > 0)) {
        Animation& original = animationStates[name];
        Animation reversed;

        // Preserve original duration
        reversed.duration = original.duration;

        // Start from current progress (reversed)
        reversed.progress = original.progress;
        reversed.elapsed = reversed.progress * reversed.duration;
        reversed.curve = curve; // Preserve the curve type flip this if ease in to out etc

        // Flip direction and reset finished state
        reversed.direction = !original.direction;
        reversed.finished = false;

        // Update the animation
        animationStates.erase(name);
        animationStates.insert({ name, reversed });
        reversedAnimations.push_back(name);
    }
}

// Helper function to apply the animation curve
double Animations::ApplyCurve(double t, Animations::AnimationCurve curve) {
    switch (curve) {
    case Animations::AnimationCurve::EaseIn:
        return t * t;
    case Animations::AnimationCurve::EaseOut:
        return t * (2 - t);
    case Animations::AnimationCurve::EaseInOut:
        return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
    case Animations::AnimationCurve::EaseOutBounce:
    {
        // Apply ease-out first
        double easedT = t * (2 - t);

        // Bounce parameters (adjust these for different effects)
        const double bounceStart = 0.5;  // When bounce kicks in (0-1)
        const int bounces = 6;           // Number of bounces
        const double bouncePower = 0.6;  // How high the bounces go
        const double decay = 0.3;       // How fast bounces shrink (0-1, lower = slower decay)

        if (easedT > bounceStart) {
            double bounceT = (easedT - bounceStart) / (1 - bounceStart);
            // Exaggerated bounce using a decaying sine wave
            double bounce = sin(bounceT * M_PI * bounces) *
                pow(1 - bounceT, decay) *
                bouncePower;
            easedT += bounce;
        }
        return std::max(0.0, std::min(1.0, easedT));
    }
    case Animations::AnimationCurve::Linear:
    default:
        return t;
    }
}

// animates all animations based on their elapsed time and curve
void Animations::Animate(float delta) {
    std::map<std::string, Animation> updatedAnimations;

    for (auto& i : animationStates) {
        Animation& anim = i.second;

        anim.elapsed += anim.direction ? delta : -delta;
        anim.progress = anim.elapsed / anim.duration;

        if (anim.progress >= 1.0 && anim.direction) {
            anim.progress = 1.0;
            anim.finished = true;
        }
        else if (anim.progress <= 0.0 && !anim.direction) {
            anim.progress = 0.0;
            anim.finished = true;
            auto it = std::find(reversedAnimations.begin(), reversedAnimations.end(), i.first);
            if (it != reversedAnimations.end()) {
                reversedAnimations.erase(it);
            }
            continue;
        }

        double clampedProgress = std::max(0.0, std::min(1.0, anim.progress));
        double curvedProgress = ApplyCurve(clampedProgress, anim.curve);
        anim.progress = curvedProgress;

        updatedAnimations.insert({ i.first, anim });
    }

    this->animationStates = updatedAnimations;
}