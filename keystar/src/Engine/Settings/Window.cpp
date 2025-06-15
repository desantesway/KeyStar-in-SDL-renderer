#include "Window.h"

WindowSettings::WindowSettings() {
    this->name = "KeyStar";
    this->width = 1920;
    this->height = 1080;
    this->fullscreen = 0;
    this->screenChange = false;
}

WindowSettings::~WindowSettings() {

}

std::string WindowSettings::GetName() { return name; }
void WindowSettings::SetName(std::string string) { this->name = string; }
int WindowSettings::GetWidth() { return width; }
int WindowSettings::GetHeight() { return height; }
int WindowSettings::GetFullscreen() { return fullscreen; }
void WindowSettings::SetFullscreen(int fullscreen) { this->fullscreen = fullscreen; screenChange = true; }
void WindowSettings::SetWidth(int width) { this->width = width; screenChange = true; }
void WindowSettings::SetHeight(int height) { this->height = height; screenChange = true; }

bool WindowSettings::DidScreenChange() { return this->screenChange; }
void WindowSettings::SetScreenChange(bool screen_change) { this->screenChange = screen_change; }