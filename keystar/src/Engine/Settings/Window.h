#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class WindowSettings {

public:

	WindowSettings();
	~WindowSettings();

    std::string GetName();
    void SetName(std::string string);
    int GetWidth();
    int GetHeight();
    int GetFullscreen();
    void SetFullscreen(int fullscreen);
    void SetWidth(int width);
    void SetHeight(int height);

	bool DidScreenChange();
    void SetScreenChange(bool screen_change);

private:

    std::string name;
    
    int width;
    int height;
    int fullscreen;
    bool screenChange;

};

#endif
